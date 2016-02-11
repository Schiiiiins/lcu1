#include "IcClient.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <Logger.h>

IcClient::IcClient(const std::string& host, int port)
{
	_host = host;
	_port = port;
	_fd = -1;
}

IcClient::~IcClient(void)
{
	Disconnect();
}

bool IcClient::Connect( )
{
	if( _fd >= 0 )return false;
	_fd = ::socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;

	struct hostent *hp = gethostbyname(_host.c_str());
	if (hp == 0) return false;

	saddr.sin_family = hp->h_addrtype;
	memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);
	saddr.sin_port = htons((u_short) _port);

	int result = ::connect(_fd, (struct sockaddr *)&saddr, sizeof(saddr));
	if( result )
	{
		Disconnect();
		LOG( Logger::LOG_ERROR, "Error %d connecting to %s:%d", result, _host.c_str(), _port);
		return false;
	}
	fcntl(_fd, F_SETFL, O_NONBLOCK);
	LOG( Logger::LOG_DEBUG, "Connected to %s:%d", _host.c_str(), _port);
	SendMessage("Nui");
	return true;
}

void IcClient::Disconnect()
{
	if( _fd >= 0 )
	{
		close(_fd);
		_fd = -1;
	}
	for(t_mapValues::iterator it=_mapValues.begin(); it!= _mapValues.end(); it++)
	{
		delete it->second;
	}
	_mapValues.clear();
	_mapValuesByNumId.clear();
	_setReceivedValues.clear();
}

unsigned long IcClient::GetValue(const std::string& id)
{
	LOG(Logger::LOG_DEBUG, "GetValue(\"%s\")", id.c_str());

	ValueInfo* valueInfo = GetValueInfo( id );
	if( !(valueInfo->flags & ValueFlagReceiver) )
	{
		if(!RegisterChannel(id, false ))return 0;
	}
	while( !(valueInfo->flags & ValueFlagReceived) )
	{
		std::string msg;
		if(!ReceiveNextMessage(&msg, ValueGetTimeout))
		{
			LOG(Logger::LOG_ERROR, "Error getting value %s", id.c_str());
			return 0;
		}
		ProcessValueMessage( msg );
	}
	LOG(Logger::LOG_DEBUG, "%s=%lu", id.c_str(), valueInfo->value);
	return valueInfo->value;
}

bool IcClient::SetValue(const std::string& id, unsigned long value)
{
	ValueInfo* valueInfo = GetValueInfo( id );
	if( !(valueInfo->flags & ValueFlagSender) )
	{
		if(!RegisterChannel(id, true ))return false;
	}
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%d:%lu", valueInfo->numId, value);
	if( !SendMessage( buffer ) )return false;
	valueInfo->value = value;
	return true;
}

bool IcClient::RegisterChannel( const std::string& id, bool sender )
{
	LOG(Logger::LOG_DEBUG, "Registering channel %s for %s", id.c_str(), sender?"sending":"receiving");
	ValueInfo* valueInfo = GetValueInfo( id );

	if( !IsConnected() )
	{
		if( !Connect() )
		{
			LOG(Logger::LOG_ERROR, "Could not connect");
			return false;
		}
	}

	std::string msg;
	if( sender )
	{
		if( valueInfo->flags & ValueFlagSender )
		{
			LOG(Logger::LOG_WARNING, "Channel %s already registered as sender", id.c_str());
			return true;
		}
		msg += 'S';
	}else{
		if( valueInfo->flags & ValueFlagReceiver )
		{
			LOG(Logger::LOG_WARNING, "Channel %s already registered as receiver", id.c_str());
			return true;
		}
		msg += 'R';
	}
	msg += id;
	if( !SendMessage( msg ) )
	{
		LOG(Logger::LOG_ERROR, "Error registering channel %s for %s", id.c_str(), sender?"sending":"receiving");
		return false;
	}
	std::string channelList;
	if( !ReceiveChannelListMessage( &channelList ) )
	{
		LOG(Logger::LOG_ERROR, "Error receiving channel list registering channel %s for %s", id.c_str(), sender?"sending":"receiving");
		return false;
	}
	char* endp;
	int numId = strtol( channelList.c_str(), &endp, 10 );
	if( *endp )
	{
		LOG(Logger::LOG_ERROR, "Error parsing channel list %s registering channel %s for %s", channelList.c_str(), id.c_str(), sender?"sending":"receiving");
		return false;
	}
	LOG(Logger::LOG_DEBUG, "Channel %s has id %d", id.c_str(), numId);
	valueInfo->numId = numId;
	valueInfo->flags |= sender?ValueFlagSender:ValueFlagReceiver;
	return true;
}

bool IcClient::ReceiveChannelListMessage(std::string* channelList)
{
	std::string msg;
	while( ReceiveNextMessage( &msg, ReceiveTimeout ) )
	{
		if( msg.find(':') != std::string::npos )
		{
			ProcessValueMessage( msg );
		}else{
			*channelList = msg;
			return true;
		}
	}
	return false;
}

bool IcClient::ProcessValueMessage( const std::string& msg )
{
	char* endp;
	int id = strtol(msg.data(), &endp, 10);
	if( *endp != ':' )return false;
	unsigned long value = strtoul(endp + 1, NULL, 0);

	ValueInfo* valueInfo = GetValueInfo( id );
	if( !valueInfo )
	{
		LOG(Logger::LOG_ERROR, "Unknown cahnnel id %d", id);
		return false;
	}
	LOG(Logger::LOG_DEBUG, "%s=%lu", valueInfo->strId.c_str(), value);
	if( (!(valueInfo->flags & ValueFlagReceived)) || valueInfo->value != value )
	{
		valueInfo->flags |= ValueFlagReceived;
		valueInfo->value = value;
		_setReceivedValues.insert( valueInfo );
	}
	return true;
}

bool IcClient::GetNextReceivedValue( std::string* id, unsigned long* value )
{
	if( _setReceivedValues.empty() )return NULL;
	ValueInfo* valueInfo = *_setReceivedValues.begin();
	_setReceivedValues.erase( _setReceivedValues.begin() );
	*id = valueInfo->strId;
	*value = valueInfo->value;
	return true;
}

void IcClient::DoPendingWork()
{
	while( Wait( WaitFlagReadable, 0 ) )
	{
		std::string msg;
		while(ReceiveNextMessage( &msg, 0 ))
		{
			ProcessValueMessage( msg );
		}
	}
}

bool IcClient::ReceiveNextMessage( std::string* msg, int timeout )
{
	while(true)
	{
		unsigned int msgSize = 0;
		if( _receiveBuffer.size()>=4)
		{
			msgSize = (_receiveBuffer[0]<<24)|(_receiveBuffer[1]<<16)|(_receiveBuffer[2]<<8)|(_receiveBuffer[3]);
			LOG(Logger::LOG_DEBUG, "Expecting %u byte long message", msgSize);
		}
		if( _receiveBuffer.size() >= msgSize+4 )
		{
			*msg = _receiveBuffer.substr(4, msgSize);
			_receiveBuffer = _receiveBuffer.substr(4+msgSize);
			LOG(Logger::LOG_DEBUG, "Received \"%s\"", msg->c_str());
			return true;
		}
		if( !Wait( WaitFlagReadable, timeout ))
		{
			if( timeout )LOG(Logger::LOG_WARNING, "Receive timeout");
			return false;
		}
		char buffer[1024];
		int count = read(_fd, buffer, sizeof(buffer));
		if( !count )
		{
			//disconnected from the remote side
			Disconnect();
			return false;
		}
		if( count <= 0 )
		{
			LOG(Logger::LOG_ERROR, "Read error");
			return false;
		}
		_receiveBuffer.append(buffer, count);
	}
}

bool IcClient::SendMessage( const std::string& msg )
{
	LOG(Logger::LOG_DEBUG, "SendMessage(\"%s\")", msg.c_str());
	std::string rawMessage;
	unsigned long size = msg.size();
	rawMessage.reserve(size + 4);
	for( int i=24; i>=0; i-=8)
	{
		rawMessage.append(1, (size>>i)&0xff);
	}
	rawMessage += msg;
	unsigned int bytesSent = 0;
	while( bytesSent < rawMessage.size() )
	{
		if( !Wait( WaitFlagWritable, SendTimeout ))
		{
			LOG(Logger::LOG_ERROR, "Send timeout");
			return false;
		}
		int count = write(_fd, rawMessage.data() + bytesSent, rawMessage.size() - bytesSent);
		if( count <= 0 )
		{
			LOG(Logger::LOG_WARNING, "Write error %d", count);
			return false;
		}
		bytesSent += count;
	}
	return true;
}

int IcClient::Wait( int flags, int timeout )
{
	if( !IsConnected() )return 0;
	// Construct the sets of descriptors we are interested in
	fd_set inFd, outFd, excFd;
	FD_ZERO(&inFd);
	FD_ZERO(&outFd);
	FD_ZERO(&excFd);
	
	if( flags & WaitFlagWritable )	FD_SET(_fd, &outFd);
	if( flags & WaitFlagReadable )	FD_SET(_fd, &inFd);

	flags = 0;	
	// Check for events
	int nEvents;
	if (timeout < 0){
		nEvents = select(_fd+1, &inFd, &outFd, &excFd, NULL);
	}
	else 
	{
		struct timeval tv;
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		nEvents = select(_fd+1, &inFd, &outFd, &excFd, &tv);
	}
	if( nEvents < 0 )
	{
		LOG( Logger::LOG_ERROR, "Error calling select: %d", nEvents );
		return 0;
	}
	if( FD_ISSET(_fd, &inFd) )flags |= WaitFlagReadable;
	if( FD_ISSET(_fd, &outFd) )flags |= WaitFlagWritable;
	return flags;
}

bool IcClient::IsConnected()
{
	return _fd >= 0;
}

IcClient::ValueInfo* IcClient::GetValueInfo( const std::string& id )
{
	ValueInfo* valueInfo = _mapValues[id];
	if( !valueInfo )
	{
		valueInfo = new ValueInfo();
		valueInfo->strId = id;
		_mapValues[id] = valueInfo;
	}
	return valueInfo;
}

IcClient::ValueInfo* IcClient::GetValueInfo( int numId )
{
	ValueInfo* valueInfo = _mapValuesByNumId[numId];
	if( !valueInfo )
	{
		for(t_mapValues::iterator it=_mapValues.begin(); it!= _mapValues.end(); it++)
		{
			if( it->second->numId == numId )
			{
				_mapValuesByNumId[numId] = it->second;
				valueInfo = it->second;
			}
		}
	}
	return valueInfo;
}

int IcClient::GetFd()
{
	return _fd;
}