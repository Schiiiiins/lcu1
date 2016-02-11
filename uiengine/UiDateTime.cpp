#include "UiDateTime.h"
#include "UiManager.h"
#include "dynamic.h"
#include <Logger.h>
#include "UiPage.h"

static dynamic::factory<UiDateTime> UiDateTimeFactory("datetime");

UiDateTime::UiDateTime(void)
{
	_font = NULL;
	_r = 0;
	_g = 0;
	_b = 0;
	_center = false;
	_resolution = 1;
	_format = "%x %X";
}

UiDateTime::~UiDateTime(void)
{
	//don't release font here. It is owned by UiManager.
}

bool UiDateTime::InitFromXml( XMLNode& rootNode )
{
	if( _font )
	{
		_font -> Release( _font );
		_font = NULL;
	}

	if( !UiElement::InitFromXml( rootNode ) )
	{
		return false;
	}

	const char* temp = rootNode.getAttribute("color");
	if( temp )
	{
		int color = strtol(temp, NULL, 0);
		_r = (color>>16)&0xff;
		_g = (color>>8)&0xff;
		_b = (color>>0)&0xff;
	}

	temp = rootNode.getAttribute("center");
	if( temp && temp[0] == 't')
	{
		_center = true;
	}

	std::string face;
	temp = rootNode.getAttribute("font");
	if( temp )
	{
		face = temp;
	}

	_font = _page->GetFont( face, _height );

	temp = rootNode.getAttribute("format");
	if( temp )
	{
		_format = temp;
		if( _format.find("%s") != std::string::npos ||
			_format.find("%S") != std::string::npos ||
			_format.find("%X") != std::string::npos ||
			_format.find("%r") != std::string::npos ||
			_format.find("%T") != std::string::npos )
		{
			_resolution = 1;
		}else{
			_resolution = 60;
		}
	}

	return true;
}

void UiDateTime::Render()
{
	IDirectFBSurface* sfc = GetSurface();
	if( !sfc )
	{
		LOG( Logger::LOG_ERROR, "UiImage: called Render() without a rendering surface");
		return;
	}

	if( !_font )
	{
		LOG( Logger::LOG_ERROR, "UiDateTime: called Render() without a font object");
		return;
	}

	DFBResult err;

	err = sfc->SetFont( sfc, _font );
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return;
	}
	
	err = sfc->SetColor( sfc, _r, _g, _b, 0xff );
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return;
	}

	char buffer[128];
	
	time_t now = time(NULL);
	strftime(buffer, sizeof(buffer), _format.c_str(), localtime(&now));
	err = sfc->DrawString( sfc, buffer, -1, _x, _y, _center ? DSTF_TOPCENTER : DSTF_TOPLEFT );
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return;
	}
	struct timeb tp;
	ftime(&tp);
	int delay = (_resolution - (tp.time % _resolution)) * 1000 - tp.millitm;
	KillTimer( 0 );
	RequestTimer( delay, 0 );
}

const std::string& UiDateTime::GetFormat()
{
	return _format;
}

void UiDateTime::SetFormat(const std::string& format)
{
	if( _format != format )
	{
		_format = format;
		_page->Invalidate();
	}
}

void UiDateTime::OnTimer(unsigned long cookie)
{
	//LOG( Logger::LOG_DEBUG, "Time update");
	_page->Invalidate();
}
