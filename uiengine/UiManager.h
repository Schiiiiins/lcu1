#pragma once

#include <directfb.h>

#include <xmlParser.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include "IcClient.h"
#include "InputManager.h"
#include <TimerTarget.h>
#include "UiBacklight.h"
#include "AudioManager.h"

class UiElement;

class UiPage;

class CachedImage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Manager for user interfaces.  </summary>
///
/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class UiManager //: public TimerTarget
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiManager(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Finaliser. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	~UiManager(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialises this object. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="directory">	Pathname of the UI description base directory. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Init( const std::string& directory );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Wait for an event, process it and return. If no event arrives, return after the timeout
	/// 			expires. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="timeout">	The timeout in ms. </param>
	///
	/// <returns>	true if it expects to be called again, false if the application shall exit. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Work( int timeout );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Invalidates the UI, causing it to be re-rendered on the next call to 
	/// 			<see cref="Work" />. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Invalidate();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the directFB surface for rendering. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	null if it fails, else the surface. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	IDirectFBSurface* GetSurface();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the directFB interface object. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	null if it fails, else the direct fb. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	IDirectFB* GetDirectFB();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets the exit flag causing <see cref="Work" /> to return false. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Exit();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets an absolute filename given either an absolute filename or a filename relative
	/// 			to the UI description base directory. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="relativeFilename">	possibly relative filename. </param>
	///
	/// <returns>	The absolute filename. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string GetAbsoluteFilename(const std::string& relativeFilename);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Shows the page with the given identifier. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="pageId">	Identifier for the page. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool ShowPage( const std::string& pageId );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a page by identifier. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="pageId">	Identifier for the page. </param>
	///
	/// <returns>	null if it fails, else the page. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiPage* GetPage( const std::string& pageId );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the current page. </summary>
	///
	/// <remarks>	Lars Reemts, 23.05.2011. </remarks>
	///
	/// <returns>	null if it fails, else the current page. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiPage* GetCurrentPage();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Shows the previous page from the page history. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Back();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a font by name and height. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="face">  	The font face name. This parameter is interpreted as path to the font
	/// 						file. Either an absolute path or a path relative to the UI manager 
	/// 						description directory is accepted. </param>
	/// <param name="height">	The requested height of the font. </param>
	///
	/// <returns>	null if it fails, else the font. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	IDirectFBFont* GetFont( const std::string& face, int height );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the immediateC client. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	The immediateC client. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	IcClient* GetIcClient();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets an image given a possibly relative filename. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="relativeFilename">	Filename of the image file, either absolute or relative 
	/// 								to the UI description base directory. </param>
	///
	/// <returns>	null if it fails, else the image. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	CachedImage* GetImage(const std::string& relativeFilename);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Shows the given image as splash screen. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="filename">	Filename of the image file, either absolute or relative 
	/// 						to the UI description base directory. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool ShowSplash( const std::string& filename );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Registers a global immediateC trigger handler. A global trigger handler is called
	/// 			even if the defining page is not currently shown. Regular trigger handlers are
	/// 			only called if they are defined on the current page. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="valueId">	Identifier for the immediateC value. </param>
	/// <param name="page">   	[in] The page that defines the TCL handler procedure. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void RegisterGlobalTrigger( const std::string& valueId, UiPage* page );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Shows the wait cursor. The wait cursor image is defined in the XML description for
	/// 			the start page. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void ShowWaitCursor();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the audio manager. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	The audio manager. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AudioManager* GetAudioManager();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Dispatches an audio event to the current page. 
	/// 			<see cref="UiPage::HandleAudioEvent /> </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">	 	The audio event identifier. </param>
	/// <param name="params">	Parameters passed to the TCL procedure. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void ExecuteAudioEvent( const std::string id, const std::vector<std::string>& params );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the backlight controller. </summary>
	///
	/// <remarks>	Lars Reemts, 20.05.2011. </remarks>
	///
	/// <returns>	The backlight controller. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiBacklight* GetBacklight();

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constant values.  </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum{
		/// <summary> Size of the page history. </summary>
		PageHistorySize = 100
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Values for the bit mask of events returned from 
	/// 			<see cref="WaitForEventWithTimeout" />. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	enum{
		/// <summary> No event. </summary>
		EventNone = 0,
		/// <summary> immediateC value changed. </summary>
		EventControl = (1<<0),
		/// <summary> Touch screen input event. </summary>
		EventInput = (1<<1),
		/// <summary> Audio player event. </summary>
		EventAudio = (1<<2)
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Renders the current page onto the directFB surface. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Render();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handle a touch screen press by triggering the backlight and forwarding the event 
	/// 			to the current page. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="x">	The x coordinate. </param>
	/// <param name="y">	The y coordinate. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void HandlePress( int x, int y );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Retrieve the next directFB input event. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="evt">	[out] The event. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool GetInputEvent( DFBInputEvent* evt );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Wait for event with timeout. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="timeout">	The timeout. </param>
	///
	/// <returns>	Bitmask of event types. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int WaitForEventWithTimeout( unsigned long timeout );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Creates the primary directFB surface. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CreatePrimarySurface();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the registered global and page dependent triggers for an 
	/// 			immediateC value. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">   	The immediateC value identifier. </param>
	/// <param name="value">	The new value. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void ExecuteValueTriggers(const std::string& id, unsigned long value);

	/// <summary> The directFB object </summary>
	IDirectFB *_dfb;
	/// <summary> The primary directFB surface </summary>
	IDirectFBSurface *_primarySurface;
	/// <summary> DirectFB event buffer object </summary>
	IDirectFBEventBuffer *_eventBuffer;
	/// <summary> Pathname of the UI description base directory </summary>
	std::string _baseDirectory; 
	/// <summary> true if the current page needs to be re-rendered </summary>
	bool _invalid;
	/// <summary> true if <see cref="Work" /> shall return false causing the application to exit </summary>
	bool _exit;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	typedef for the map of UI pages. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::map<std::string, UiPage*> t_pages;
	/// <summary> Map of UI pages </summary>
	t_pages _pages;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	typedef for the page history. . </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::list<UiPage*> t_pageHistory;
	/// <summary> The page history </summary>
	t_pageHistory _pageHistory;
	/// <summary> The event fd associated with the <see cref="_eventBuffer" /> </summary>
	int _eventFd;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	typedef for the map of fonts. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::map<std::pair<std::string, int>, IDirectFBFont*> t_mapFonts;
	/// <summary> The map of fonts </summary>
	t_mapFonts _mapFonts;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	typedef for the image cache. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::map<std::string, CachedImage*> t_mapImages;
	/// <summary> The image cache </summary>
	t_mapImages _mapImages;
	/// <summary> The immediateC client </summary>
	IcClient _icClient;
	/// <summary> Input manager object </summary>
	InputManager _inputManager;
	friend class InputManager;
	/// <summary> Audio manager object </summary>
	AudioManager _audioManager;
	/// <summary> Width of the screen </summary>
	int _screenWidth;
	/// <summary> Height of the screen </summary>
	int _screenHeight;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	typedef for a set of UI pages. Used for global trigger handling. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::set<UiPage*> t_setPages;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	typedef mapping immediateC identifiers to sets of UI pages. 
	/// 			Used for global trigger handling. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::map<std::string, t_setPages> t_mapGlobalTriggers;
	/// <summary> The map of global immediateC triggers </summary>
	t_mapGlobalTriggers _mapGlobalTriggers;

	/// <summary> The wait cursor </summary>
	CachedImage* _waitCursor;

	/// <summary> The x coordinate of the last touch screen event</summary>
	int _currentX;
	/// <summary> The y coordinate of the last touch screen event</summary>
	int _currentY;

	/// <summary> The backlight object </summary>
	UiBacklight _backlight;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of UiManager.h
////////////////////////////////////////////////////////////////////////////////////////////////////

