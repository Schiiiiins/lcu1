#pragma once
#include <XmlRpc.h>
#include <map>
#include <xmlParser.h>

class HmInterface;
class HmValue;
class HmDevice;
class XmlLoop;
class HmManager
{
public:
	static HmManager* GetSingleton();
	~HmManager(void);
	std::string GetServerUrl();
	HmInterface* GetInterface( const std::string& id );
	HmInterface* AddInterface( const std::string& id, const std::string& url, bool UserDefined );
	void HandleXmlRpcEvent( const std::string& interfaceId, const std::string& channel, const std::string& valueId, XmlRpc::XmlRpcValue& v );
	bool ReadDevicesFromXml( XMLNode& rootNode );
	bool ReadInterfacesFromXml( XMLNode& rootNode, bool fromSystemConfig );
	bool ReadConnectionParametersFromXml( XMLNode& rootNode, bool fromSystemConfig );
	void ResetUserDefinedConnectionParameters();
	HmDevice* GetDevice( const std::string& id );
	HmDevice* AddDevice( const std::string& id, const std::string& type );
	bool LoadUserConfig( const std::string& filename );
	bool ReloadUserConfig( );
	bool GetInterfaces( XmlRpc::XmlRpcValue* result );
	bool GetDevices( XmlRpc::XmlRpcValue* result );
	bool MapDevice( const std::string& deviceId, const std::string& interfaceId, const std::string& deviceAddress );
	bool UnmapDevice( const std::string& deviceId );
	bool ListValueConnections( XmlRpc::XmlRpcValue& list );
private:
	bool SaveUserConfig( );
	bool SaveUserInterfacesToXml( XMLNode* rootNode );
	bool SaveUserConnectionParametersToXml( XMLNode* rootNode );
	bool ReadDeviceFromXml( XMLNode& deviceNode, XmlLoop* loop );
	HmManager();
	typedef std::map<std::string, HmInterface*> t_mapInterfaces;
	t_mapInterfaces _interfaces;
	typedef std::map<std::string, HmDevice*> t_mapDevices;
	t_mapDevices _devices;
	static HmManager* s_singleton;
	std::string _userConfigFile;
};
