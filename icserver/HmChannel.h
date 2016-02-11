#pragma once
#include <XmlRpc.h>
#include <xmlParser.h>
#include "HmConfigParameter.h"

class HmDevice;
class HmValue;
class XmlLoop;
class HmChannel
{
public:
	HmChannel(HmDevice* device, int index);
	~HmChannel(void);
	void HandleXmlRpcEvent( const std::string& valueId, XmlRpc::XmlRpcValue& value );
	XmlRpc::XmlRpcValue& GetValueDescription( const std::string& valueId );
	HmValue* AddValue( const std::string& valueId );
	bool SetValue( const std::string& valueId, XmlRpc::XmlRpcValue& v );
	XmlRpc::XmlRpcValue GetValue( const std::string& valueId );
	bool ReadAllValues();
	const std::string& GetSerial();
	HmDevice* GetDevice();
	bool ListValueConnections( XmlRpc::XmlRpcValue& list );
	void UpdateSerial();
	bool ReadConfigurationFromXml( XMLNode& configNode );
	bool SendConfigurationToDevice();
	bool InitFromXml( XMLNode& node, XmlLoop* loop );
	int GetIndex();
	std::string GetTerminal();

private:
	HmValue* ReadValueFromXml( XMLNode& node, XmlLoop* loop );
	HmDevice* _device;
	int _index;
	typedef std::map<std::string, HmValue*> t_mapValues;
	t_mapValues _values;
	XmlRpc::XmlRpcValue _valuesParamsetDescription;
	std::string _serial;
	std::string _terminal;
	typedef std::vector<HmConfigParameter> t_configParameters;
	t_configParameters _configParameters;
};
