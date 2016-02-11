#include "UiImage.h"
#include "UiManager.h"
#include "dynamic.h"
#include <Logger.h>
#include "UiPage.h"
#include "CachedImage.h"

static dynamic::factory<UiImage> UiImageFactory("image");

UiImage::UiImage(void)
{
	_imageIndex = 0;
}

UiImage::~UiImage(void)
{
}

bool UiImage::InitFromXml( XMLNode& rootNode )
{
	if( !UiElement::InitFromXml( rootNode ) )
	{
		return false;
	}

	int i = 0;
	
	const char* temp = rootNode.getAttribute("file", &i);
	if( temp )
	{
		CachedImage* cachedImage = _page->GetManager()->GetImage(temp);
		if( !cachedImage ) return false;
		_cachedImages.push_back( cachedImage );
		if( cachedImage->GetWidth() > _width ) _width = cachedImage->GetWidth();
		if( cachedImage->GetHeight() > _height ) _height = cachedImage->GetHeight();
		temp = rootNode.getAttribute("file", &i);
	}

	i=0;
	XMLNode fileNode = rootNode.getChildNode("file", &i);
	while( !fileNode.isEmpty() )
	{
		const char* temp = fileNode.getText();
		if( !temp )
		{
			LOG( Logger::LOG_ERROR, "Could not read node text from file node for element %s", rootNode.getName() );
			return false;
		}
		CachedImage* cachedImage = _page->GetManager()->GetImage(temp);
		if( !cachedImage ) return false;
		_cachedImages.push_back( cachedImage );
		if( cachedImage->GetWidth() > _width ) _width = cachedImage->GetWidth();
		if( cachedImage->GetHeight() > _height ) _height = cachedImage->GetHeight();
		fileNode = rootNode.getChildNode("file", &i);
	}

	if( _cachedImages.empty() )
	{
		LOG( Logger::LOG_ERROR, "<image> must have a file attribute or at leat one file child node" );
		return false;
	}
	return true;
}

void UiImage::Render()
{
	if( _cachedImages.size() > _imageIndex )_cachedImages[_imageIndex]->Render(GetSurface(), _x, _y);
}

bool UiImage::SetProperty( const std::string& id, const std::string& value )
{
	if( id == "index" ){
		_imageIndex = strtoul(value.c_str(), NULL, 0);
	}else{
		return UiElement::SetProperty(id, value);
	}
	_page->Invalidate();
	return true;
}

bool UiImage::GetProperty( const std::string& id, std::string* value )
{
	if( id == "index" ){
		char buffer[16];
		sprintf(buffer, "%u", _imageIndex);
		*value = buffer;
	}else{
		return UiElement::GetProperty(id, value);
	}
	return true;
}
