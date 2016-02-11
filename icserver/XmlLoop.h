#pragma once

#include <xmlParser.h>
#include <string>
#include "ExpressionParser.h"

class XmlLoop
{
public:
	XmlLoop(void);
	~XmlLoop(void);
	bool InitFromXml( XMLNode& loopNode );
	std::string ResolveIdentifier( const std::string id );
	bool Iterate();
	void Reset();
private:
	int _start;
	int _count;
	int _index;
	ExpressionParser _parser;
};
