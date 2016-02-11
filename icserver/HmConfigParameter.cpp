#include "HmConfigParameter.h"
#include <Logger.h>
#include <stdio.h>

HmConfigParameter::HmConfigParameter(void)
{
}

HmConfigParameter::~HmConfigParameter(void)
{
}

bool HmConfigParameter::ReadFromXml(XMLNode& paramNode)
{
	const char* temp = paramNode.getAttribute("name");
	if( !temp )
	{
		LOG(Logger::LOG_ERROR, "<parameter> missing \"name\" attribute");
		return false;
	}
	_name = temp;

	temp = paramNode.getAttribute("value");
	if( temp )
	{
		_value = temp;
		_hasValue = true;
	}else{
		_hasValue = false;
	}

	temp = paramNode.getAttribute("reset_to_default");
	_resetToDefault = (!temp) || temp[0]=='t';

	return true;
}

const std::string& HmConfigParameter::GetName()
{
	return _name;
}

const std::string& HmConfigParameter::GetValue()
{
	return _value;
}

bool HmConfigParameter::WantsResetToDefault()
{
	return _resetToDefault;
}

bool HmConfigParameter::HasValue()
{
	return _hasValue;
}
