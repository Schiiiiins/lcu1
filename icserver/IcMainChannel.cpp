#include "IcMainChannel.h"
#include "IcSubChannel.h"
#include "Logger.h"
#include "HmValue.h"

IcMainChannel::IcMainChannel(const std::string& name, int index)
:IcChannel(name, index)
{
	_value = 0;
	_valueUndefinedBits = 0xffffffff;
}

IcMainChannel::~IcMainChannel(void)
{
}

void IcMainChannel::SetValue( unsigned long value, unsigned long bitmask )
{
	LOG( Logger::LOG_DEBUG, "%s set %lu/%lu old value=%lu, undefined bits=0x%lX", GetAlias().c_str(), value, bitmask, _value, _valueUndefinedBits);
	if( ((value & bitmask) == (_value & bitmask)) && !(_valueUndefinedBits & bitmask) )return;
	unsigned long newValue = (_value & ~bitmask) | (value & bitmask);
	unsigned long changedBits = ((value ^ _value) | _valueUndefinedBits) & bitmask;
	SendValueChangeToReceivers( newValue, changedBits );
	for( t_subChannels::iterator it=_subChannels.begin(); it!=_subChannels.end(); it++ )
	{
		(*it)->OnMainSetValue( newValue, changedBits);
	}
	_value = newValue;
	_valueUndefinedBits &= ~bitmask;
	if( !_valueUndefinedBits && changedBits )PersistValue();
}

void IcMainChannel::SetValue( unsigned long value )
{
	SetValue( value, 0xffffffff );
}

unsigned long IcMainChannel::GetValue()
{
	if( !_valueUndefinedBits )
	{
		return _value;
	}
	EnsureValueValid( 0xffffffff );
	return _value;
}

unsigned long IcMainChannel::GetValue( unsigned long bitmask )
{
	if( ! (_valueUndefinedBits & bitmask))
	{
		return _value;
	}
	EnsureValueValid( bitmask );
	return _value;
}

void IcMainChannel::AddSubChannel( IcSubChannel* subChannel )
{
	_subChannels.push_back( subChannel );
}

void IcMainChannel::RemoveSubChannel( IcSubChannel* subChannel )
{
	for( t_subChannels::iterator it=_subChannels.begin(); it!=_subChannels.end(); it++ )
	{
		if( *it == subChannel )
		{
			_subChannels.erase( it );
			return;
		}
	}
}

bool IcMainChannel::IsUndefined( unsigned long bitmask )
{
	return (_valueUndefinedBits & bitmask) != 0;
}

unsigned long IcMainChannel::GetUndefinedBits()
{
	return _valueUndefinedBits;
}

bool IcMainChannel::EnsureValueValid( unsigned long bitmask )
{
	if( _sourceHmValue )
	{
		_sourceHmValue->TriggerValueGet();
	}else{
		for( t_subChannels::iterator it=_subChannels.begin(); it!=_subChannels.end(); it++ )
		{
			(*it)->EnsureValueValid( bitmask );
		}
	}
	return IsUndefined( bitmask );
}

void IcMainChannel::MarkAsUndefined()
{
	_valueUndefinedBits = 0xffffffff;
}

void IcMainChannel::MarkAsUndefined( unsigned long bitmask )
{
	_valueUndefinedBits |= bitmask;
}
