#pragma once
#include "Connection.h"

class HmValue;

class HmValueConnection : public Connection
{
public:
	HmValueConnection(HmValue* value, const std::string& channelName);
	~HmValueConnection(void);
	void ReceivingSetValue( IcChannel* channel, unsigned long value );
private:
	HmValue* _value;
};
