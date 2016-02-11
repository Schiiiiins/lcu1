#include "IcChannel.h"
#include <cstdlib>
#include <Logger.h>
#include "Manager.h"
#include "HmValue.h"

IcChannel::IcChannel(const std::string& name, int index)
{
	_index = index;
	_name = name;
	_alias = name;
	_isPersistent = false;
	_sourceHmValue = NULL;
}

IcChannel::~IcChannel(void)
{
}

int IcChannel::GetIndex()
{
	return _index;
}

void IcChannel::AddReceiver( Connection* conn, int bitmask )
{
	IcReceiver receiver;
	receiver.connection = conn;
	receiver.bitmask = bitmask;
	_listReceivers.push_back( receiver );
}

void IcChannel::RemoveReceiver( Connection* conn, int bitmask )
{
	t_listReceivers::iterator it;
	for( it=_listReceivers.begin(); it!=_listReceivers.end(); it++ )
	{
		if( it->connection == conn )
		{
			if( it->bitmask & bitmask )
			{
				it = _listReceivers.erase( it );
			}
		}
	}
}

const std::string& IcChannel::GetName()
{
	return _name;
}

const std::string& IcChannel::GetAlias()
{
	return _alias;
}

void IcChannel::SetAlias(const std::string& alias)
{
	_alias = alias;
}

void IcChannel::SendValueChangeToReceivers( unsigned long value, unsigned long changedBits )
{
	LOG(Logger::LOG_DEBUG, "%s(%s):SendValueChangeToReceivers(%lu, %lu)", _name.c_str(), _alias.c_str(), value, changedBits);
	for( t_listReceivers::iterator it=_listReceivers.begin(); it!=_listReceivers.end(); it++ )
	{
		if( it->bitmask & ( changedBits ) )it->connection->ReceivingSetValue( this, value );
	}
}

void IcChannel::PersistValue()
{
	if( _isPersistent )
	{
		Manager::GetSingleton()->SavePersistentValues();
	}
}

void IcChannel::SetIsPersistent( bool isPersistent )
{
	_isPersistent = isPersistent;
}

bool IcChannel::IsPersistent()
{
	return _isPersistent;
}

void IcChannel::SetSourceHmValue( HmValue* hmValue )
{
	_sourceHmValue = hmValue;
}

void IcChannel::SetFlags(const std::string& flags)
{
	_flags = flags;
}

void IcChannel::SetDisplayName(const std::string& displayName)
{
	_displayName = displayName;
}

const std::string& IcChannel::GetFlags()
{
	return _flags;
}

const std::string& IcChannel::GetDisplayName()
{
	return _displayName;
}
