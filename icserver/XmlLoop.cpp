#include "XmlLoop.h"
#include <Logger.h>
#include <cstring>

XmlLoop::XmlLoop(void)
{
}

XmlLoop::~XmlLoop(void)
{
}

bool XmlLoop::InitFromXml( XMLNode& loopNode )
{
	const char* temp = loopNode.getAttribute("start");
	if( !temp )
	{
		LOG( Logger::LOG_ERROR, "<loop> is missing \"start\" attribute");
		return false;
	}
	_start = strtol( temp, NULL, 0 );
	if( _start < 0 )
	{
		LOG( Logger::LOG_ERROR, "<loop> \"start\" attribute must not be negative");
		return false;
	}

	temp = loopNode.getAttribute("count");
	if( !temp )
	{
		LOG( Logger::LOG_ERROR, "<loop> is missing \"count\" attribute");
		return false;
	}
	_count = strtol( temp, NULL, 0 );
	if( _count <= 0 )
	{
		LOG( Logger::LOG_ERROR, "<loop> \"count\" attribute must be positive");
		return false;
	}
	Reset();
	return true;
}

bool XmlLoop::Iterate()
{
	_index++;
	return _index < _start + _count;
}

void XmlLoop::Reset()
{
	_index = _start;
}

std::string XmlLoop::ResolveIdentifier( const std::string id )
{
	_parser.SetVariable("n", _index);
	std::string resolved = id;
	std::string::size_type pos = 0;
	while( (pos = resolved.find('[', pos)) != std::string::npos )
	{
		std::string::size_type closePos = resolved.find(']', pos);
		if( closePos == std::string::npos ){
			LOG( Logger::LOG_ERROR, "Mismatched brackets on %s", id.c_str());
			return false;
		}
		std::string expr = resolved.substr(pos+1, closePos - pos -1);
		int result;
		if( !_parser.Eval( expr, &result ) )
		{
			LOG( Logger::LOG_ERROR, "Error evaluating expression %s", expr.c_str());
			return false;
		}
		char buffer[16];
		sprintf(buffer, "%d", result);
		resolved.replace( pos, closePos - pos + 1, buffer );
		pos += strlen(buffer);
	}
	return resolved;
}
