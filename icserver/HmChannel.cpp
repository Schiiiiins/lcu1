#include "HmChannel.h"
#include "HmDevice.h"
#include "HmValue.h"
#include <sstream>
#include <Logger.h>
#include <xmlParser.h>
#include "XmlLoop.h"

using namespace XmlRpc;

HmChannel::HmChannel(HmDevice* device, int index)
{
	_device = device;
	_index = index;
}

HmChannel::~HmChannel(void)
{
}

void HmChannel::HandleXmlRpcEvent( const std::string& valueId, XmlRpc::XmlRpcValue& value )
{
	t_mapValues::iterator it = _values.find( valueId );
	if( it != _values.end() )
	{
		it->second->HandleXmlRpcEvent( value );
	}else{
		LOG( Logger::LOG_WARNING, "Value %s not found handling XmlRpc event", valueId.c_str());
	}
}

XmlRpcValue& HmChannel::GetValueDescription( const std::string& valueId )
{
	if( !_valuesParamsetDescription.valid() )
	{
		HmInterface* iface = _device->GetInterface();
		if( !iface )
		{
			return _valuesParamsetDescription;
		}
		XmlRpcValue params;
		params[0] = GetSerial();
		params[1] = "VALUES";
		if( !iface->ExecuteXmlRpc( "getParamsetDescription", params, &_valuesParamsetDescription ) )
		{
			_valuesParamsetDescription.clear();
			return _valuesParamsetDescription;
		}
	}
	if( !_valuesParamsetDescription.valid() )
	{
		return _valuesParamsetDescription;
	}
	return _valuesParamsetDescription[valueId];
}

XmlRpcValue HmChannel::GetValue( const std::string& valueId )
{
	HmInterface* iface = _device->GetInterface();
	if( !iface )
	{
		return XmlRpcValue();
	}
	XmlRpcValue params;
	params[0] = GetSerial();
	params[1] = valueId;
	XmlRpcValue result;
	if( !iface->ExecuteXmlRpc( "getValue", params, &result ) )
	{
		result.clear();
	}
	return result;
}

bool HmChannel::SetValue( const std::string& valueId, XmlRpcValue& v )
{
	HmInterface* iface = _device->GetInterface();
	if( !iface )
	{
		return false;
	}
	XmlRpcValue params;
	params[0] = GetSerial();
	params[1] = valueId;
	params[2] = v;
	iface->ExecuteXmlRpcAsync( "setValue", params );
	return true;
}

const std::string& HmChannel::GetSerial()
{
	if( _serial.empty() )
	{
		std::stringstream ss;
		ss << _device->GetSerial() << ":" << _index;
		_serial = ss.str();
	}
	return _serial;
}

void HmChannel::UpdateSerial()
{
	//Clear the serial number to force recalculation the next time it is used
	_serial.clear();
}

HmValue* HmChannel::AddValue(const std::string &valueId)
{
	HmValue*& value = _values[valueId];
	if( !value )
	{
		value = new HmValue(this, valueId);
	}
	return value;
}

bool HmChannel::ReadAllValues()
{
	bool success = true;
	for( t_mapValues::iterator it=_values.begin(); it!=_values.end(); it++ )
	{
		if( !it->second->TriggerValueGet() )success = false;
	}
	return success;
}

HmDevice* HmChannel::GetDevice()
{
	return _device;
}

bool HmChannel::ListValueConnections( XmlRpc::XmlRpcValue& list )
{
	for( t_mapValues::iterator it=_values.begin(); it!=_values.end(); it++ )
	{
		it->second->ListConnections( list );
	}
	return true;
}

bool HmChannel::ReadConfigurationFromXml( XMLNode& configNode )
{
	int i = 0;
	XMLNode parameterNode = configNode.getChildNode("parameter", &i);
	while( !parameterNode.isEmpty() )
	{
		HmConfigParameter parameter;
		if( !parameter.ReadFromXml( parameterNode ) )
		{
			return false;
		}
		_configParameters.push_back( parameter );
		parameterNode = configNode.getChildNode("parameter", &i);
	}
	return true;
}

bool HmChannel::SendConfigurationToDevice()
{
	XmlRpcValue params;

	const std::string& channelAddress = GetSerial();
	XmlRpcValue channelDescription;
	params[0] = channelAddress;
	if( !_device->GetInterface()->ExecuteXmlRpc( "getDeviceDescription", params, &channelDescription ) )
	{
		LOG( Logger::LOG_ERROR, "Error getting channel description for channel %s", channelAddress.c_str() );
		return false;
	}

	XmlRpcValue masterParamsetDescription;
	params[1] = "MASTER";
	if( !_device->GetInterface()->ExecuteXmlRpc( "getParamsetDescription", params, &masterParamsetDescription ) )
	{
		LOG( Logger::LOG_ERROR, "Error getting master paramset description for channel %s", channelAddress.c_str() );
		return false;
	}
	LOG( Logger::LOG_DEBUG, "Master paramset description: %s", masterParamsetDescription.toText().c_str());
	if( !masterParamsetDescription.valid() || !masterParamsetDescription.size() )return true;

	XmlRpcValue oldMasterParamsetValues;
	if( !_device->GetInterface()->ExecuteXmlRpc( "getParamset", params, &oldMasterParamsetValues ) )
	{
		LOG( Logger::LOG_ERROR, "Error getting master paramset for channel %s", channelAddress.c_str() );
		return false;
	}
	LOG( Logger::LOG_DEBUG, "Old master paramset: %s", oldMasterParamsetValues.toText().c_str());
	if( !oldMasterParamsetValues.valid() || !oldMasterParamsetValues.size() )return true;

	XmlRpcValue& newMasterParamsetValues = params[2];

	std::set<std::string> handledParameters;
	for( t_configParameters::iterator it = _configParameters.begin(); it != _configParameters.end(); it++ )
	{
		if( it->HasValue() )
		{
			XmlRpcValue& paramValue = oldMasterParamsetValues[it->GetName()];
			if( paramValue.valid() )
			{
				XmlRpcValue newValue = it->GetValue();
				if( newValue != paramValue )
				{
					newMasterParamsetValues[it->GetName()] = newValue;
				}
				handledParameters.insert( it->GetName() );
			}else{
				LOG(Logger::LOG_ERROR, "Parameter %s not found in MASTER paramset for channel %s", it->GetName().c_str(), channelAddress.c_str());
			}
		}
		if( !it->WantsResetToDefault() )
		{
			handledParameters.insert( it->GetName() );
		}
	}

	//set all values not handled to default
	XmlRpcValue::ValueStruct& masterParamsetDescriptionStruct = masterParamsetDescription;
	for( XmlRpcValue::ValueStruct::iterator it=masterParamsetDescriptionStruct.begin(); it != masterParamsetDescriptionStruct.end(); it++ )
	{
		if( handledParameters.find( it->first ) == handledParameters.end() )
		{
			if( oldMasterParamsetValues[it->first] != it->second["DEFAULT"] )
			{
				newMasterParamsetValues[it->first] = it->second["DEFAULT"];
			}
		}
	}
	LOG( Logger::LOG_DEBUG, "Changed master paramset values: %s", newMasterParamsetValues.toText().c_str());
	if( newMasterParamsetValues.valid() )
	{
		XmlRpcValue dummy;
		if( !_device->GetInterface()->ExecuteXmlRpc( "putParamset", params, &dummy ) )
		{
			LOG( Logger::LOG_ERROR, "Error setting master paramset for channel %s", channelAddress.c_str() );
			return false;
		}
	}
	return true;
}

HmValue* HmChannel::ReadValueFromXml( XMLNode& node, XmlLoop* loop )
{
	const char* temp = node.getAttribute("id");
	if( !temp )
	{
		LOG( Logger::LOG_ERROR, "<%s> is missing \"id\" attribute", node.getName());
		return NULL;
	}
	HmValue* hmValue = AddValue( temp );
	if( !hmValue->ReadParametersFromXml( node, loop ) )
	{
		return NULL;
	}

	return hmValue;
}

bool HmChannel::InitFromXml( XMLNode& node, XmlLoop* loop )
{
	int i=0;
	XMLNode valueNode = node.getChildNode("value", &i);
	while( !valueNode.isEmpty() )
	{
		HmValue* value = ReadValueFromXml( valueNode, loop );
		if( !value )
		{
			return false;
		}
		valueNode = node.getChildNode("value", &i);
	}

	XMLNode configurationNode = node.getChildNode("configuration");
	if( !configurationNode.isEmpty() )
	{
		if( !ReadConfigurationFromXml( configurationNode ))return false;
	}

	const char* temp = node.getAttribute("terminal");
	if( temp )
	{
		std::string terminal = temp;
		if( loop )
		{
			terminal = loop->ResolveIdentifier( terminal );
		}
		_terminal = terminal;
	}
	return true;
}

int HmChannel::GetIndex()
{
	return _index;
}

std::string HmChannel::GetTerminal()
{
	return _terminal;
}
