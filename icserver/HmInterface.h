#pragma once
#include <string>
#include <XmlRpc.h>
#include <map>

class HmManager;
class HmDevice;

class HmInterface
{
public:
	HmInterface(const std::string& id, const std::string& url, bool isUserDefined);
	~HmInterface(void);
	bool Register();
	bool Unregister();
	void AddDevice( HmDevice* device );
	void RemoveDevice( HmDevice* device );
	bool IsUserDefined();
	const std::string& GetId();
	const std::string& GetUrl();
	void HandleXmlRpcEvent( const std::string& channelSerial, const std::string& valueId, XmlRpc::XmlRpcValue& value );
	bool ExecuteXmlRpc( const std::string& method, XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue* result);
	void ExecuteXmlRpcAsync( const std::string& method, XmlRpc::XmlRpcValue& params );
	bool GetDescription( XmlRpc::XmlRpcValue *result );
private:
	std::string _url;
	std::string _id;
	typedef std::map<std::string, HmDevice*> t_mapDevices;
	t_mapDevices _devices;
	bool _isUserDefined;
};
