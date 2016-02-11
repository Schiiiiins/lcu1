#include "IcSubChannel.h"
#include "IcMainChannel.h"
#include "HmValue.h"
#include <Logger.h>

IcSubChannel::IcSubChannel(IcMainChannel* mainChannel, int bitIndex, const std::string& name, int index)
:IcChannel(name, index)
{
	_mainChannel = mainChannel;
	_bitIndex = bitIndex;
	_mainChannel->AddSubChannel( this );
}

IcSubChannel::~IcSubChannel(void)
{
	_mainChannel->RemoveSubChannel( this );
}

void IcSubChannel::SetValue( unsigned long value )
{
	LOG(Logger::LOG_DEBUG, "%s=%lu, _bitIndex=%d", GetName().c_str(), value, _bitIndex);
	_mainChannel->SetValue( (value & 0x01) << _bitIndex, (unsigned long)(1<<_bitIndex) );
}

void IcSubChannel::SetValue( unsigned long value, unsigned long bitmask )
{
	if( bitmask & 0x01 )
	{
		SetValue(value);
	}
}

unsigned long IcSubChannel::GetValue()
{
	return (_mainChannel->GetValue( 1<<_bitIndex ) >> _bitIndex) & 0x01;
}

void IcSubChannel::OnMainSetValue( unsigned long value, unsigned long changedBits )
{
	SendValueChangeToReceivers( (value>>_bitIndex)&0x01, (changedBits>>_bitIndex)&0x01 );
	if( changedBits & (1<<_bitIndex) )PersistValue();
}

bool IcSubChannel::EnsureValueValid( unsigned long bitmask )
{
	if( !_mainChannel->IsUndefined( (1<<_bitIndex) & bitmask ) )return true;
	if( _sourceHmValue )
	{
		_sourceHmValue->TriggerValueGet();
	}
	return !_mainChannel->IsUndefined( 1<<_bitIndex );
}

void IcSubChannel::MarkAsUndefined()
{
	_mainChannel->MarkAsUndefined( 1<<_bitIndex );
}

unsigned long IcSubChannel::GetUndefinedBits()
{
	return (_mainChannel->GetUndefinedBits() >> _bitIndex) & 0x01;
}