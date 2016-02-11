#include "AsyncXmlRpcSender.h"
#include <pthread.h>
#include <Logger.h>
using namespace XmlRpc;

AsyncXmlRpcSender::AsyncXmlRpcSender(const std::string& url):c(url)
{
}

AsyncXmlRpcSender::~AsyncXmlRpcSender(void)
{
}

void AsyncXmlRpcSender::AsyncCall(const std::string& method, XmlRpc::XmlRpcValue& params)
{
	this->method=method;
	this->params=params;
	pthread_t new_thread;
	pthread_create(&new_thread, NULL, _ThreadFunction, reinterpret_cast<void*>(this));
}

void* AsyncXmlRpcSender::_ThreadFunction(void* arg)
{
	AsyncXmlRpcSender* This=reinterpret_cast<AsyncXmlRpcSender*>(arg);
	pthread_detach(pthread_self());
	return This->ThreadFunction();
}

void* AsyncXmlRpcSender::ThreadFunction()
{
	XmlRpcValue result;
	if(!c.execute(method.c_str(), params, result)){
		LOG( Logger::LOG_WARNING, "XmlRpc transport error calling %s(%s) on %s:", method.c_str(), params.toText().c_str(), c.getURL().c_str());
	}else{
		if(c.isFault()){
			LOG( Logger::LOG_WARNING, "XmlRpc fault %s calling %s(%s) on %s:", result.toText().c_str(), method.c_str(), params.toText().c_str(), c.getURL().c_str());
		}
	}
	delete this;
	return 0;
}
