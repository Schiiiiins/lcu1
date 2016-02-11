#include "UiText.h"
#include "UiManager.h"
#include "dynamic.h"
#include <Logger.h>
#include "UiPage.h"

static dynamic::factory<UiText> UiTextFactory("text");

UiText::UiText(void)
{
	_font = NULL;
	_r = 0;
	_g = 0;
	_b = 0;
	_center = false;
}

UiText::~UiText(void)
{
	//don't release font here. It is owned by UiManager.
}

bool UiText::InitFromXml( XMLNode& rootNode )
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

	temp = rootNode.getAttribute("text");
	if( temp )
	{
		_text = temp;
	}

	return true;
}

void UiText::Render()
{
	IDirectFBSurface* sfc = GetSurface();
	if( !sfc )
	{
		LOG( Logger::LOG_ERROR, "UiImage: called Render() without a rendering surface");
		return;
	}

	if( !_font )
	{
		LOG( Logger::LOG_ERROR, "UiText: called Render() without a font object");
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

	err = sfc->DrawString( sfc, _text.c_str(), -1, _x, _y, _center ? DSTF_TOPCENTER : DSTF_TOPLEFT );
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return;
	}

}

const std::string& UiText::GetText()
{
	return _text;
}

void UiText::SetText(const std::string& text)
{
	if( _text != text )
	{
		_text = text;
		_page->Invalidate();
	}
}

bool UiText::SetProperty( const std::string& id, const std::string& value )
{
	if( id == "text" ){
		_text = value;
	}else if( id == "color" ){
		int color = strtol(value.c_str(), NULL, 0);
		_r = (color>>16)&0xff;
		_g = (color>>8)&0xff;
		_b = (color>>0)&0xff;
	}else{
		return UiElement::SetProperty(id, value);
	}
	_page->Invalidate();
	return true;
}

bool UiText::GetProperty( const std::string& id, std::string* value )
{
	if( id == "text" ){
		*value = _text;
	}else if( id == "color" ){
		char buffer[16];
		sprintf(buffer, "0x%06X", (_r<<16)|(_g<<8)|_b);
		*value = buffer;
	}else{
		return UiElement::GetProperty(id, value);
	}
	return true;
}

bool UiText::HitTest( int x, int y )
{
	if( !_center )return UiElement::HitTest(x, y);
	else return UiElement::HitTest( x + _width/2, y );
}

