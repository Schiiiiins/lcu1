#include "UiElement.h"
#include <Logger.h>
#include "UiPage.h"
#include "UiManager.h"

UiElement::UiElement()
{
	_width = 0;
	_height = 0;
	_x = 0;
	_y = 0;
	_page = NULL;
	_clickAction = ClickActionNone;
	_visible = true;
}

bool UiElement::InitFromXml( XMLNode& rootNode )
{
	const char* temp;
	temp = rootNode.getAttribute("id");
	if( temp )
	{
		_id = temp;
	}

	temp = rootNode.getAttribute("visible");
	if( temp && temp[0] != 't' )
	{
		_visible = false;
	}

	temp = rootNode.getAttribute("width");
	if( temp )
	{
		_width = strtol(temp, NULL, 0);
	}

	temp = rootNode.getAttribute("height");
	if( temp )
	{
		_height = strtol(temp, NULL, 0);
	}

	temp = rootNode.getAttribute("x");
	if( temp )
	{
		_x = strtol(temp, NULL, 0);
	}

	temp = rootNode.getAttribute("y");
	if( temp )
	{
		_y = strtol(temp, NULL, 0);
	}

	temp = rootNode.getAttribute("onclick");
	if( temp )
	{
		std::string action = temp;
		std::string actionParam;
		std::string::size_type colonpos = action.find_first_of(':');
		if(colonpos != std::string::npos)
		{
			actionParam = action.substr(colonpos+1);
			action = action.substr(0, colonpos);
		}
		if( action == "exit" )
		{
			_clickAction = ClickActionExit;
			_clickActionParam.clear();
		}else if( action == "back" )
		{
			_clickAction = ClickActionBack;
			_clickActionParam.clear();
		}else if( action == "link" )
		{
			_clickAction = ClickActionLink;
			_clickActionParam = actionParam;
		}else if( action == "code" )
		{
			_clickAction = ClickActionCode;
			_clickActionParam = actionParam;
		}else{
			LOG(Logger::LOG_ERROR, "Invalid onlick specification: %s", temp);
			return false;
		}
	}
	return true;
}

bool UiElement::Init(UiPage* page)
{
	_page =page;
	return true;
}

bool UiElement::HitTest( int x, int y )
{
	x -= _x;
	y -= _y;
	return !( x<0 || y<0 || x>=_width || y>=_height );
}

void UiElement::HandlePress( int x, int y )
{
	//hit test
	if( !HitTest( x, y ) )return;
	switch(_clickAction)
	{
	case ClickActionNone:
		break;
	case ClickActionExit:
		_page->GetManager()->Exit();
		break;
	case ClickActionBack:
		_page->GetManager()->Back();
		break;
	case ClickActionLink:
		_page->GetManager()->ShowPage(_clickActionParam);
		break;
	case ClickActionCode:
		_page->CodeEvent( this, _clickActionParam );
		break;
	}
}

IDirectFBSurface* UiElement::GetSurface()
{
	if(!_page)return NULL;
	return _page->GetManager()->GetSurface();
}

IDirectFB* UiElement::GetDirectFB()
{
	if(!_page)return NULL;
	return _page->GetManager()->GetDirectFB();
}

const std::string& UiElement::GetId()
{
	return _id;
}

bool UiElement::SetProperty( const std::string& id, const std::string& value )
{
	if( id == "width" ){
		_width = strtoul(value.c_str(), NULL, 0);
	}else if( id == "height" ){
		_height = strtoul(value.c_str(), NULL, 0);
	}else if( id == "x" ){
		_x = strtoul(value.c_str(), NULL, 0);
	}else if( id == "y" ){
		_y = strtoul(value.c_str(), NULL, 0);
	}else if( id == "visible" ){
		_visible = value != "true" && value != "0";
	}else{
		return false;
	}
	LOG( Logger::LOG_DEBUG, "%s.%s=%s", GetId().c_str(), id.c_str(), value.c_str());
	_page->Invalidate();
	return true;
}

bool UiElement::GetProperty( const std::string& id, std::string* value )
{
	char buffer[32];
	if( id == "width" ){
		sprintf(buffer, "%d", _width);
	}else if( id == "height" ){
		sprintf(buffer, "%d", _height);
	}else if( id == "x" ){
		sprintf(buffer, "%d", _x);
	}else if( id == "y" ){
		sprintf(buffer, "%d", _y);
	}else if( id == "visible" ){
		sprintf(buffer, "%d", _visible?1:0);
	}else{
		return false;
	}
	*value = buffer;
	return true;
}

bool UiElement::IsVisible()
{
	return _visible;
}
