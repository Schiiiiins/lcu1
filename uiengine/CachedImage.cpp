#include "CachedImage.h"
#include <Logger.h>

CachedImage::CachedImage()
{
	_width = _height = 0;
}

CachedImage::~CachedImage()
{
	if( _imageSurface )
	{
		_imageSurface -> Release( _imageSurface );
	}
}

bool CachedImage::Load(IDirectFB* dfb, const std::string& imageFilename)
{
	IDirectFBImageProvider *provider;

	DFBResult err = dfb->CreateImageProvider (dfb, imageFilename.c_str(), &provider);
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d reading image file %s", err, imageFilename.c_str() );
		return false;
	}

	DFBSurfaceDescription dsc;
	err = provider->GetSurfaceDescription (provider, &dsc);
	if( err != DFB_OK )
	{
		provider->Release(provider);
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return false;
	}

	_width = dsc.width;
	_height = dsc.height;
	dsc.pixelformat = DSPF_ARGB;

	err = dfb->CreateSurface( dfb, &dsc, &_imageSurface );
	if( err != DFB_OK )
	{
		provider->Release(provider);
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return false;
	}

	err = provider->RenderTo (provider, _imageSurface, NULL);
	if( err != DFB_OK )
	{
		_imageSurface->Release(_imageSurface);
		_imageSurface = NULL;
		provider->Release(provider);
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return false;
	}
	provider->Release (provider);
	return true;
}

void CachedImage::Render(IDirectFBSurface* sfc, int x, int y)
{
	if( !_imageSurface )
	{
		LOG( Logger::LOG_ERROR, "UiImage: called Render() without a surface object");
		return;
	}

	DFBResult err;

	err = sfc->Blit(sfc, _imageSurface, NULL, x, y); 
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return;
	}
}

int CachedImage::GetWidth()
{
	return _width;
}

int CachedImage::GetHeight()
{
	return _height;
}
