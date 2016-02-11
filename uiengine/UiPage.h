#pragma once

#include <xmlParser.h>
#include <directfb.h>
#include <list>
#include <string>
#include "PageScriptingEngine.h"

class UiManager;

class UiElement;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	User interface page.  </summary>
///
/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class UiPage
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constant values that represent the reason for loading a page.  </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef enum
	{
		/// <summary> Page was loaded explicitly as result of a <c>ui load</c> TCL command, a 
		/// 		  <c>link:</c> XML action or as start page.  </summary>
		LoadReasonShow,
		/// <summary> Page was loaded as result of a <c>ui back</c> TCL command or a <c>back</c> 
		/// 		  XML action. </summary>
		LoadReasonBack
	}LoadReason;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="manager">	[in] The UI manager. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiPage(UiManager* manager);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Finaliser. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	~UiPage(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Renders the page on the given directFB surface. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="sfc">	[in] The directFB surface object. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Render(IDirectFBSurface* sfc);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Invalidates this page causing it to be re-rendered on the next occasion. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void Invalidate();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the UI manager object. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	The manager. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiManager* GetManager();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initializes this page from an XML description. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="rootNode">	[in] The XML root node. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool InitFromXml( XMLNode& rootNode );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handle a touch screen press (click). </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="x">	The x coordinate. </param>
	/// <param name="y">	The y coordinate. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void HandlePress( int x, int y );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles a code event from a child element. Causes the corresponding TCL procedure to be called.</summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="sender">	[in] If not null, the sender element. </param>
	/// <param name="evt">   	The name of the TCL procedure to call. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void CodeEvent(UiElement* sender, const std::string& evt);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a user interface element by id. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">	The identifier. </param>
	///
	/// <returns>	null if it fails, else the user interface element. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiElement* GetUiElement( const std::string& id );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the page identifier. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	The page identifier. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	const std::string& GetId();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Executes the TCL procedure registered for the load event. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="loadReason">	The load reason. </param>
	/// <param name="lastPage">  	[in] If non-null, the previous page. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void OnLoad(LoadReason loadReason, UiPage* lastPage);

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

	IDirectFBFont* GetFont( std::string face, int height );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handle immediateC value trigger. Calls a TCL procedure registered as trigger for
	/// 			the given immediateC value identifier. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">   	The immediateC value identifier. </param>
	/// <param name="value">	The new value. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void HandleValueTrigger( const std::string id, unsigned long value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handle audio player event. Calls a TCL procedure registered as handler for the 
	/// 			given audio event. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">	 	The identifier. Supported values are: <c>start</c>, <c>progress</c>, 
	/// 						<c>stop</c>. </param>
	/// <param name="params">	Parameters passed to the TCL procedure. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void HandleAudioEvent( const std::string id, const std::vector<std::string>& params );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Adds a handler for an immediateC value trigger. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="controlValueId"> 	Identifier of the immediateC value to add the handler for. </param>
	/// <param name="method">		  	The name of the TCL procedure to call. </param>
	/// <param name="dynamic">		  	true if the trigger shall be removed automatically on each page
	/// 								load. Used for triggers added from TCL code. </param>
	/// <param name="registerChannel">	true to register as immediateC client as receiver for the 
	/// 								channel. If false, the channel should be registered at a later
	/// 								time. Used for triggers specified in the page XML description.
	/// 								In this case, actual channel registration is deferred until the 
	/// 								page is loaded for the first time. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void AddTrigger(const std::string& controlValueId, const std::string& method, bool dynamic, bool registerChannel);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the file path for the wait cursor image. Only valid on the start page. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	The wait cursor image file path. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	const std::string& GetWaitCursor();
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Removes all user interface elements. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void ClearUiElements();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Registers the value triggers. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void RegisterValueTriggers();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Registers the immediateC client as receiver for all immediateC values that have 
	/// 			a trigger defined. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void RemoveDynamicTriggers();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Removed all triggers marked as dynamic from the list of defined triggers. Called
	/// 			on each load of the page. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary> typedef for the list of user interface elements </summary>
	typedef std::list<UiElement*> t_uiElements;
	/// <summary> The list of user interface elements </summary>
	t_uiElements _uiElements;
	/// <summary> The UI manager object </summary>
	UiManager* _manager;
	/// <summary> The TCL scripting engine </summary>
	PageScriptingEngine _scriptingEngine;
	/// <summary> The page identifier </summary>
	std::string _id;
	/// <summary> Name of the TCL procedure called on page load </summary>
	std::string _onload;
	/// <summary> The default font face for the page </summary>
	std::string _defaultFontFace;
	/// <summary> The default font height for the page </summary>
	int _defaultFontHeight;
	/// <summary> The wait cursor. Only valid for the start page. </summary>
	std::string _waitCursor;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Trigger.  </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	struct t_trigger
	{
		std::string method;
		bool dynamic;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Defines an alias representing the triggers. . </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef	std::map<std::string, t_trigger> t_triggers;
	/// <summary> The triggers </summary>
	t_triggers _triggers;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Defines an alias representing the audio event handlers. . </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef	std::map<std::string, std::string> t_audioEventHandlers;
	/// <summary> The audio event handlers </summary>
	t_audioEventHandlers _audioEventHandlers;
	/// <summary> The background color </summary>
	unsigned long _backgroundColor;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of UiPage.h
////////////////////////////////////////////////////////////////////////////////////////////////////

