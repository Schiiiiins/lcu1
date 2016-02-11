#include "Connection.h"
#include "Manager.h"
#include "IcChannel.h"
#include <cstdlib>
#include <Logger.h>

using namespace XmlRpc;

Connection::Connection( )
{
}

Connection::~Connection(void)
{
	while( _mapReceivingChannelsByName.begin() != _mapReceivingChannelsByName.end() )
	{
		t_mapChannelsByName::iterator it = _mapReceivingChannelsByName.begin();
		_mapReceivingChannelsByName.erase( it );
	}

	while( _mapSendingChannelsByName.begin() != _mapSendingChannelsByName.end() )
	{
		t_mapChannelsByName::iterator it = _mapSendingChannelsByName.begin();
		IcChannel* channel = it->second;
		_mapSendingChannelsByIndex.erase( channel->GetIndex() );
		_mapSendingChannelsByName.erase( it );
	}
}

void Connection::GetNameAndBitmaskFromChannelSpec( const std::string& channelSpec, std::string* name, unsigned long* bitmask )
{
	std::string::size_type openingBracket = channelSpec.find('(');
	if( openingBracket != std::string::npos )
	{
		std::string::size_type closingBracket = channelSpec.find(')', openingBracket);
		if( closingBracket != std::string::npos )
		{
			*bitmask = atoi(channelSpec.substr( openingBracket+1, closingBracket-openingBracket-1 ).c_str());
			*name = channelSpec.substr(0, openingBracket);
			return;
		}
	}
	*bitmask = (unsigned long)-1;
	*name = channelSpec;
}

IcChannel* Connection::CmdRegisterSender( const std::string& channelSpec )
{
	std::string name;
	unsigned long bitmask;
	GetNameAndBitmaskFromChannelSpec( channelSpec, &name, &bitmask );
	IcChannel* channel = Manager::GetSingleton()->GetChannel( name );
	if( !channel )
	{
		LOG(Logger::LOG_WARNING, "Registering sender for undefined channel %s", name.c_str());
		channel = Manager::GetSingleton()->AddChannel( name );
	}
	_mapSendingChannelsByName[name] = channel;
	_mapSendingChannelsByIndex[channel->GetIndex()] = channel;
	unsigned long undefined_bits = channel->GetUndefinedBits();
	channel->SetValue(0, undefined_bits & bitmask);
	LOG(Logger::LOG_DEBUG, "Registering sender %s=%d\n", name.c_str(), channel->GetIndex());
	return channel;
}

void Connection::CmdUnregisterSender( const std::string& channelSpec )
{
	std::string name;
	unsigned long bitmask;
	GetNameAndBitmaskFromChannelSpec( channelSpec, &name, &bitmask );
	LOG(Logger::LOG_DEBUG, "Unregistering sender %s\n", name.c_str());
	t_mapChannelsByName::iterator it = _mapSendingChannelsByName.find( name );
	if( it != _mapSendingChannelsByName.end() )
	{
		_mapSendingChannelsByName.erase( it );
		_mapSendingChannelsByIndex.erase( it->second->GetIndex() );
	}
}

IcChannel* Connection::CmdRegisterReceiver( const std::string& channelSpec )
{
	std::string name;
	unsigned long bitmask;
	GetNameAndBitmaskFromChannelSpec( channelSpec, &name, &bitmask );
	IcChannel* channel = Manager::GetSingleton()->GetChannel( name );
	if( !channel )
	{
		LOG(Logger::LOG_WARNING, "Registering receiver for undefined channel %s", name.c_str());
		channel = Manager::GetSingleton()->AddChannel( name );
	}
	_mapReceivingChannelsByName[name] = channel;
	channel->AddReceiver(this, bitmask);
	//push the initial value
	unsigned long channelValue = channel->GetValue();
	ReceivingSetValue(channel, channelValue);
	LOG(Logger::LOG_DEBUG, "Registering receiver %s=%d\n", name.c_str(), channel->GetIndex());
	return channel;
}

void Connection::CmdUnregisterReceiver( const std::string& channelSpec )
{
	std::string name;
	unsigned long bitmask;
	GetNameAndBitmaskFromChannelSpec( channelSpec, &name, &bitmask );
	LOG(Logger::LOG_DEBUG, "Unregistering receiver %s\n", name.c_str());
	t_mapChannelsByName::iterator it = _mapReceivingChannelsByName.find( name );
	if( it != _mapReceivingChannelsByName.end() )
	{
		it->second->RemoveReceiver(this, bitmask);
		_mapReceivingChannelsByName.erase( it );
	}
}

void Connection::SendingSetValue( unsigned long channelIndex, unsigned long value )
{
	IcChannel* channel = _mapSendingChannelsByIndex[channelIndex];
	if( !channel )return;
	LOG(Logger::LOG_DEBUG, "Sending data %s=%lu\n", channel->GetName().c_str(), value);
	channel->SetValue( value );
}

std::vector<IcChannel*> Connection::GetReceivingChannels()
{
	std::vector<IcChannel*> vecChannels(_mapReceivingChannelsByName.size());
	int i=0;
	for( t_mapChannelsByName::iterator it = _mapReceivingChannelsByName.begin(); it!=_mapReceivingChannelsByName.end(); it++ )
	{
		vecChannels[i] = it->second;
	}
	return vecChannels;
}

