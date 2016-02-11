#pragma once

#include <string>
#include <map>
#include <vector>

class Manager;
class IcChannel;

class Connection
{
public:
	Connection( );
	~Connection(void);
	virtual void ReceivingSetValue( IcChannel* channel, unsigned long value )=0;
	std::vector<IcChannel*> GetReceivingChannels();
protected:
	IcChannel* CmdRegisterSender( const std::string& name );
	void CmdUnregisterSender( const std::string& name );
	IcChannel* CmdRegisterReceiver( const std::string& name );
	void CmdUnregisterReceiver( const std::string& name );
	void SendingSetValue( unsigned long channelIndex, unsigned long data );
	void GetNameAndBitmaskFromChannelSpec( const std::string& channelSpec, std::string* name, unsigned long* bitmask );
	typedef std::map<std::string, IcChannel*> t_mapChannelsByName;
	typedef std::map<int, IcChannel*> t_mapChannelsByIndex;
	t_mapChannelsByName _mapSendingChannelsByName;
	t_mapChannelsByIndex _mapSendingChannelsByIndex;
	t_mapChannelsByName _mapReceivingChannelsByName;
};
