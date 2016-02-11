#ifndef _ASYNCXMLRPCSENDER_H_
#define _ASYNCXMLRPCSENDER_H_

#include <XmlRpc.h>
#include <string>

class AsyncXmlRpcSender
{
public:
	AsyncXmlRpcSender(const std::string& url);
	~AsyncXmlRpcSender(void);
	void AsyncCall(const std::string& method, XmlRpc::XmlRpcValue& params);
protected:
	static void* _ThreadFunction(void* arg);
	void* ThreadFunction();
	XmlRpc::XmlRpcClient c;
	std::string method;
	XmlRpc::XmlRpcValue params;
};

#endif
