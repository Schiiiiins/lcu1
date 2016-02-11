#pragma once

#include <list>
#include "Connection.h"

class HmValue;

class IcChannel
{
public:
	IcChannel(const std::string& name, int index);
	~IcChannel(void);
	int GetIndex();
	void AddReceiver( Connection* conn, int bitmask );
	void RemoveReceiver( Connection* conn, int bitmask );
	virtual void SetValue( unsigned long value )=0;
	virtual unsigned long GetValue()=0;
	virtual void SetValue( unsigned long value, unsigned long bitmask )=0;
	virtual unsigned long GetUndefinedBits()=0;
	const std::string& GetName();
	const std::string& GetAlias();
	void SetAlias( const std::string& alias );
	void SetIsPersistent( bool isPersistent );
	bool IsPersistent();
	void PersistValue();
	void SetSourceHmValue( HmValue* hmValue );
	virtual void MarkAsUndefined() = 0;
	void SetFlags(const std::string& flags);
	void SetDisplayName(const std::string& displayName);
	const std::string& GetFlags();
	const std::string& GetDisplayName();
protected:
	void SendValueChangeToReceivers( unsigned long value, unsigned long changedBits );
	HmValue* _sourceHmValue;
private:
	struct IcReceiver{
		Connection* connection;
		int bitmask;
	};
	int _index;
	typedef std::list<IcReceiver> t_listReceivers;
	t_listReceivers _listReceivers;
	std::string _name;
	std::string _alias;
	std::string _flags;
	std::string _displayName;
	bool _isPersistent;
};
