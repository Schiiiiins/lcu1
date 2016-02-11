#pragma once
#include "HmDevice.h"
#include <string>
#include <XmlRpc.h>

class IcChannel;
class HmValueConnection;
class XmlLoop;

class HmValue
{
public:
	HmValue(HmChannel* channel, const std::string& valueId );
	~HmValue(void);
	void HandleXmlRpcEvent( XmlRpc::XmlRpcValue& value );
	bool SetReceiver( const std::string& channelName );
	bool SetSender( const std::string& channelName );
	void ReceivingSetValue( unsigned long value );
	bool TriggerValueGet();
	HmChannel* GetChannel();
	const std::string& GetId();
	bool ListConnections( XmlRpc::XmlRpcValue& list );
	bool ReadParametersFromXml( XMLNode& node, XmlLoop* loop );
private:
	HmChannel* _channel;
	std::string _valueId;
	double _floatScale;
	bool _boolInvert;
	IcChannel* _sendingChannel;
	HmValueConnection* _receivingConnection;
};
