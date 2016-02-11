#include "HmInterface.h"
#include "HmManager.h"
#include "HmDevice.h"
#include <XmlRpc.h>
#include "AsyncXmlRpcSender.h"
#include <cstdlib>
#include "Logger.h"

using namespace XmlRpc;

HmInterface::HmInterface(const std::string& id, const std::string& url, bool isUserDefined)
{
	_id = id;
	_url = url;
	_isUserDefined = isUserDefined;
	Register();
}

HmInterface::~HmInterface(void)
{
	Unregister();
	for(t_mapDevices::iterator it=_devices.begin(); it!=_devices.end(); it++)
	{
		it->second->ResetConnectionParameters( );
	}
}

bool HmInterface::IsUserDefined()
{
	return _isUserDefined;
}

bool HmInterface::Register()
{
	XmlRpcValue params;
	params[0] = HmManager::GetSingleton()->GetServerUrl();
	params[1] = _id;
    (bool&)params[2] = true;
	XmlRpcValue result;
	return ExecuteXmlRpc( "init", params, &result );
}

bool HmInterface::Unregister()
{
	XmlRpcValue params;
	params[0] = HmManager::GetSingleton()->GetServerUrl();
	params[1] = "";
	XmlRpcValue result;
	return ExecuteXmlRpc( "init", params, &result );
}

void HmInterface::AddDevice( HmDevice* device )
{
	_devices[device->GetSerial()] = device;
}

void HmInterface::RemoveDevice( HmDevice* device )
{
	t_mapDevices::iterator it = _devices.find( device->GetSerial() );
	if( it != _devices.end() )
	{
		_devices.erase( it );
	}else{
		for(it=_devices.begin(); it!=_devices.end(); it++)
		{
			if( it->second == device )
			{
				_devices.erase( it );
				break;
			}
		}
	}
}

void HmInterface::HandleXmlRpcEvent( const std::string& channelSerial, const std::string& valueId, XmlRpcValue& value )
{
	std::string::size_type colonPos = channelSerial.find(':');
	std::string deviceSerial = channelSerial.substr(0, colonPos);
	int channelIndex = atoi(channelSerial.substr( colonPos+1 ).c_str());
	t_mapDevices::iterator it = _devices.find( deviceSerial );
	if( it != _devices.end() )
	{
		it->second->HandleXmlRpcEvent( channelIndex, valueId, value );
	}else{
		LOG( Logger::LOG_WARNING, "Device %s not found handling XmlRpc event", deviceSerial.c_str());
	}
}

void HmInterface::ExecuteXmlRpcAsync( const std::string& method, XmlRpcValue& params )
{
	LOG( Logger::LOG_DEBUG, "%s(%s)", method.c_str(), params.toText().c_str());
	AsyncXmlRpcSender* s = new AsyncXmlRpcSender( _url );
	s->AsyncCall( method, params );
}

bool HmInterface::ExecuteXmlRpc( const std::string& method, XmlRpcValue& params, XmlRpcValue* result)
{
	LOG( Logger::LOG_DEBUG, "%s:%s(%s)", _url.c_str(), method.c_str(), params.toText().c_str());
	XmlRpcClient c( _url );
	if( !c.execute( method.c_str(), params, *result ) )
	{
		return false;
	}
	if( c.isFault() )
	{
		return false;
	}
	return true;
}

const std::string& HmInterface::GetId()
{
	return _id;
}

const std::string& HmInterface::GetUrl()
{
	return _url;
}

bool HmInterface::GetDescription( XmlRpc::XmlRpcValue *result )
{
	(*result)["Id"] = _id;
	(*result)["Url"] = _url;
	(bool&)(*result)["IsUserDefined"] = _isUserDefined;
	return true;
}
