#include "HmValue.h"
#include "IcChannel.h"
#include "HmValueConnection.h"
#include "Manager.h"
#include "HmChannel.h"
#include <sstream>
#include <cstdlib>
#include <Logger.h>
#include "XmlLoop.h"

using namespace XmlRpc;

HmValue::HmValue(HmChannel* channel, const std::string& valueId )
{
	_channel = channel;
	_valueId = valueId;
	_sendingChannel = NULL;
	_receivingConnection = NULL;
	_floatScale = 100.0;
	_receivingConnection = NULL;
	_boolInvert = false;
}

HmValue::~HmValue(void)
{
	if( _sendingChannel )
	{
		_sendingChannel->SetSourceHmValue( NULL );
	}
}

bool HmValue::ReadParametersFromXml( XMLNode& node, XmlLoop* loop )
{
	const char* temp = node.getAttribute("float_scale");
	if( temp )
	{
		char* endp;
		_floatScale = strtod( temp, &endp );
		if( *endp != 0 )
		{
			LOG( Logger::LOG_ERROR, "<%s> has an invalid \"float_scale\" attribute: %s", node.getName(), temp);
			return false;
		}
	}

	temp = node.getAttribute("bool_invert");
	if( temp )
	{
		_boolInvert = *temp == 't';
	}

	bool connected = false;
	temp = node.getAttribute("source");
	if( temp )
	{
		std::string channelName = temp;
		if( loop )channelName = loop->ResolveIdentifier( channelName );
		if( !SetReceiver( channelName ) )
		{
			LOG( Logger::LOG_ERROR, "Error setting source value for %s:%d.%s", _channel->GetDevice()->GetId().c_str(), _channel->GetIndex(), GetId().c_str());
			return false;
		}
		connected = true;
	}

	temp = node.getAttribute("target");
	if( temp )
	{
		std::string channelName = temp;
		if( loop )channelName = loop->ResolveIdentifier( channelName );
		if( !SetSender( channelName ) )
		{
			LOG( Logger::LOG_ERROR, "Error setting target value for %s:%d.%s", _channel->GetDevice()->GetId().c_str(), _channel->GetIndex(), GetId().c_str());
			return false;
		}
		connected = true;
	}
	if( !connected )
	{
		LOG( Logger::LOG_ERROR, "You must specify source or target for HM value %s:%d.%s", _channel->GetDevice()->GetId().c_str(), _channel->GetIndex(), GetId().c_str());
		return false;
	}
	return true;
}

bool HmValue::SetReceiver( const std::string& channelName )
{
	if( _receivingConnection )return false;
	_receivingConnection = new HmValueConnection( this, channelName );
	return true;
}

bool HmValue::SetSender( const std::string& channelName )
{
	if( _sendingChannel )return false;

	_sendingChannel = Manager::GetSingleton()->AddChannel( channelName );
	_sendingChannel->SetSourceHmValue( this );
	return true;
}

bool HmValue::TriggerValueGet()
{
	if( _sendingChannel )
	{
		XmlRpcValue value = _channel->GetValue(_valueId);
		if( value.valid() )
		{
			HandleXmlRpcEvent( value );
			return true;
		}else{
			_sendingChannel->MarkAsUndefined();
			LOG( Logger::LOG_WARNING, "Error getting value %s.%s", _channel->GetSerial().c_str(), _valueId.c_str());
			return false;
		}
	}else{
		return true;
	}
}

void HmValue::HandleXmlRpcEvent( XmlRpcValue& value )
{
	LOG( Logger::LOG_DEBUG, "HmValue %s.%s=%s", _channel->GetSerial().c_str(), _valueId.c_str(), value.toText().c_str());
	if( _sendingChannel )
	{
		if( value.getType() == XmlRpcValue::TypeInt )
		{
			_sendingChannel->SetValue( (int&)value );
		}else if( value.getType() == XmlRpcValue::TypeDouble )
		{
			_sendingChannel->SetValue( (unsigned long)(((double&)value) * _floatScale) );
		}else if( value.getType() == XmlRpcValue::TypeBoolean )
		{
			_sendingChannel->SetValue( ((bool&)value) != _boolInvert ? 1 : 0 );
		}else{
			LOG( Logger::LOG_WARNING, "Unsupported type of value %s handling XmlRpc event", value.toText().c_str());
		}
	}else{
		LOG( Logger::LOG_WARNING, "No IcChannel defined handling XmlRpc event");
	}
}

void HmValue::ReceivingSetValue( unsigned long value )
{
	XmlRpcValue& description = _channel->GetValueDescription( _valueId );
	if( !description.valid() )return;
	XmlRpcValue v;
	switch( description["DEFAULT"].getType() )
	{
	case XmlRpcValue::TypeBoolean:
		(bool&)v = ((value & 0x01) != 0) != _boolInvert;
		break;
	case XmlRpcValue::TypeDouble:
		(double&)v = double(value) / _floatScale;
		break;
	case XmlRpcValue::TypeInt:
		(int&)v = value;
		break;
	default:
		break;
	}
	if( v.valid() )
	{
		_channel->SetValue( _valueId, v );
	}
}

HmChannel* HmValue::GetChannel()
{
	return _channel;
}

const std::string& HmValue::GetId()
{
	return _valueId;
}

bool HmValue::ListConnections( XmlRpc::XmlRpcValue& list )
{
	if( _sendingChannel )
	{
		XmlRpcValue dict;
		dict["Alias"] = _sendingChannel->GetAlias();
		dict["Name"] = _sendingChannel->GetName();
		dict["DisplayName"] = _sendingChannel->GetDisplayName();
		if( _channel->GetDevice()->GetInterface() )
		{
			dict["XmlRpcInterfaceId"] = _channel->GetDevice()->GetInterface()->GetId();
			dict["XmlRpcInterfaceUrl"] = _channel->GetDevice()->GetInterface()->GetUrl();
			dict["XmlRpcDevice"] = _channel->GetDevice()->GetSerial();
			dict["XmlRpcChannel"] = _channel->GetSerial();
			dict["XmlRpcValue"] = GetId();
		}
		dict["Device"] = _channel->GetDevice()->GetId();
		dict["DeviceDisplayName"] = _channel->GetDevice()->GetDisplayName();
		dict["ChannelIndex"] = _channel->GetIndex();
		dict["Terminal"] = _channel->GetTerminal();
		dict["Direction"] = "Input";
		list[list.size()] = dict;
	}
	if( _receivingConnection )
	{
		std::vector<IcChannel*> receivingChannels = _receivingConnection->GetReceivingChannels();
		for( unsigned int i=0; i<receivingChannels.size(); i++ )
		{
			XmlRpcValue dict;
			dict["Alias"] = receivingChannels[i]->GetAlias();
			dict["Name"] = receivingChannels[i]->GetName();
			dict["DisplayName"] = receivingChannels[i]->GetDisplayName();
			if( _channel->GetDevice()->GetInterface() )
			{
				dict["XmlRpcInterfaceId"] = _channel->GetDevice()->GetInterface()->GetId();
				dict["XmlRpcInterfaceUrl"] = _channel->GetDevice()->GetInterface()->GetUrl();
				dict["XmlRpcDevice"] = _channel->GetDevice()->GetSerial();
				dict["XmlRpcChannel"] = _channel->GetSerial();
				dict["XmlRpcValue"] = GetId();
			}
			dict["Device"] = _channel->GetDevice()->GetId();
			dict["DeviceDisplayName"] = _channel->GetDevice()->GetDisplayName();
			dict["ChannelIndex"] = _channel->GetIndex();
			dict["Terminal"] = _channel->GetTerminal();
			dict["Direction"] = "Output";
			list[list.size()] = dict;
		}
	}
	return true;
}

