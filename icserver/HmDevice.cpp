#include "HmDevice.h"
#include "HmChannel.h"
#include <Logger.h>
#include "HmConfigParameter.h"
#include <XmlRpc.h>
#include <set>
#include "XmlLoop.h"

using namespace XmlRpc;

HmDevice::HmDevice(const std::string& id, const std::string& type)
{
	_interface = NULL;
	_id = id;
	_type = type;
	_hasUserDefinedConnectionParameters = true;
}

HmDevice::~HmDevice(void)
{
}

bool HmDevice::HasUserDefinedConnectionParameters()
{
	return _hasUserDefinedConnectionParameters;
}

void HmDevice::HandleXmlRpcEvent( int channelIndex, const std::string& valueId, XmlRpc::XmlRpcValue& value )
{
	t_mapChannels::iterator it = _channels.find( channelIndex );
	if( it != _channels.end() )
	{
		it->second->HandleXmlRpcEvent( valueId, value );
	}else{
		LOG( Logger::LOG_WARNING, "Channel %s:%d not found handling XmlRpc event", _serial.c_str(), channelIndex);
	}
}

HmChannel* HmDevice::AddChannel( int index )
{
	HmChannel*& channel = _channels[index];
	if( !channel )
	{
		channel = new HmChannel( this, index );
	}
	return channel;
}

const std::string& HmDevice::GetSerial()
{
	return _serial;
}

const std::string& HmDevice::GetId()
{
	return _id;
}

const std::string& HmDevice::GetDisplayName()
{
	if( _displayName.size() )return _displayName;
	else return _id;
}

const std::string& HmDevice::GetType()
{
	return _type;
}

HmInterface* HmDevice::GetInterface()
{
	return _interface;
}

void HmDevice::ResetConnectionParameters()
{
	SetConnectionParameters(NULL, "", true);
}

bool HmDevice::SetConnectionParameters(HmInterface* iface, const std::string& serial, bool userDefined)
{
	bool success = true;
	_serial = serial;
	for( t_mapChannels::iterator it=_channels.begin(); it != _channels.end(); it++ )
	{
		it->second->UpdateSerial();
	}

	if( _interface != iface )
	{
		if(_interface)
		{
			_interface->RemoveDevice( this );
		}
		_interface = iface;
		if( _interface )
		{
			_interface->AddDevice( this );
			success = SendConfigurationToDevice();
			if( success )
			{
				success = ReadAllValues();
			}
		}
	}
	_hasUserDefinedConnectionParameters = userDefined;
	return success;
}

bool HmDevice::GetDescription( XmlRpc::XmlRpcValue *result )
{
	(*result)["Id"] = _id;
	(*result)["Type"] = _type;
	(*result)["DisplayName"] = GetDisplayName();
	if( _interface )
	{
		(*result)["MappedInterface"] = _interface->GetId();
		(*result)["MappedAddress"] = _serial;
		(bool&)(*result)["MappedByUser"] = _hasUserDefinedConnectionParameters;
	}
	return true;
}

bool HmDevice::SendConfigurationToDevice()
{
	if( !_interface ){
		LOG( Logger::LOG_ERROR, "Tried to send config data with no interface set on device %s", _id.c_str() );
		return false;
	}
	try{
		for( t_mapChannels::iterator it=_channels.begin(); it != _channels.end(); it++ )
		{
			it->second->SendConfigurationToDevice();
		}
	}catch( XmlRpcException e )
	{
		LOG( Logger::LOG_ERROR, "XmlRpx error: %s", e.getMessage().c_str() );
		return false;
	}
	return true;
}

bool HmDevice::ReadAllValues()
{
	bool success = true;
	for( t_mapChannels::iterator it=_channels.begin(); it != _channels.end(); it++ )
	{
		if( !it->second->ReadAllValues())success = false;
	}
	return success;
}

bool HmDevice::ListValueConnections( XmlRpc::XmlRpcValue& list )
{
	for( t_mapChannels::iterator it=_channels.begin(); it != _channels.end(); it++ )
	{
		it->second->ListValueConnections( list );
	}
	return true;
}

bool HmDevice::InitFromXml( XMLNode& node, XmlLoop* loop )
{
	const char* temp = node.getAttribute("display_name");
	if( temp )
	{
		_displayName = temp;
		if( loop )
		{
			_displayName = loop->ResolveIdentifier(_displayName);
		}
	}
	int i=0;
	XMLNode channelNode= node.getChildNode("channel", &i);
	while( !channelNode.isEmpty() )
	{
		const char* temp = channelNode.getAttribute("index");
		if( !temp )
		{
			LOG( Logger::LOG_ERROR, "<channel> missing \"index\" attribute");
			return false;
		}
		std::string sIndex = temp;
		if( loop )
		{
			sIndex = loop->ResolveIdentifier( sIndex );
		}
		int index = strtol(sIndex.c_str(), NULL, 0);
		HmChannel* channel = AddChannel(index);
		if( !channel->InitFromXml( channelNode, loop ) )return false;
		channelNode= node.getChildNode("channel", &i);
	}
	return true;
}
