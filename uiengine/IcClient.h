#pragma once

#include <string>
#include <map>
#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Client class for interfacing with an immediateC hub (icserver).  </summary>
///
/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class IcClient
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="host">	The host the hub is running on. </param>
	/// <param name="port">	The TCP port the hub is listening on. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	IcClient(const std::string& host, int port);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Finaliser. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	~IcClient(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets an immediateC value referenced by its id. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="id">	The identifier. </param>
	///
	/// <returns>	The value. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned long GetValue(const std::string& id);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets an immediateC value. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="id">   	The identifier. </param>
	/// <param name="value">	The value. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool SetValue(const std::string& id, unsigned long value);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Called in a loop to get the value updates received from the hub. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="id">   	[out] The identifier. </param>
	/// <param name="value">	[out] The value. </param>
	///
	/// <returns>	true if the parameters contain information on the next value, 
	/// 			false if all updates have been read. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool GetNextReceivedValue( std::string* id, unsigned long* value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Should be called if the file descriptor for the TCP connection to the hub becomes 
	/// 			readable. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void DoPendingWork();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the file descriptor for the TCP connection to the hub. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	The fd. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int GetFd();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if this object is connected. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	true if connected, false if not. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool IsConnected();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Registers a value channel with the hub. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="id">	 	The value channel's identifier. </param>
	/// <param name="sender">	true if we want to send values to the channel, false if we want to receive 
	/// 						updates for the value. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool RegisterChannel( const std::string& id, bool sender );
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Several timeout constants in ms.  </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum{
		ValueGetTimeout = 5000,
		SendTimeout = 5000,
		ReceiveTimeout = 8000
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constants for value flags. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum{
		/// <summary> We are sending values for the channel.  </summary>
		ValueFlagSender = 1,
		/// <summary> We are informed on value updates for the channel.  </summary>
		ValueFlagReceiver = 2,
		/// <summary> We have received at least one value for the channel.  </summary>
		ValueFlagReceived = 4
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Flag constants. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum{
		/// <summary> Wait for a file descriptor to become readable.  </summary>
		WaitFlagReadable = 1,
		/// <summary> Wait for a file descriptor to become writable.  </summary>
		WaitFlagWritable = 2
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Information about a immediateC value. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	class ValueInfo
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Default constructor. </summary>
		///
		/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
		////////////////////////////////////////////////////////////////////////////////////////////////////

		ValueInfo()
		{
			numId = -1;
			value = 0;
			flags = 0;
		}
		/// <summary> Numerical identifier used in TCP communication </summary>
		int numId;
		/// <summary> Current value </summary>
		unsigned long value;
		/// <summary> The flags: sender, receiver, received </summary>
		int flags;
		/// <summary> Textual identifier for this value </summary>
		std::string strId;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Connects this client to the hub. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Connect();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Disconnects this client. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Disconnect();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Wait for reception of a channel list message in the process of registering a new 
	/// 			channel. When registering channels with their textual ids, the hub sends back a 
	/// 			list of associated numerical identifers.</summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="channelList">	[out] List of channel identifiers as received by the hub. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool ReceiveChannelListMessage(std::string* channelList);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Process the value message described by msg. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="msg">	The message received from the hub. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool ProcessValueMessage( const std::string& msg );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Receive next message synchronously with a timeout. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="msg">	  	[out] The message. </param>
	/// <param name="timeout">	The timeout in ms. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool ReceiveNextMessage( std::string* msg, int timeout );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sends a message to the hub. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="msg">	The message to send. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool SendMessage( const std::string& msg );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Waits for the TCP socket to become readable or writable. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="flags">  	The flags indicating what to wait for.
	/// 						Combination of <see cref="WaitFlagReadable"/> and <see cref="WaitFlagWritable"/>. </param>
	/// <param name="timeout">	The timeout in ms. </param>
	///
	/// <returns>	A combination of <see cref="WaitFlagReadable"/> and <see cref="WaitFlagWritable"/>. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int Wait( int flags, int timeout );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the info structure for a value. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="id">	The textual identifier of the value. </param>
	///
	/// <returns>	null if it fails, else the value information. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	ValueInfo* GetValueInfo( const std::string& id );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the info structure for a value. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="id">	The numerical identifier of the value. </param>
	///
	/// <returns>	null if it fails, else the value information. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	ValueInfo* GetValueInfo( int id );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Typedef for mapping textual ids to values. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::map<std::string, ValueInfo*> t_mapValues;
	/// <summary> Map for looking up values by their textual ids </summary>
	t_mapValues _mapValues;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Typedef for mapping numerical ids to values. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::map<int, ValueInfo*> t_mapValuesByNumId;
	/// <summary> Map for looking up values by their numerical ids </summary>
	t_mapValuesByNumId _mapValuesByNumId;
	/// <summary> Set of recently received values </summary>
	std::set<ValueInfo*> _setReceivedValues;
	/// <summary> The socket fd for the connection to the hub</summary>
	int _fd;
	/// <summary> Buffer for receive data </summary>
	std::string _receiveBuffer;
	/// <summary> The remote host </summary>
	std::string _host;
	/// <summary> The remote port </summary>
	int _port;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of IcClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////

