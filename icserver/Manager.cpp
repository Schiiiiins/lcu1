#if defined(_WIN32)
# include <stdio.h>
# include <winsock2.h>
typedef int socklen_t;

# define EINPROGRESS	WSAEINPROGRESS
# define EWOULDBLOCK	WSAEWOULDBLOCK
# define ETIMEDOUT	    WSAETIMEDOUT
#else
extern "C" {
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <netinet/in.h>
# include <netdb.h>
# include <errno.h>
# include <fcntl.h>
}
#endif  // _WIN32

#include "Manager.h"
#include "HmManager.h"
#include "HmValue.h"
#include "HmChannel.h"
#include "Socket.h"
#include "IcClientConnection.h"
#include "IcMainChannel.h"
#include "IcSubChannel.h"
#include "XmlRpcMethods.h"
#include <XmlRpc.h>
#include <Logger.h>
#include <xmlParser.h>
#include <PropertyMap.h>
#include "XmlLoop.h"

using namespace XmlRpc;

static const unsigned short PORT = 8778;
static const unsigned int BACKLOG = 10;
static const unsigned short LISTEN_PORT_XML = 2005;

Manager* Manager::s_singleton = NULL;

Manager* Manager::GetSingleton()
{
	if( !s_singleton )s_singleton = new Manager();
	return s_singleton;
}


Manager::Manager(void)
{
	_nextChannelIndex = 1;
	if( !s_singleton )s_singleton = this;
	//_persistValuesInhibit = false;
	_valuesSaveRequired = false;
}

bool Manager::InitServers()
{

	// Create the server socket on the specified port
	_xmlRpcServer.bindAndListen(LISTEN_PORT_XML,100);

	_xmlRpcServer.enableIntrospection(true);

	InitXmlRpcMethods();

	//create the iC TCP server socket
	int socket = Socket::socket();
	if( socket < 0 ){
		LOG(Logger::LOG_ERROR, "Could not create server socket\n");
		return false;
	}
	Socket::setNonBlocking( socket );
	Socket::setReuseAddr( socket );
	if( !Socket::bind( socket, PORT ) )
	{
		LOG(Logger::LOG_ERROR, "Could not bind server socket\n");
		return false;
	}
	if( !Socket::listen( socket, BACKLOG ) )
	{
		LOG(Logger::LOG_ERROR, "Could not listen on server socket\n");
		return false;
	}

	setfd( socket );

	_xmlRpcServer.getDispatcher()->addSource(this, XmlRpcDispatch::ReadableEvent);
	return true;
}


Manager::~Manager(void)
{
	if( s_singleton == this )s_singleton = NULL;
}

unsigned Manager::handleEvent( unsigned eventType )
{
	int clientSocket = Socket::accept( getfd() );
	if( clientSocket<0 ){
		LOG(Logger::LOG_ERROR, "Error accepting connection\n");
		return XmlRpcDispatch::ReadableEvent;
	}
	IcClientConnection* newConnection = new IcClientConnection( clientSocket );
	_xmlRpcServer.getDispatcher()->addSource(newConnection, XmlRpcDispatch::ReadableEvent);
	return XmlRpcDispatch::ReadableEvent;
}

void Manager::Run()
{

	while(true)
	{
		_xmlRpcServer.getDispatcher()->work( 1000 );
		if( _valuesSaveRequired )
		{
			DoSavePersistentValues();
			_valuesSaveRequired = false;
		}
	}
}

void Manager::RemoveConnection( IcClientConnection* conn )
{
	_xmlRpcServer.getDispatcher()->removeSource(conn);
	delete conn;
}

IcChannel* Manager::GetChannel( const std::string& name )
{
	t_mapChannelsByName::iterator it = _mapChannelsByName.find( name );
	if( it==_mapChannelsByName.end() )return NULL;
	return it->second;
}

IcChannel* Manager::AddChannel( const std::string& name )
{
	IcChannel* channel = _mapChannelsByName[name];
	if( !channel )
	{
		std::string::size_type dotpos = name.find('.');
		if( dotpos == std::string::npos )
		{
			channel = new IcMainChannel( name, _nextChannelIndex++ );
		}else{
			std::string mainChannelName = name.substr(0, dotpos);
			IcMainChannel* mainChannel = dynamic_cast<IcMainChannel*>(AddChannel( mainChannelName ));
			int bitIndex = atoi(name.c_str() + dotpos + 1);
			channel = new IcSubChannel( mainChannel, bitIndex, name, _nextChannelIndex++ );
		}
		_mapChannelsByName[name] = channel;
	}
	return channel;
}

bool Manager::AddChannelAlias( const std::string& name, const std::string& alias )
{
	IcChannel* channel = GetChannel( name );
	if( !channel ) return false;
	IcChannel* aliasChannel = GetChannel( alias );
	if( aliasChannel ) return false;
	_mapChannelsByName[alias] = channel;
	channel->SetAlias( alias );
	return true;
}

XmlRpc::XmlRpcServer* Manager::GetXmlRpcServer()
{
	return &_xmlRpcServer;
}

bool Manager::LoadSystemConfig( const std::string& configFilename, const std::string& mappingFilename )
{
	XMLResults xmlResult;
	XMLNode configRootNode = XMLNode::parseFile( configFilename.c_str(), "Root", &xmlResult );
	if( xmlResult.error )
	{
		LOG( Logger::LOG_ERROR, "Error %d reading system config file %s (%d:%d)", xmlResult.error, configFilename.c_str(), xmlResult.nLine, xmlResult.nColumn);
		return false;
	}

	XMLNode mappingRootNode = XMLNode::parseFile( mappingFilename.c_str(), "Root", &xmlResult );
	if( xmlResult.error )
	{
		LOG( Logger::LOG_WARNING, "Error %d reading system mapping file %s (%d:%d)", xmlResult.error, mappingFilename.c_str(), xmlResult.nLine, xmlResult.nColumn);
	}

	HmManager* hmManager = HmManager::GetSingleton();

	if( !ReadValuesFromXml( configRootNode ) )return false;
	if( !hmManager->ReadInterfacesFromXml( configRootNode, true ) )return false;
	if( !hmManager->ReadDevicesFromXml( configRootNode ) )return false;
	if( !mappingRootNode.isEmpty() )
	{
		if( !hmManager->ReadDevicesFromXml( mappingRootNode ) )return false;
	}
	if( !hmManager->ReadConnectionParametersFromXml( configRootNode, true ) )return false;
	return true;
}

bool Manager::LoadPersistentValues( const std::string& filename )
{
	_persistedValuesFile = filename;
	PropertyMap valuesMap;
	if( !valuesMap.ReadFromFile( filename ) )return false;
	//bool persistValuesInhibitSave = _persistValuesInhibit;
	//_persistValuesInhibit = true;

	bool valuesSaveWasRequired = _valuesSaveRequired;

	PropertyMap::Section values = valuesMap.GetSection(valuesMap.GetCurrentSection());
	for( PropertyMap::Section::iterator it=values.begin(); it!=values.end(); it++)
	{
		IcChannel* channel = this->GetChannel( it->first );
		if( channel && channel->IsPersistent() )
		{
			unsigned long value = strtoul( it->second.c_str(), NULL, 0 );
			channel->SetValue( value );
		}else{
			LOG(Logger::LOG_WARNING, "Persisted value found for unknown or not persistent channel %s", it->first.c_str());
		}
	}
	_valuesSaveRequired = valuesSaveWasRequired;
	//_persistValuesInhibit = persistValuesInhibitSave;
	return true;
}

bool Manager::DoSavePersistentValues( )
{
	if( _persistedValuesFile.empty() )return false;
	//if( _persistValuesInhibit )return false;
	LOG( Logger::LOG_DEBUG, "Saving persistent values");
	PropertyMap valuesMap;
	for( t_mapChannelsByName::iterator it = _mapChannelsByName.begin(); it!=_mapChannelsByName.end(); it++ )
	{
		IcChannel* channel = it->second;
		if( it->first == channel->GetAlias() )
		{
			if( channel->IsPersistent() )
			{
				char buffer[32];
				sprintf( buffer, "%lu", channel->GetValue() );
				valuesMap.SetStringValue( it->first, buffer );
				LOG(Logger::LOG_DEBUG, "Save %s = %s", it->first.c_str(), buffer);
			}
		}
	}
	std::string new_filename=_persistedValuesFile+".new";
	std::string bak_filename=_persistedValuesFile+".bak";

	if( !valuesMap.WriteToFile( new_filename ) )return false;

	unlink(bak_filename.c_str());

	rename(_persistedValuesFile.c_str(), bak_filename.c_str());
	if(rename(new_filename.c_str(), _persistedValuesFile.c_str()))return false;
	unlink(bak_filename.c_str());
	return true;
}

void Manager::SavePersistentValues( )
{
	_valuesSaveRequired = true;
}

bool Manager::ReadValueFromXml( XMLNode& valueNode, XmlLoop* loop )
{
	const char* temp = valueNode.getAttribute("name");
	if( !temp )
	{
		LOG( Logger::LOG_ERROR, "<value> is missing \"name\" attribute");
		return false;
	}
	std::string channelName = temp;
	if( loop )
	{
		channelName = loop->ResolveIdentifier( channelName );
	}

	if( GetChannel( channelName ) )
	{
		LOG( Logger::LOG_ERROR, "Duplicate value, name=\"%s\"", channelName.c_str());
		return false;
	}
	IcChannel* channel = AddChannel( channelName );

	temp = valueNode.getAttribute("alias");
	if( temp )
	{
		std::string alias = temp;
		if( loop )
		{
			alias = loop->ResolveIdentifier( alias );
		}
		if( !AddChannelAlias( channelName, alias ) )
		{
			LOG( Logger::LOG_ERROR, "Could not add alias \"%s\" for value \"%s\"", alias.c_str(), channelName.c_str());
			return false;
		}
	}

	temp = valueNode.getAttribute("persistent");
	if( temp && temp[0] == 't' )
	{
		channel->SetIsPersistent( true );
	}

	temp = valueNode.getAttribute("flags");
	if( temp )
	{
		channel->SetFlags( temp );
	}

	temp = valueNode.getAttribute("display_name");

	if( temp )
	{
		std::string displayName = temp;
		if( loop )
		{
			displayName = loop->ResolveIdentifier( displayName );
		}
		channel->SetDisplayName( displayName );
	}

	LOG(Logger::LOG_DEBUG, "New channel %s(%s)", channel->GetName().c_str(), channel->GetAlias().c_str());

	temp = valueNode.getAttribute("default_value");
	if( temp )
	{
		unsigned long default_value = strtoul(temp, NULL, 0);
		channel->SetValue(default_value);
		LOG(Logger::LOG_DEBUG, "%s default is %lu", channel->GetAlias().c_str(), default_value);
	}
	return true;
}

bool Manager::ReadValuesFromXml( XMLNode& rootNode )
{
	//bool persistValuesInhibitSave = _persistValuesInhibit;
	//_persistValuesInhibit = true;

	bool valuesSaveWasRequired = _valuesSaveRequired;
	XMLNode valuesNode = rootNode.getChildNode("values");
	int i = 0;
	XMLNode valueNode = valuesNode.getChildNode("value", &i);
	while( !valueNode.isEmpty() )
	{
		if( !ReadValueFromXml( valueNode, NULL ) )
		{
			//_persistValuesInhibit = persistValuesInhibitSave;
			_valuesSaveRequired = valuesSaveWasRequired;
			return false;
		}
		valueNode = valuesNode.getChildNode("value", &i);
	}

	i=0;
	XMLNode loopNode = valuesNode.getChildNode("loop", &i);
	while( !loopNode.isEmpty() )
	{
		XmlLoop loop;
		if( !loop.InitFromXml(loopNode) )
		{
			//_persistValuesInhibit = persistValuesInhibitSave;
			_valuesSaveRequired = valuesSaveWasRequired;
			return false;
		}
		int j=0;
		XMLNode valueNode = loopNode.getChildNode("value", &j);
		while( !valueNode.isEmpty() )
		{
			loop.Reset();
			do{
				if(!ReadValueFromXml( valueNode, &loop ))
				{
					//_persistValuesInhibit = persistValuesInhibitSave;
					_valuesSaveRequired = valuesSaveWasRequired;
					return false;
				}
			}while( loop.Iterate() );
			valueNode = loopNode.getChildNode("value", &j);
		}
		loopNode = valuesNode.getChildNode("loop", &i);
	}

	//_persistValuesInhibit = persistValuesInhibitSave;
	_valuesSaveRequired = valuesSaveWasRequired;
	return true;
}

bool Manager::GetValue( const std::string& channelName, unsigned long* result )
{
	IcChannel* ch = GetChannel(channelName);
	if( !ch )return false;

	*result = ch->GetValue();
	return true;
}

bool Manager::SetValue( const std::string& channelName, unsigned long value )
{
	IcChannel* ch = GetChannel(channelName);
	if( !ch )return false;

	ch->SetValue( value );
	return true;
}
