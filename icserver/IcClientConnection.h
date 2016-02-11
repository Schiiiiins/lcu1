#pragma once
#include "Connection.h"

#include <XmlRpc.h>

class IcClientConnection : public XmlRpc::XmlRpcSource, public Connection
{
public:
	IcClientConnection(int socket);
	~IcClientConnection(void);
    //! Return true to continue monitoring this source
    unsigned handleEvent(unsigned eventType);

	//! Return mask of events to monitor in addition to what handleEvent returned last time
	unsigned getExtraMask() const;
	void ReceivingSetValue( IcChannel* channel, unsigned long value );
	const std::string& GetName();
private:
	bool HandleIncomingData();
	bool SendOutgoingData();
	void SendFromBuffer(std::string& buffer, std::string::size_type* offset);
	bool ProcessIncomingMessage( const std::string& msg );
	void CmdRegisterClient( const std::string& name );
	void CmdUnregisterClient( const std::string& name );
	std::string _eventBuffer;
	std::string _responseBuffer;
	std::string _readBuffer;
	std::string::size_type _readBufferLength;
	std::string::size_type _eventBufferBytesSent;
	std::string::size_type _responseBufferBytesSent;
	std::string _name;
	typedef enum{
		ResponseBuffer,
		EventBuffer
	}Buffer;
	Buffer _currentBuffer;
};
