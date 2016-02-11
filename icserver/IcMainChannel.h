#pragma once
#include "IcChannel.h"
#include <vector>

class IcSubChannel;

class IcMainChannel :
	public IcChannel
{
public:
	IcMainChannel(const std::string& name, int index);
	~IcMainChannel(void);
	void SetValue( unsigned long value );
	void SetValue( unsigned long value, unsigned long bitmask );
	unsigned long GetValue();
	unsigned long GetValue( unsigned long bitmask );
	void AddSubChannel( IcSubChannel* subChannel );
	void RemoveSubChannel( IcSubChannel* subChannel );
	bool IsUndefined( unsigned long bitmask );
	unsigned long GetUndefinedBits();
	void MarkAsUndefined();
	void MarkAsUndefined( unsigned long bitmask );
private:
	bool EnsureValueValid( unsigned long bitmask );
	unsigned long _value;
	unsigned long _valueUndefinedBits;
	typedef std::vector<IcSubChannel*> t_subChannels;
	t_subChannels _subChannels;
};

