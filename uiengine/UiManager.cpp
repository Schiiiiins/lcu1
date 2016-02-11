#include "UiManager.h"
#include "UiElement.h"
#include "CachedImage.h"
#include <Logger.h>
#include "UiPage.h"
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>

UiManager::UiManager(void) : _icClient("127.0.0.1", 8778), _inputManager(this), _audioManager(this)
{
	DirectFBCreate (&_dfb);
	
	_primarySurface = NULL;
	_eventBuffer = NULL;

	_invalid = false;
	_exit = false;
	_eventFd = -1;

	_screenWidth = 0;
	_screenHeight = 0;

	_currentX = 0;
	_currentY = 0;

	_waitCursor = NULL;

	_backlight.Trigger();
}

UiManager::~UiManager(void)
{
	for( t_pages::iterator it = _pages.begin(); it != _pages.end(); it++ )
	{
		delete it->second;
	}
	_pages.clear();
	_pageHistory.clear();
	for( t_mapFonts::iterator it = _mapFonts.begin(); it!=_mapFonts.end(); it++ )
	{
		it->second->Release(it->second);
	}
	_mapFonts.clear();
	for( t_mapImages::iterator it = _mapImages.begin(); it!=_mapImages.end(); it++ )
	{
		delete it->second;
	}
	_mapImages.clear();
	if(_primarySurface)_primarySurface->Release (_primarySurface);
	close( _eventFd );
	if(_eventBuffer)_eventBuffer->Release (_eventBuffer);
	if(_dfb)_dfb->Release (_dfb);

}

bool UiManager::ShowPage( const std::string& pageId )
{
	LOG(Logger::LOG_DEBUG, "ShowPage(%s)", pageId.c_str());
	UiPage* page = GetPage( pageId );
	if( !page )
	{
		LOG(Logger::LOG_ERROR, "ShowPage: page %s not found", pageId.c_str());
		return false;
	}
	UiPage* lastPage = NULL;
	if( _pageHistory.size() )
	{
		if( _pageHistory.back()->GetId() == pageId )
		{
			//requested Page is already shown
			return true;
		}
		lastPage = _pageHistory.back();
	}
	ShowWaitCursor();
	_pageHistory.push_back(page);
	if( _pageHistory.size() > PageHistorySize )
	{
		_pageHistory.pop_front();
	}
	_pageHistory.back()->OnLoad(UiPage::LoadReasonShow, lastPage);
	Invalidate();
	return true;
}

UiPage* UiManager::GetCurrentPage()
{
	return _pageHistory.back();
}

bool UiManager::Back()
{
	LOG(Logger::LOG_DEBUG, "Back()");
	if( _pageHistory.size() < 2 )
	{
		LOG(Logger::LOG_WARNING, "No page left in history to go back to");
		return false;
	}
	UiPage* lastPage = _pageHistory.back();
	_pageHistory.pop_back();
	_pageHistory.back()->OnLoad(UiPage::LoadReasonBack, lastPage);
	Invalidate();
	return true;
}

UiPage* UiManager::GetPage( const std::string& pageId )
{
	t_pages::iterator it = _pages.find( pageId );
	if( it == _pages.end() )
	{
		LOG( Logger::LOG_ERROR, "Page %s not found", pageId.c_str());
		return NULL;
	}
	return it->second;
}

IDirectFBSurface* UiManager::GetSurface()
{
	return _primarySurface;
}

IDirectFB* UiManager::GetDirectFB()
{
	return _dfb;
}

bool UiManager::Init( const std::string& directory )
{
	_baseDirectory = directory;
	if( _baseDirectory[_baseDirectory.size()-1] != '/' )_baseDirectory += '/';
	DIR *pDirectory;
	bool retval=true;
	struct dirent *pEntry;
	pDirectory = opendir(directory.c_str());
	if(!pDirectory)
	{
		LOG(Logger::LOG_ERROR, "could not open directory %s", directory.c_str());
		return false;
	}

	pEntry = readdir( pDirectory );
	while(retval && pEntry){
		XMLResults xmlResult;
		std::string filename=directory;
		filename+="/";
		filename+=pEntry->d_name;
		unsigned int dotpos=filename.rfind('.');
		if(dotpos!=std::string::npos && filename.substr(dotpos)==".xml"){
			LOG( Logger::LOG_DEBUG, "Loading page %s", pEntry->d_name);
			XMLNode rootNode = XMLNode::parseFile( filename.c_str(), "ui", &xmlResult );
			if(!xmlResult.error){
				int i = 0;
				XMLNode pageNode = rootNode.getChildNode("page", &i);
				while(!pageNode.isEmpty())
				{
					UiPage* page = new UiPage(this);
					if(!page->InitFromXml( pageNode ))
					{
						LOG( Logger::LOG_ERROR, "Error initializing page from file %s", pEntry->d_name);
						delete page;
						retval = false;
						break;
					}else{
						if( _pages.find(page->GetId()) != _pages.end() )
						{
							LOG( Logger::LOG_ERROR, "Page id \"%s\" multiply defined", page->GetId().c_str());
							retval = false;
							break;
						}
						_pages[page->GetId()] = page;
						const char* temp = pageNode.getAttribute("start");
						if( temp && temp[0]=='t' )
						{
							_pageHistory.push_back(page);
						}
					}
					pageNode = rootNode.getChildNode("page", &i);
				}
			}else{
				LOG( Logger::LOG_ERROR, "Error parsing file %s", pEntry->d_name);
				retval = false;
				break;
			}
		}
		pEntry = readdir( pDirectory );
	}
	closedir(pDirectory);
	if( retval )
	{
		if( _pageHistory.size() > 1 )
		{
			LOG( Logger::LOG_ERROR, "More than one start page defined");
			retval = false;
		}else if( _pageHistory.size() == 0 )
		{
			LOG( Logger::LOG_ERROR, "No start page defined");
			retval = false;
		}
		if( retval )
		{
			if( !_audioManager.IsInitialized() )_audioManager.Init();
			UiPage* startPage = _pageHistory.back();
			startPage->OnLoad(UiPage::LoadReasonShow, NULL);
			Invalidate();
			_waitCursor = GetImage(startPage->GetWaitCursor());
			if( !_waitCursor )
			{
				LOG( Logger::LOG_WARNING, "No wait cursor image found");
			}
		}
	}
	return retval;
}

std::string UiManager::GetAbsoluteFilename(const std::string& relativeFilename)
{
	if( !relativeFilename.size() )
	{
		if( _baseDirectory.size() )
		{
			//return base directory without trailing '/'
			return _baseDirectory.substr(0, _baseDirectory.size()-1);
		}
	}else{
		if( relativeFilename[0] == '/' )
		{
			return relativeFilename;
		}
	}
	return _baseDirectory + relativeFilename;
}

void UiManager::Exit()
{
	_exit = true;
}

void UiManager::Invalidate()
{
	_invalid = true;
}

void UiManager::Render()
{
	if(_pageHistory.size())
	{
		_pageHistory.back()->Render(_primarySurface);
	}
	DFBResult err = _primarySurface->Flip (_primarySurface, NULL, DSFLIP_WAITFORSYNC);
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return;
	}
}

void UiManager::ShowWaitCursor()
{
	if( _waitCursor && _primarySurface )
	{
		int x = _currentX - _waitCursor->GetWidth()/2;
		int y = _currentY - _waitCursor->GetHeight()/2;

		_waitCursor->Render( _primarySurface, x, y );
		_primarySurface->Flip (_primarySurface, NULL, DSFLIP_WAITFORSYNC);
		Invalidate();
	}
}

void UiManager::HandlePress( int x, int y )
{
	if( !_backlight.Trigger() )return;
	_currentX = x;
	_currentY = y;

	if(_pageHistory.size())
	{
		_pageHistory.back()->HandlePress(x, y);
	}
}

bool UiManager::CreatePrimarySurface()
{
	_dfb->SetCooperativeLevel (_dfb, DFSCL_FULLSCREEN);
	DFBSurfaceDescription dsc;
	dsc.flags = DSDESC_CAPS;
	dsc.caps  = (DFBSurfaceCapabilities)(DSCAPS_PRIMARY | DSCAPS_FLIPPING);
	DFBResult err = _dfb->CreateSurface( _dfb, &dsc, &_primarySurface );
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return false;
	}
	_primarySurface->GetSize( _primarySurface, &_screenWidth, &_screenHeight );
	_primarySurface->SetBlittingFlags(_primarySurface, DSBLIT_BLEND_ALPHACHANNEL);
	err = _dfb->CreateInputEventBuffer( _dfb, DICAPS_ALL, DFB_FALSE, &_eventBuffer );
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return false;
	}
	if( (err=_eventBuffer->CreateFileDescriptor(_eventBuffer, &_eventFd)) != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d creating event file descriptor", err );
		return false;
	}
	fcntl(_eventFd, F_SETFL, O_NONBLOCK);
	return true;
}

bool UiManager::Work( int timeout )
{
	if( !_primarySurface )
	{
		CreatePrimarySurface();
	}

	if(_invalid)
	{
		Render();
		_invalid = false;
		return true;
	}

	int pendingEvents = WaitForEventWithTimeout( timeout );
	if( pendingEvents & EventInput )
	{

		DFBInputEvent evt;

		while (GetInputEvent( &evt )) {
			_inputManager.HandleInputEvent( evt );
			if( _exit )return false;
		}
	}
	if( pendingEvents & EventControl )
	{
		_icClient.DoPendingWork();
	}
	if( pendingEvents & EventAudio )
	{
		_audioManager.DoPendingWork();
	}
	std::string receivedValueId;
	unsigned long receivedValue;
	while( _icClient.GetNextReceivedValue( &receivedValueId, &receivedValue ) )
	{
		ExecuteValueTriggers( receivedValueId, receivedValue );
	}
	if(_invalid)
	{
		Render();
		_invalid = false;
	}
	return true;
}

int UiManager::WaitForEventWithTimeout( unsigned long timeout )
{
	// Construct the sets of descriptors we are interested in
	fd_set inFd, outFd, excFd;
	FD_ZERO(&inFd);
	FD_ZERO(&outFd);
	FD_ZERO(&excFd);

	int maxFd = _eventFd;
	FD_SET(_eventFd, &inFd);
	if( _icClient.IsConnected() )
	{
		int fd = _icClient.GetFd();
		FD_SET(fd, &inFd);
		if( fd > maxFd )maxFd = fd;
	}

	int audioFd = _audioManager.GetFd();
	if( audioFd >= 0 )
	{
		FD_SET(audioFd, &inFd);
		if( audioFd > maxFd )maxFd = audioFd;
	}
	// Check for events
	int nEvents;
	if (timeout == 0){
		nEvents = select(maxFd+1, &inFd, &outFd, &excFd, NULL);
	}
	else 
	{
		struct timeval tv;
		tv.tv_sec = timeout/1000;
		tv.tv_usec = (timeout%1000)*1000;
		nEvents = select(maxFd+1, &inFd, &outFd, &excFd, &tv);
	}
	if( nEvents < 0 )
	{
		LOG( Logger::LOG_ERROR, "Error calling select: %d", nEvents );
		return 0;
	}
	int retval = EventNone;
	if( _icClient.IsConnected() && FD_ISSET(_icClient.GetFd(), &inFd))
	{
		retval |= EventControl;
	}
	if( audioFd >= 0 && FD_ISSET(audioFd, &inFd) )
	{
		retval |= EventAudio;
	}
	if( FD_ISSET(_eventFd, &inFd) )
	{
		retval |= EventInput;
	}
	return retval;
}

bool UiManager::GetInputEvent( DFBInputEvent* inputEvent )
{
	DFBEvent evt;
	int count = read(_eventFd, &evt, sizeof(evt));
	if( count < 0 )
	{
		if( errno == EAGAIN )return false;
		LOG( Logger::LOG_ERROR, "Error reading from event fd: %d", errno );
		return false;
	}
	if( count != sizeof(evt) )
	{
		LOG( Logger::LOG_ERROR, "Expected %d bytes on read from event fd but received %d bytes", sizeof(evt), count );
		return false;
	}
	if( evt.clazz != DFEC_INPUT )
	{
		LOG( Logger::LOG_DEBUG, "Discarding unexpected event of clazz %d", evt.clazz );
		return false;
	}
	memcpy( inputEvent, &evt.input, sizeof(DFBInputEvent) );
	return true;
}

IDirectFBFont* UiManager::GetFont( const std::string& face, int height )
{
	t_mapFonts::key_type key(face, height);
	if( _mapFonts[key] )
	{
		//LOG( Logger::LOG_DEBUG, "Reusing font %s:%d", face.c_str(), height );
		return _mapFonts[key];
	}

	DFBFontDescription fontDsc;
	fontDsc.flags = DFDESC_HEIGHT;
	fontDsc.height = height;

	IDirectFBFont* font;
	DFBResult err = _dfb->CreateFont (_dfb, GetAbsoluteFilename(face).c_str(), &fontDsc, &font);
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d creating font %s", err, face.c_str() );
		return NULL;
	}
	//LOG( Logger::LOG_DEBUG, "Created font %s:%d", face.c_str(), height );
	_mapFonts[key] = font;
	return font;
}

IcClient* UiManager::GetIcClient()
{
	return &_icClient;
}

CachedImage* UiManager::GetImage(const std::string& relativeFilename)
{
	CachedImage* image = _mapImages[relativeFilename];
	if( image )return image;
	image = new CachedImage();
	if( !image->Load(_dfb, GetAbsoluteFilename(relativeFilename)))
	{
		delete image;
		return NULL;
	}
	_mapImages[relativeFilename] = image;
	return image;
}

bool UiManager::ShowSplash( const std::string& filename )
{
	CachedImage image;
	if( !image.Load(_dfb, filename ) )
	{
		return false;
	}
	if( !_primarySurface )
	{
		if(! CreatePrimarySurface() )
		{
			return false;
		}
	}
	LOG( Logger::LOG_DEBUG, "Showing splash image %s", filename.c_str());

	DFBResult err = _primarySurface->Clear (_primarySurface, 0, 0, 0, 0xff);
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return false;
	}

	int x = (_screenWidth - image.GetWidth()) / 2;
	int y = (_screenHeight - image.GetHeight()) / 2;
	image.Render( _primarySurface, x, y );
	err = _primarySurface->Flip (_primarySurface, NULL, DSFLIP_WAITFORSYNC);
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return false;
	}
	return true;
}

void UiManager::RegisterGlobalTrigger( const std::string& valueId, UiPage* page )
{
	std::set<UiPage*>& pages = _mapGlobalTriggers[valueId];
	pages.insert( page );
	GetIcClient()->RegisterChannel( valueId, false );
}

void UiManager::ExecuteValueTriggers(const std::string& id, unsigned long value)
{
	UiPage* curPage = NULL;
	if(_pageHistory.size())
	{
		curPage = _pageHistory.back();
		curPage->HandleValueTrigger( id, value );
	}
	t_mapGlobalTriggers::iterator it = _mapGlobalTriggers.find( id );
	if( it != _mapGlobalTriggers.end() )
	{
		t_setPages::iterator pagesIt;
		for( pagesIt = it->second.begin(); pagesIt != it->second.end(); pagesIt++ )
		{
			if( (*pagesIt) != curPage )
			{
				(*pagesIt)->HandleValueTrigger( id, value );
			}
		}
	}
}

void UiManager::ExecuteAudioEvent( const std::string id, const std::vector<std::string>& params )
{
	UiPage* curPage = NULL;
	if(_pageHistory.size())
	{
		curPage = _pageHistory.back();
		curPage->HandleAudioEvent( id, params );
	}
}

AudioManager* UiManager::GetAudioManager()
{
	return &_audioManager;
}

UiBacklight* UiManager::GetBacklight()
{
	return &_backlight;
}
