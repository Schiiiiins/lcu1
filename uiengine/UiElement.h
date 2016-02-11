#ifndef _UIELEMENT_H_
#define _UIELEMENT_H_

#include <string>
#include <xmlParser.h>
#include <directfb.h>

class UiPage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Base class for all user interface elements.  </summary>
///
/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class UiElement
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Values that represent the action to perform when the element is clicked.  </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef enum
	{
		/// <summary> Do nothing (default).  </summary>
		ClickActionNone,
		/// <summary> Go to a different UI page.  </summary>
		ClickActionLink,
		/// <summary> Go back in the page history.  </summary>
		ClickActionBack,
		/// <summary> Exit the application.  </summary>
		ClickActionExit,
		/// <summary> Execute a TCL procedure.  </summary>
		ClickActionCode
	}ClickAction;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiElement();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialises this object. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="page">	[in] The UI page this UI element belongs to. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Init(UiPage* page);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initializes this object from an XML description. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="rootNode">	[in] The XML node containing the description for this object. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool InitFromXml( XMLNode& rootNode );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Must be implemented in any derived class to render this object on 
	/// 			the directFB surface. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void Render() = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Handles a press (click) on the touch screen. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="x">	The x coordinate. </param>
	/// <param name="y">	The y coordinate. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void HandlePress( int x, int y );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the UI element identifier. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	The identifier. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	const std::string& GetId();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets a property. Called from the TCL <c>ui set</c> command. 
	/// 			Supported properties:
	/// 			- <c>int width</c>: width in pixels
	/// 			- <c>int height</c>: height in pixels
	/// 			- <c>int x</c>: X coordinate of upper left corner
	/// 			- <c>int y</c>: Y coordinate of upper left corner
	/// 			- <c>bool visible</c>: determines if the element is visible or hidden
	/// 			</summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">   	The property identifier. </param>
	/// <param name="value">	The property value. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool SetProperty( const std::string& id, const std::string& value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a property. <see cref="SetProperty" /> for a list of properties. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">   	The identifier. </param>
	/// <param name="value">	[out] The value. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool GetProperty( const std::string& id, std::string* value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if this object is visible. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	true if visible, false if not. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool IsVisible();
protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Checks if the given coordinates belong to this UI element. </summary>
	///
	/// <remarks>	Lars Reemts, 04.05.2011. </remarks>
	///
	/// <param name="x">	The x coordinate. </param>
	/// <param name="y">	The y coordinate. </param>
	///
	/// <returns>	true if the test passes, false if the test fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool HitTest( int x, int y );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the surface. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	null if it fails, else the surface. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	IDirectFBSurface* GetSurface();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the direct fb. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <returns>	null if it fails, else the direct fb. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	IDirectFB* GetDirectFB();
	/// <summary> The width </summary>
	int _width;
	/// <summary> The height </summary>
	int _height;
	/// <summary> The x coordinate </summary>
	int _x;
	/// <summary> The y coordinate </summary>
	int _y;
	/// <summary> The identifier </summary>
	std::string _id;
	/// <summary> The page </summary>
	UiPage* _page;
	/// <summary> true to show, false to hide </summary>
	bool _visible;
	/// <summary> The click action </summary>
	ClickAction _clickAction;
	/// <summary> The click action parameter, e.g. the TCL procedure name. </summary>
	std::string _clickActionParam;
};

#endif
