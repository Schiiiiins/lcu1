#include "Manager.h"
#include "HmManager.h"
#include "XmlRpcMethods.h"

using namespace XmlRpc;

//#define LOG_METHOD_CALLS

#ifdef LOG_METHOD_CALLS
class XmlRpcDebugMethod : public XmlRpcServerMethod  
{
public:
	XmlRpcDebugMethod(const char* name, XmlRpcServer* s):XmlRpcServerMethod(name, s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		LOG(Logger::LOG_DEBUG, "XmlRpcMethod %s(%s)", _name.c_str(), params.toText().c_str());
		do_execute(params, result);
		LOG(Logger::LOG_DEBUG, "XmlRpcMethod %s(%s)returns %s", _name.c_str(), params.toText().c_str(), result.toText().c_str());
	}
	virtual void do_execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)=0;
};
#define XmlRpcServerMethod XmlRpcDebugMethod
#define execute(x,y) do_execute(x,y)
#endif

class XmlRpcMethodListDevices : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodListDevices(XmlRpcServer* s):XmlRpcServerMethod("listDevices", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
	}
};

class XmlRpcMethodEvent : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodEvent(XmlRpcServer* s):XmlRpcServerMethod("event", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		params[3];
		HmManager::GetSingleton()->HandleXmlRpcEvent( params[0], params[1], params[2], params[3] );	
	}
};

class XmlRpcMethodGetValues : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodGetValues(XmlRpcServer* s):XmlRpcServerMethod("getValues", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		if( params.size() )
		{
			result[params.size()-1];
			for( int i = 0; i<params.size(); i++)
			{
				unsigned long value;
				if( Manager::GetSingleton()->GetValue( params[i], &value ) )
				{
					(int&)result[i] = value;
				}
			}
		}
	}
};

class XmlRpcMethodGetValue : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodGetValue(XmlRpcServer* s):XmlRpcServerMethod("getValue", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		unsigned long value;
		if( Manager::GetSingleton()->GetValue( params[0], &value ) )
		{
			(int&)result = value;
		}
	}
};

class XmlRpcMethodSetValue : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodSetValue(XmlRpcServer* s):XmlRpcServerMethod("setValue", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		params[1];
		Manager::GetSingleton()->SetValue( params[0], (int&)params[1] );
	}
};

class XmlRpcMethodReloadUserConfig : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodReloadUserConfig(XmlRpcServer* s):XmlRpcServerMethod("reloadUserConfig", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		(bool&)result = HmManager::GetSingleton()->ReloadUserConfig();
	}
};

class XmlRpcMethodGetInterfaces : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodGetInterfaces(XmlRpcServer* s):XmlRpcServerMethod("getInterfaces", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		if( !HmManager::GetSingleton()->GetInterfaces( &result ) )
		{
			throw XmlRpcException("Failure");
		}
	}
};

class XmlRpcMethodGetDevices : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodGetDevices(XmlRpcServer* s):XmlRpcServerMethod("getDevices", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		if( !HmManager::GetSingleton()->GetDevices( &result ) )
		{
			throw XmlRpcException("Failure");
		}
	}
};

class XmlRpcMethodMapDevice : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodMapDevice(XmlRpcServer* s):XmlRpcServerMethod("mapDevice", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		if( params.size() != 3 )
		{
			throw XmlRpcException("3 parameters expected");
		}
		if( !HmManager::GetSingleton()->MapDevice( params[0], params[1], params[2] ) )
		{
			throw XmlRpcException("Failure");
		}
	}
};

class XmlRpcMethodUnmapDevice : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodUnmapDevice(XmlRpcServer* s):XmlRpcServerMethod("unmapDevice", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		if( params.size() != 1 )
		{
			throw XmlRpcException("1 parameters expected");
		}
		if( !HmManager::GetSingleton()->UnmapDevice( params[0] ) )
		{
			throw XmlRpcException("Failure");
		}
	}
};

class XmlRpcMethodListValueConnections : public XmlRpcServerMethod  
{
public:
	XmlRpcMethodListValueConnections(XmlRpcServer* s):XmlRpcServerMethod("listValueConnections", s){};
	void execute(XmlRpc::XmlRpcValue& params, XmlRpc::XmlRpcValue& result)
	{
		result.assertArray(0);
		if( !HmManager::GetSingleton()->ListValueConnections( result ) )
		{
			throw XmlRpcException("Failure");
		}
	}
};

void InitXmlRpcMethods()
{
	XmlRpcServer* s = Manager::GetSingleton()->GetXmlRpcServer();
	static XmlRpcMethodListDevices xmlRpcMethodListDevices(s);
	static XmlRpcMethodEvent xmlRpcMethodEventDevices(s);
	static XmlRpcMethodGetValues xmlRpcMethodGetValues(s);
	static XmlRpcMethodGetValue xmlRpcMethodGetValue(s);
	static XmlRpcMethodSetValue xmlRpcMethodSetValue(s);
	static XmlRpcMethodReloadUserConfig xmlRpcMethodReloadUserConfig(s);
	static XmlRpcMethodGetInterfaces xmlRpcMethodGetInterfaces(s);
	static XmlRpcMethodGetDevices xmlRpcMethodGetDevices(s);
	static XmlRpcMethodMapDevice xmlRpcMethodMapDevice(s);
	static XmlRpcMethodUnmapDevice xmlRpcMethodUnmapDevice(s);
	static XmlRpcMethodListValueConnections xmlRpcMethodListValueConnections(s);
}
