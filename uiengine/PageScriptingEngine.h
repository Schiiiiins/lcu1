// HSSTypeConversionScript.h: Schnittstelle für die Klasse HSSTypeConversionScript.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <XmlRpc.h>
#include <tcl.h>
#include <xmlParser.h>

#ifndef CONST84
#define CONST84
#endif

class UiPage;
class UiElement;

class PageScriptingEngine
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="page">	[in] The associated UI page object. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	PageScriptingEngine( UiPage* page );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Finaliser. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual ~PageScriptingEngine();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initializes this object from an XML description. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="node">	[in] The XML node. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool InitFromXml(XMLNode &node);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Call a TCL command associated with a UI page. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="command">	The TCL procedure name. </param>
	/// <param name="sender"> 	[in] The sending page. </param>
	/// <param name="param">  	The single parameter. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void CallPageCommand( const std::string& command, UiPage* sender, const std::string& param);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Call a TCL command associated with a UI element. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="command">	The TCL procedure name. </param>
	/// <param name="sender"> 	[in] The sendinf element. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void CallElementCommand( const std::string& command, UiElement* sender=NULL);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Call a TCL command. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="commands">	The TCL procedure name followed by the positional arguments. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void CallCommand( const std::vector<std::string>& commands );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Defines an alias representing an associative array. . </summary>
	///
	/// <remarks>	Lars Reemts, 05.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	typedef std::map<std::string, std::string> ArrayType;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// Converts an associative array to a TCL list. The list can be converted back to an array using
	/// the TCL command "array set".
	/// </summary>
	///
	/// <remarks>	Lars Reemts, 05.05.2011. </remarks>
	///
	/// <param name="arr"> 	The array. </param>
	/// <param name="list">	[out] The list. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static bool ArrayToList( const ArrayType& arr, std::string* list );

protected:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Activates the associated TCL namespace. </summary>
	///
	/// <remarks>	Lars Reemts, 23.05.2011. </remarks>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool ActivateNamespace();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Deactivates the associated TCL namespace. </summary>
	///
	/// <remarks>	Lars Reemts, 23.05.2011. </remarks>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool DeactivateNamespace();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Implementation of the TCL command <c>ui</c> </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="cd">	 	Pointer to the PageScriptingEngine object. </param>
	/// <param name="interp">	[in] The TCL interpreter. </param>
	/// <param name="argc">  	The argument count. </param>
	/// <param name="argv">  	The arguments. </param>
	///
	/// <returns>	<c>TCL_OK</c> or <c>TCL_ERROR</c> </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static int Tcl_CmdUi (ClientData cd, Tcl_Interp * interp, int argc, CONST84 char* argv[]);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Implementation of the TCL command <c>control</c> </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="cd">	 	Pointer to the PageScriptingEngine object. </param>
	/// <param name="interp">	[in] The TCL interpreter. </param>
	/// <param name="argc">  	The argument count. </param>
	/// <param name="argv">  	The arguments. </param>
	///
	/// <returns>	<c>TCL_OK</c> or <c>TCL_ERROR</c> </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	static int Tcl_CmdControlValue (ClientData cd, Tcl_Interp * interp, int argc, CONST84 char* argv[]);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Implementation of the TCL command <c>audio</c> </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="cd">	 	Pointer to the PageScriptingEngine object. </param>
	/// <param name="interp">	[in] The TCL interpreter. </param>
	/// <param name="argc">  	The argument count. </param>
	/// <param name="argv">  	The arguments. </param>
	///
	/// <returns>	<c>TCL_OK</c> or <c>TCL_ERROR</c> </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static int Tcl_CmdAudio (ClientData cd, Tcl_Interp * audio, int argc, CONST84 char* argv[]);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Implementation of the TCL command <c>backlight</c> </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="cd">	 	Pointer to the PageScriptingEngine object. </param>
	/// <param name="interp">	[in] The TCL interpreter. </param>
	/// <param name="argc">  	The argument count. </param>
	/// <param name="argv">  	The arguments. </param>
	///
	/// <returns>	<c>TCL_OK</c> or <c>TCL_ERROR</c> </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////
	static int Tcl_CmdBacklight (ClientData cd, Tcl_Interp * audio, int argc, CONST84 char* argv[]);

	/// <summary> Instance of the global TCL interpreter </summary>
	static Tcl_Interp * s_interp;
	static int s_interpRefCount;
	/// <summary> The associated UI page </summary>
	UiPage* _page;
	/// <summary> The TCL namespace associated with the page </summary>
	Tcl_Namespace* _namespace;
	/// <summary> The TCL call frame used while executing commands </summary>
	Tcl_CallFrame _callFrame;
	/// <summary> Constant for the <c>ui</c> command </summary>
    static const char TCL_COMMAND_NAME_UI[];
	/// <summary> Constant for the <c>control</c> command </summary>
    static const char TCL_COMMAND_NAME_CONTROL_VALUE[];
	/// <summary> Constant for the <c>audio</c> command </summary>
    static const char TCL_COMMAND_NAME_AUDIO[];
	/// <summary> Constant for the <c>backlight</c> command </summary>
    static const char TCL_COMMAND_NAME_BACKLIGHT[];
};

