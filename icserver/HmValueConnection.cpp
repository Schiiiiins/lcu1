#include "HmValueConnection.h"
#include "HmValue.h"
#include "IcChannel.h"

HmValueConnection::HmValueConnection(HmValue* value, const std::string& channelName)
{
	_value = value;
	this->CmdRegisterReceiver( channelName );
}

HmValueConnection::~HmValueConnection(void)
{
}

void HmValueConnection::ReceivingSetValue( IcChannel* channel, unsigned long value )
{
	_value->ReceivingSetValue( value );
}
