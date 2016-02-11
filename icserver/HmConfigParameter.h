#pragma once
#include <set>
#include <string>
#include <xmlParser.h>

class HmConfigParameter
{
public:
	HmConfigParameter(void);
	~HmConfigParameter(void);
	bool ReadFromXml(XMLNode& paramNode);
	const std::string& GetName();
	const std::string& GetValue();
	bool WantsResetToDefault();
	bool HasValue();
private:
	std::string _name;
	std::string _value;
	bool _resetToDefault;
	bool _hasValue;
};
