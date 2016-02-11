#ifdef _WIN32
#include <Winsock2.h>
#endif

#include "HmManager.h"
#include "HmInterface.h"
#include "HmDevice.h"
#include "HmChannel.h"
#include "HmValue.h"
#include "Manager.h"
#include "XmlLoop.h"
#include <XmlRpc.h>
#include <sstream>
#include <fstream>
#include <Logger.h>
#include <string.h>

using namespace XmlRpc;

#ifndef _WIN32
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

HmManager* HmManager::s_singleton = NULL;

HmManager* HmManager::GetSingleton()
{
	if( !s_singleton )s_singleton = new HmManager();
	return s_singleton;
}

HmManager::HmManager()
{
	if( !s_singleton )s_singleton = this;
}

HmManager::~HmManager(void)
{
	if( s_singleton == this )s_singleton = NULL;
}

#ifdef _WIN32
std::string HmManager::GetServerUrl()
{
	char szHostName[255];
	gethostname(szHostName, 255);

	struct hostent *host_entry;
	host_entry=gethostbyname(szHostName);

	char * szLocalIP;
	if( host_entry )
	{
		szLocalIP = inet_ntoa (*(struct in_addr *)*host_entry->h_addr_list);
	}else{
		szLocalIP = const_cast<char*>("127.0.0.1");
	}

	std::stringstream ss;
	ss << "bin://" << szLocalIP << ":" << Manager::GetSingleton()->GetXmlRpcServer()->getListenPort();
	return ss.str();
}
#else
std::string HmManager::GetServerUrl()
{
	std::string localIp = "127.0.0.1";

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if( sock != -1 )
	{
	    const char* kGoogleDnsIp = "8.8.8.8";
		uint16_t kDnsPort = 53;
		struct sockaddr_in serv;
		memset(&serv, 0, sizeof(serv));
		serv.sin_family = AF_INET;
		serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
		serv.sin_port = htons(kDnsPort);

		int err = connect(sock, (const sockaddr*) &serv, sizeof(serv));
		if( err != -1 )
		{

		    sockaddr_in name;
			socklen_t namelen = sizeof(name);
			err = getsockname(sock, (sockaddr*) &name, &namelen);
			if(err != -1)
			{
				char buffer[32];
			    localIp = inet_ntop(AF_INET, &name.sin_addr, buffer, sizeof(buffer));
			}
		}
		close(sock);
	}
	std::stringstream ss;
	ss << "bin://" << localIp << ":" << Manager::GetSingleton()->GetXmlRpcServer()->getListenPort();
	return ss.str();
}
#endif

HmInterface* HmManager::GetInterface( const std::string& id )
{
	t_mapInterfaces::iterator it = _interfaces.find( id );
	if( it == _interfaces.end() )return NULL;
	return it->second;
}

HmInterface* HmManager::AddInterface( const std::string& id, const std::string& url, bool userDefined )
{
	HmInterface*& iface = _interfaces[id];
	if( !iface )iface = new HmInterface( id, url, userDefined );
	return iface;
}

HmDevice* HmManager::GetDevice( const std::string& id )
{
	t_mapDevices::iterator it = _devices.find( id );
	if( it == _devices.end() )return NULL;
	return it->second;
}

HmDevice* HmManager::AddDevice( const std::string& id, const std::string& type )
{
	HmDevice*& device = _devices[id];
	if( !device )device = new HmDevice( id, type );
	return device;
}

void HmManager::HandleXmlRpcEvent( const std::string& interfaceId, const std::string& channel, const std::string& valueId, XmlRpcValue& v )
{
	if( _interfaces.find( interfaceId ) == _interfaces.end() )
	{
		LOG( Logger::LOG_WARNING, "Interface %s not found handling XmlRpc event", interfaceId.c_str());
		return;
	}
	_interfaces[ interfaceId ]->HandleXmlRpcEvent( channel, valueId, v );
}

bool HmManager::ReloadUserConfig( )
{
	ResetUserDefinedConnectionParameters();
	return LoadUserConfig( _userConfigFile );
}

bool HmManager::LoadUserConfig( const std::string& filename )
{
	_userConfigFile = filename;
	XMLResults xmlResult;
	XMLNode rootNode = XMLNode::parseFile( filename.c_str(), "Root", &xmlResult );
	if( xmlResult.error )
	{
		LOG( Logger::LOG_WARNING, "Error %d reading user config file %s (%d:%d)", xmlResult.error, filename.c_str(), xmlResult.nLine, xmlResult.nColumn);
		return false;
	}
	if( !ReadInterfacesFromXml( rootNode, false ) )return false;
	if( !ReadConnectionParametersFromXml( rootNode, false ) )return false;

	return true;
}

bool HmManager::SaveUserConfig( )
{
	LOG(Logger::LOG_DEBUG, "Saving user config");
	std::string new_filename=_userConfigFile+".new";
	std::string bak_filename=_userConfigFile+".bak";
	XMLNode n=XMLNode::createXMLTopNode();
	n.setNameConst("icserver_user");
	if(!SaveUserInterfacesToXml(&n))return false;
	if(!SaveUserConnectionParametersToXml(&n))return false;
	char* s=n.createXMLString(1);
	if(!s)return false;
	std::ofstream f(new_filename.c_str());
	if(f.good()){
		f<<s;
	}
	free(s);
	if(!f.good())return false;
	f.close();

	unlink(bak_filename.c_str());

	rename(_userConfigFile.c_str(), bak_filename.c_str());
	if(rename(new_filename.c_str(), _userConfigFile.c_str()))return false;
	unlink(bak_filename.c_str());
	return true;
}

bool HmManager::ReadDeviceFromXml( XMLNode& deviceNode, XmlLoop* loop )
{
	const char* temp = deviceNode.getAttribute("id");
	if( !temp )
	{
		LOG( Logger::LOG_ERROR, "<device> is missing \"id\" attribute");
		return false;
	}

	std::string deviceId = temp;
	if( loop )
	{
		deviceId = loop->ResolveIdentifier( deviceId );
	}

	if( GetDevice( deviceId ) )
	{
		LOG( Logger::LOG_ERROR, "Duplicate device, id=\"%s\"", deviceId.c_str());
		return false;
	}

	temp = deviceNode.getAttribute("type");
	if( !temp )
	{
		LOG( Logger::LOG_ERROR, "<device> is missing \"type\" attribute");
		return false;
	}
	std::string deviceType = temp;

	HmDevice* device = AddDevice( deviceId, deviceType );

	return device->InitFromXml( deviceNode, loop );

}

bool HmManager::ReadDevicesFromXml( XMLNode& rootNode )
{
	XMLNode devicesNode = rootNode.getChildNode("devices");
	int i = 0;
	XMLNode deviceNode = devicesNode.getChildNode("device", &i);
	while( !deviceNode.isEmpty() )
	{
		if(!ReadDeviceFromXml( deviceNode, NULL ))
		{
			return false;
		}
		deviceNode = devicesNode.getChildNode("device", &i);
	}
	i=0;
	XMLNode loopNode = devicesNode.getChildNode("loop", &i);
	while( !loopNode.isEmpty() )
	{
		XmlLoop loop;
		if( !loop.InitFromXml(loopNode) )
		{
			return false;
		}
		int j=0;
		XMLNode deviceNode = loopNode.getChildNode("device", &j);
		while( !deviceNode.isEmpty() )
		{
			loop.Reset();
			do{
				if(!ReadDeviceFromXml( deviceNode, &loop ))
				{
					return false;
				}
			}while( loop.Iterate() );
			deviceNode = loopNode.getChildNode("device", &j);
		}
		loopNode = devicesNode.getChildNode("loop", &i);
	}

	return true;
}

bool HmManager::ReadInterfacesFromXml( XMLNode& rootNode, bool fromSystemConfig )
{
	XMLNode interfacesNode = rootNode.getChildNode("interfaces");
	int i = 0;
	const char* temp;
	XMLNode interfaceNode = interfacesNode.getChildNode("interface", &i);
	while( !interfaceNode.isEmpty() )
	{
		temp = interfaceNode.getAttribute("id");
		if( !temp )
		{
			LOG( Logger::LOG_ERROR, "<interface> is missing \"id\" attribute");
			return false;
		}
		std::string interfaceId = temp;

		if( GetInterface( interfaceId ) )
		{
			LOG( Logger::LOG_ERROR, "Duplicate interface, id=\"%s\"", interfaceId.c_str());
			return false;
		}

		temp = interfaceNode.getAttribute("url");
		if( !temp )
		{
			LOG( Logger::LOG_ERROR, "<interface> id=\"%s\" is missing \"url\" attribute", interfaceId.c_str());
			return false;
		}
		AddInterface( interfaceId, temp, !fromSystemConfig );
		interfaceNode = interfacesNode.getChildNode("interface", &i);
	}
	return true;
}

bool HmManager::SaveUserInterfacesToXml( XMLNode* rootNode )
{
	XMLNode interfacesNode = rootNode->addChildConst("interfaces");

	for( t_mapInterfaces::iterator interfacesIt = _interfaces.begin(); interfacesIt!=_interfaces.end(); interfacesIt++ )
	{
		if(interfacesIt->second->IsUserDefined())
		{
			XMLNode interfaceNode = interfacesNode.addChildConst("interface");
			interfaceNode.addAttributeConst("id", interfacesIt->second->GetId().c_str());
			interfaceNode.addAttributeConst("url", interfacesIt->second->GetUrl().c_str());
		}
	}
	return true;
}

bool HmManager::SaveUserConnectionParametersToXml( XMLNode* rootNode )
{
	XMLNode devicesNode = rootNode->addChildConst("devices");

	for(t_mapDevices::iterator devicesIt = _devices.begin(); devicesIt!=_devices.end(); devicesIt++)
	{
		HmDevice* device = devicesIt->second;
		if(device->HasUserDefinedConnectionParameters())
		{
			HmInterface* iface = device->GetInterface();
			if( iface )
			{
				XMLNode deviceNode = devicesNode.addChildConst("device");
				deviceNode.addAttributeConst("id", devicesIt->second->GetId().c_str());
				deviceNode.addAttributeConst("address", devicesIt->second->GetSerial().c_str());
				deviceNode.addAttributeConst("interface", iface->GetId().c_str());
			}
		}
	}
	return true;
}

void HmManager::ResetUserDefinedConnectionParameters()
{
	for(t_mapDevices::iterator devicesIt = _devices.begin(); devicesIt!=_devices.end(); devicesIt++)
	{
		if(devicesIt->second->HasUserDefinedConnectionParameters())
		{
			devicesIt->second->ResetConnectionParameters();
		}
	}

	t_mapInterfaces::iterator interfacesIt = _interfaces.begin();
	while( interfacesIt!=_interfaces.end() )
	{
		if(interfacesIt->second->IsUserDefined())
		{
			delete interfacesIt->second;
			_interfaces.erase(interfacesIt);
			interfacesIt=_interfaces.begin();
		}else{
			interfacesIt++;
		}
	}
}

bool HmManager::ReadConnectionParametersFromXml( XMLNode& rootNode, bool fromSystemConfig )
{
	LOG( Logger::LOG_DEBUG, "Loading %s connection parameters", fromSystemConfig?"system":"user");
	XMLNode devicesNode = rootNode.getChildNode("devices");
	int i = 0;
	const char* temp;
	XMLNode deviceNode = devicesNode.getChildNode("device", &i);
	while( !deviceNode.isEmpty() )
	{
		temp = deviceNode.getAttribute("id");
		if( !temp )
		{
			LOG( Logger::LOG_ERROR, "<device> is missing \"id\" attribute");
			return false;
		}
		std::string deviceId = temp;

		HmDevice* device = GetDevice( deviceId );
		if( device )
		{

			if( device->GetInterface() )
			{
				LOG( Logger::LOG_ERROR, "Connection parameters for device \"%s\" duplicately defined", deviceId.c_str());
				return false;
			}

			temp = deviceNode.getAttribute("interface");
			if( temp )
			{
				std::string interfaceId = temp;
				HmInterface* iface = GetInterface( interfaceId );
				if( !iface )
				{
					LOG( Logger::LOG_ERROR, "Connection parameters for device %s: Undefined interface, id=\"%s\"", deviceId.c_str(), temp);
					return false;
				}

				temp = deviceNode.getAttribute("address");
				if( !temp )
				{
					LOG( Logger::LOG_ERROR, "Connection parameters: <device> id=\"%s\" is missing \"address\" attribute", deviceId.c_str());
					return false;
				}

				std::string deviceAddress = temp;

				device->SetConnectionParameters( iface, deviceAddress, !fromSystemConfig );
				LOG( Logger::LOG_DEBUG, "%s is at %s:%s", device->GetSerial().c_str(), interfaceId.c_str(), deviceAddress.c_str());
			}else{
				if( !fromSystemConfig )
				{
					LOG( Logger::LOG_ERROR, "Connection parameters: <device> id=\"%s\" is missing \"interface\" attribute", deviceId.c_str());
					return false;
				}
			} 

		} else {
			LOG( Logger::LOG_WARNING, "Undefined device in user connection parameters, id=\"%s\"", temp);
		}

		deviceNode = devicesNode.getChildNode("device", &i);
	}
	return true;
}

bool HmManager::GetInterfaces( XmlRpcValue* result )
{
	int i=0;
	for( t_mapInterfaces::iterator interfacesIt = _interfaces.begin(); interfacesIt!=_interfaces.end(); interfacesIt++ )
	{
		if(!interfacesIt->second->GetDescription(&(*result)[i]))return false;
		i++;
	}
	return true;
}

bool HmManager::GetDevices( XmlRpcValue* result )
{
	int i=0;
	for(t_mapDevices::iterator devicesIt = _devices.begin(); devicesIt!=_devices.end(); devicesIt++)
	{
		if(!devicesIt->second->GetDescription(&(*result)[i]))return false;
		i++;
	}
	return true;
}

bool HmManager::MapDevice( const std::string& deviceId, const std::string& interfaceId, const std::string& deviceAddress )
{
	HmDevice* device = GetDevice( deviceId );
	if( !device )
	{
		LOG(Logger::LOG_ERROR, "Trying to map unknown device %s", deviceId.c_str());
		return false;
	}
	if( !device->HasUserDefinedConnectionParameters() )
	{
		LOG(Logger::LOG_ERROR, "Trying to remap device %s, which has a system defined mapping", deviceId.c_str());
		return false;
	}
	HmInterface* iface = GetInterface(interfaceId);
	if( !iface )
	{
		LOG(Logger::LOG_ERROR, "Trying to map device %s to unknown interface %s", deviceId.c_str(), interfaceId.c_str());
		return false;
	}
	device->SetConnectionParameters( iface, deviceAddress, true );
	return SaveUserConfig();
}

bool HmManager::UnmapDevice( const std::string& deviceId )
{
	HmDevice* device = GetDevice( deviceId );
	if( !device )
	{
		LOG(Logger::LOG_ERROR, "Trying to unmap unknown device %s", deviceId.c_str());
		return false;
	}
	if( !device->HasUserDefinedConnectionParameters() )
	{
		LOG(Logger::LOG_ERROR, "Trying to unmap device %s, which has a system defined mapping", deviceId.c_str());
		return false;
	}
	device->ResetConnectionParameters();
	return SaveUserConfig();
}

bool HmManager::ListValueConnections( XmlRpc::XmlRpcValue& list )
{
	for(t_mapDevices::iterator devicesIt = _devices.begin(); devicesIt!=_devices.end(); devicesIt++)
	{
		devicesIt->second->ListValueConnections( list );
	}
	return true;
}


