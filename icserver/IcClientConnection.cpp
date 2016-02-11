#include "IcClientConnection.h"
#include <XmlRpc.h>
#include "Socket.h"
#include "IcChannel.h"
#include <cstdlib>
#include <Logger.h>

using namespace XmlRpc;

IcClientConnection::IcClientConnection(int socket) : XmlRpcSource( socket )
{
	_responseBufferBytesSent = 0;
	_eventBufferBytesSent = 0;
	_readBufferLength = 0;
	_currentBuffer = ResponseBuffer;
	Socket::setNonBlocking( socket );
    LOG(Logger::LOG_DEBUG, "New client connection");
}

IcClientConnection::~IcClientConnection(void)
{
}

void IcClientConnection::CmdRegisterClient( const std::string& name )
{
	LOG(Logger::LOG_DEBUG, "Registering client %s\n", name.c_str());
	_name = name;
}

void IcClientConnection::CmdUnregisterClient( const std::string& name )
{
	LOG(Logger::LOG_DEBUG, "Unregistering client %s\n", name.c_str());
	if( name == _name )_name.clear();
}

unsigned IcClientConnection::handleEvent( unsigned eventType )
{
	//printf("handleEvent(%d)\n", int(eventType));
	if( eventType == XmlRpcDispatch::ReadableEvent )
	{
		if(!HandleIncomingData())return 0;
	}
	if( eventType == XmlRpcDispatch::WritableEvent )
	{
		SendOutgoingData();
	}
	return XmlRpcDispatch::ReadableEvent;
}

unsigned IcClientConnection::getExtraMask() const 
{
	if( _responseBuffer.empty() && _eventBuffer.empty() )return 0;
	else return XmlRpcDispatch::WritableEvent;
}


bool IcClientConnection::HandleIncomingData()
{
	//printf("HandleIncomingData()\n");
	bool eof;
	bool result = true;
	if( !Socket::nbRead( getfd(), _readBuffer, &eof ))
	{
		LOG(Logger::LOG_ERROR, "socket error in connection %s\n", _name.c_str());
		return false;
	}
	if( eof ){
		return false;
	}
	while(true)
	{
		//printf("_readBuffer.size()=%u, _readBufferLength=%u\n", _readBuffer.size(), _readBufferLength);
		if( !_readBufferLength )
		{
			if( _readBuffer.length() >= 4 )
			{
				for( int i=0; i<4; i++ )
				{
					_readBufferLength <<= 8;
					_readBufferLength |= (unsigned char)_readBuffer[i];
				}
				_readBuffer.erase(0, 4);
			}else{
				//need more input
				break;
			}
		}else{
			if( _readBuffer.length() >= _readBufferLength )
			{
				result = ProcessIncomingMessage( _readBuffer.substr(0, _readBufferLength) );
				_readBuffer.erase(0, _readBufferLength);
				_readBufferLength = 0;
			}else{
				//need more input
				break;
			}
		}
	}
	return result;
}

bool IcClientConnection::ProcessIncomingMessage( const std::string& msg )
{
    LOG(Logger::LOG_DEBUG, "Incoming message %s", msg.c_str());
	std::string::size_type left = 0;
	std::string::size_type right;
	std::string response;
	while( left != std::string::npos )
	{
		IcChannel* newChannel = NULL;
		right = msg.find(',', left);
		switch(msg[left]){
			case 'N':
				CmdRegisterClient( msg.substr(left+1, right-left-1) );
				break;
			case 'n':
				CmdUnregisterClient( msg.substr(left+1, right-left-1) );
				break;
			case 'S':
				newChannel = CmdRegisterSender( msg.substr(left+1, right-left-1) );
				break;
			case 's':
				CmdUnregisterSender( msg.substr(left+1, right-left-1) );
				break;
			case 'R':
				newChannel = CmdRegisterReceiver( msg.substr(left+1, right-left-1) );
				break;
			case 'r':
				CmdUnregisterReceiver( msg.substr(left+1, right-left-1) );
				break;
			case 'X':
				//ToDo: exit iCserver
				break;
			case 'T':
				//ToDo: print tables
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				{
					std::string::size_type colonPos = msg.find( ':', left );
					if( (colonPos != std::string::npos) && (colonPos != msg.length()-1) ){
						unsigned long channelIndex = strtoul( msg.substr( left, colonPos-left ).c_str(), NULL, 10 );
						unsigned long value = strtoul( msg.substr( colonPos+1, right - colonPos - 1 ).c_str(), NULL, 10 );
						SendingSetValue( channelIndex, value );
					}
				}
				break;
			default:
				LOG(Logger::LOG_ERROR, "unexpected command received\n");
				return true;
		}
		if( newChannel )
		{
			char buffer[32];
			sprintf( buffer, "%d", newChannel->GetIndex());
			if( response.length() ){
				response += ",";
			}
			response += buffer;
		}
		left = right!=std::string::npos?right+1:std::string::npos;
	}
	if( response.length() )
	{
		std::string::size_type l = response.length();
		_responseBuffer.append(1, l>>24);
		_responseBuffer.append(1, l>>16);
		_responseBuffer.append(1, l>>8);
		_responseBuffer.append(1, l);
		_responseBuffer.append(response);
	}
	return true;
}

bool IcClientConnection::SendOutgoingData()
{
	if( (_currentBuffer == ResponseBuffer) && !_responseBuffer.empty() )
	{
		SendFromBuffer(_responseBuffer, &_responseBufferBytesSent);
	}else if( (_currentBuffer == EventBuffer) && !_eventBuffer.empty() )
	{
		SendFromBuffer(_eventBuffer, &_eventBufferBytesSent);
		if(_eventBuffer.empty())
		{
			_currentBuffer = ResponseBuffer;
		}
	}else if( !_responseBuffer.empty() )
	{
		SendFromBuffer(_responseBuffer, &_responseBufferBytesSent);
		_currentBuffer = ResponseBuffer;
	}else if( !_eventBuffer.empty() )
	{
		SendFromBuffer(_eventBuffer, &_eventBufferBytesSent);
		if(!_eventBuffer.empty())
		{
			_currentBuffer = EventBuffer;
		}else{
			_currentBuffer = ResponseBuffer;
		}
	}
	return true;
}

void IcClientConnection::SendFromBuffer(std::string& buffer, std::string::size_type* offset)
{
	Socket::nbWrite( getfd(), buffer, (int*)offset );
	if( *offset == buffer.length() )
	{
		buffer.clear();
		*offset = 0;
	}
}

void IcClientConnection::ReceivingSetValue( IcChannel* channel, unsigned long value )
{
	char buffer[32];
	int len = sprintf(buffer, "%d:%lu", channel->GetIndex(), value );
	_eventBuffer.append(1, len>>24);
	_eventBuffer.append(1, len>>16);
	_eventBuffer.append(1, len>>8);
	_eventBuffer.append(1, len);
	_eventBuffer.append( buffer, len );
	
}

const std::string& IcClientConnection::GetName()
{
	return _name;
}

