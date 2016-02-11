#pragma once
#include "HmInterface.h"
#include <string>
#include <xmlParser.h>
#include <vector>

class HmChannel;
class XmlLoop;

class HmDevice
{
public:
	HmDevice(const std::string& id, const std::string& type);
	~HmDevice(void);
	void HandleXmlRpcEvent( int channelIndex, const std::string& valueId, XmlRpc::XmlRpcValue& value );
	HmChannel* AddChannel( int index );
	const std::string& GetSerial();
	const std::string& GetType();
	const std::string& GetId();
	const std::string& GetDisplayName();
	HmInterface* GetInterface();
	bool SetConnectionParameters(HmInterface* iface, const std::string& serial, bool userDefined);
	void ResetConnectionParameters();
	bool HasUserDefinedConnectionParameters();
	bool GetDescription( XmlRpc::XmlRpcValue *result );
	bool SendConfigurationToDevice();
	bool ListValueConnections( XmlRpc::XmlRpcValue& list );
	bool InitFromXml( XMLNode& node, XmlLoop* loop );
private:
	bool ReadAllValues();
	std::string _id;
	std::string _displayName;
	std::string _type;
	HmInterface* _interface;
	std::string _serial;
	typedef std::map<int, HmChannel*> t_mapChannels;
	t_mapChannels _channels;
	bool _hasUserDefinedConnectionParameters;
};
