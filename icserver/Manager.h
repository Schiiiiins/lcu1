#pragma once
#include <map>
#include <string>
#include <XmlRpc.h>
#include <xmlParser.h>

class IcClientConnection;
class IcChannel;
class XmlLoop;

class Manager: public XmlRpc::XmlRpcSource
{
public:
	static Manager* GetSingleton();
	~Manager(void);
	bool InitServers();
	void Run();
	IcChannel* GetChannel( const std::string& name );
	IcChannel* AddChannel( const std::string& name );
	bool AddChannelAlias( const std::string& name, const std::string& alias );
    //! Return true to continue monitoring this source
    unsigned handleEvent(unsigned eventType);
	XmlRpc::XmlRpcServer* GetXmlRpcServer();
	bool LoadSystemConfig( const std::string& configFilename, const std::string& mappingFilename );
	bool LoadUserConfig( const std::string& filename );
	bool GetValue( const std::string& name, unsigned long* value );
	bool SetValue( const std::string& name, unsigned long value );
	bool LoadPersistentValues( const std::string& filename );
	void SavePersistentValues( );
private:
	bool DoSavePersistentValues( );
	Manager(void);
	void HandleServerEvent();
	void RemoveConnection( IcClientConnection* conn );
	bool ReadValuesFromXml( XMLNode& rootNode );
	bool ReadValueFromXml( XMLNode& valueNode, XmlLoop* loop );

	typedef std::map<std::string, IcChannel*> t_mapChannelsByName;
	t_mapChannelsByName _mapChannelsByName;
	int _nextChannelIndex;
	XmlRpc::XmlRpcServer _xmlRpcServer;
	static Manager* s_singleton;
	std::string _persistedValuesFile;
	//bool _persistValuesInhibit;
	bool _valuesSaveRequired;
};
