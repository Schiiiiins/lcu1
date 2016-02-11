#pragma once
#include "IcChannel.h"

class IcMainChannel;

class IcSubChannel :
	public IcChannel
{
public:
	IcSubChannel(IcMainChannel* mainChannel, int bitIndex, const std::string& name, int index);
	~IcSubChannel(void);
	void SetValue( unsigned long value );
	void SetValue( unsigned long value, unsigned long bitmask );
	unsigned long GetValue();
	void OnMainSetValue( unsigned long value, unsigned long changedBits );
	bool EnsureValueValid( unsigned long bitmask );
	void MarkAsUndefined();
	unsigned long GetUndefinedBits();
private:
	IcMainChannel* _mainChannel;
	int _bitIndex;
};
