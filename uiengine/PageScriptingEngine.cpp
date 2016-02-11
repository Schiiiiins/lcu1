// PageScriptingEngine.cpp: Implementierung der Klasse PageScriptingEngine.
//
//////////////////////////////////////////////////////////////////////

#include "PageScriptingEngine.h"
#include "UiPage.h"
#include "UiText.h"
#include "UiManager.h"
#include <Logger.h>
#include <cstring>
#include <typeinfo>
#include <tclInt.h>

const static char* USAGE_UI =		"usage: ui get controlId\n"
									"       ui set controlId text\n"
									"       ui load page\n"
									"       ui back\n"
									"		ui basedir\n";

const static char* USAGE_BACKLIGHT =	"usage: backlight range [min max]\n"
										"       backlight time [seconds]\n";

const static char* USAGE_CONTROL_VALUE =	"usage: control get valueId\n"
											"       control set valueId value\n"
											"		control trigger valueId handlerProcedure\n";

const static char* USAGE_AUDIO =	"usage: audio volume <channel> [<volume>]\n"
									"       audio switch <channel> [<on/off/1/0>]\n"
									"       audio play [-add] [-skip <millis>] [-duration <millis>] [-repeat] <mp3_file>\n"
									"       audio stop\n"
									"       audio info\n";


using namespace XmlRpc;

/*static*/ const char PageScriptingEngine::TCL_COMMAND_NAME_UI[]="ui";
/*static*/ const char PageScriptingEngine::TCL_COMMAND_NAME_CONTROL_VALUE[]="control";
/*static*/ const char PageScriptingEngine::TCL_COMMAND_NAME_AUDIO[]="audio";
/*static*/ const char PageScriptingEngine::TCL_COMMAND_NAME_BACKLIGHT[]="backlight";

/*static*/ Tcl_Interp * PageScriptingEngine::s_interp = NULL;
/*static*/ int PageScriptingEngine::s_interpRefCount = 0;

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

PageScriptingEngine::PageScriptingEngine(UiPage* page)
{
	if( !s_interp )
	{
		s_interp=Tcl_CreateInterp();
		Tcl_CreateCommand(s_interp, (char*)TCL_COMMAND_NAME_UI, Tcl_CmdUi, (ClientData) page->GetManager(), NULL);
		Tcl_CreateCommand(s_interp, (char*)TCL_COMMAND_NAME_CONTROL_VALUE, Tcl_CmdControlValue, (ClientData) page->GetManager(), NULL);
		Tcl_CreateCommand(s_interp, (char*)TCL_COMMAND_NAME_AUDIO, Tcl_CmdAudio, (ClientData) page->GetManager(), NULL);
		Tcl_CreateCommand(s_interp, (char*)TCL_COMMAND_NAME_BACKLIGHT, Tcl_CmdBacklight, (ClientData) page->GetManager(), NULL);
	}
	s_interpRefCount++;
	_page=page;
	_namespace = NULL;
}

PageScriptingEngine::~PageScriptingEngine()
{
	if( !(--s_interpRefCount) )
	{
		Tcl_DeleteInterp(s_interp);
		s_interp = NULL;
	}
}

bool PageScriptingEngine::InitFromXml(XMLNode &node)
{
	Tcl_Obj* script = Tcl_NewObj();
	const char* text=node.getText();
	std::string scriptText;
	if(text){
		//LOG(Logger::LOG_DEBUG, "Script:%s", text);
		Tcl_AppendToObj(script, (char*)text, -1);
		scriptText += text;
	}
	for(int j=0;j<node.nClear();j++){
		text=node.getClear(j).lpszValue;
		if(text){
			//LOG(Logger::LOG_DEBUG, "Script:%s", text);
			Tcl_AppendToObj(script, (char*)text, -1);
			scriptText += text;
		}
	}

	// script will be deleted by Tcl_EvalObjEx decrementing the reference count
	if( !ActivateNamespace() )return false;
	int result = Tcl_EvalObjEx(s_interp, script, TCL_EVAL_DIRECT);
    if (result == TCL_ERROR) {
		LOG(Logger::LOG_ERROR, "Script error: %s, script line %d\n%s", Tcl_GetStringResult(s_interp), s_interp->errorLine, scriptText.c_str());
    }
	DeactivateNamespace();
	return result != TCL_ERROR;
}

void PageScriptingEngine::CallPageCommand( const std::string& command, UiPage* sender, const std::string& param)
{
	std::vector<std::string> commands;
	commands.push_back(command);
	if( sender )
	{
		commands.push_back( sender->GetId() );
	}else{
		commands.push_back( "" );
	}
	commands.push_back( param );
	CallCommand( commands );
}

void PageScriptingEngine::CallElementCommand( const std::string& command, UiElement* sender/*=NULL*/)
{
	std::vector<std::string> commands;
	commands.push_back(command);
	if( sender )
	{
		commands.push_back( sender->GetId() );
	}
	CallCommand( commands );
}

void PageScriptingEngine::CallCommand( const std::vector<std::string>& commands )
{
	Tcl_Obj* tclCommand[commands.size()];
	for( unsigned int i=0; i<commands.size(); i++)
	{
		tclCommand[i] = Tcl_NewStringObj(commands[i].c_str(), -1);
	}

	if( ActivateNamespace() )
	{
		int code = Tcl_EvalObjv(s_interp, commands.size(), tclCommand, TCL_EVAL_DIRECT);
		if(code!=TCL_OK){
			std::string command;
			for( unsigned int i=0; i<commands.size(); i++)
			{
				if( command.size() )command += " ";
				command += commands[i];
			}
			LOG(Logger::LOG_ERROR, "Event %s error: %s", command.c_str(), Tcl_GetStringResult(s_interp));
		}
		DeactivateNamespace();
	}
}

bool PageScriptingEngine::ActivateNamespace()
{
    /*
     * Create our namespace if it doesn't exist
     */
    
	if( !_namespace )
	{
		_namespace = Tcl_CreateNamespace(s_interp, const_cast<char*>(_page->GetId().c_str()), (ClientData) NULL, (Tcl_NamespaceDeleteProc *) NULL);
		if (_namespace == NULL)
		{
			LOG(Logger::LOG_ERROR, "Error creating TCL namespace %s", _page->GetId().c_str());
			return false;
		}
	}

    /*
     * Make the specified namespace the current namespace and evaluate
     * the command(s).
     */

    int result = Tcl_PushCallFrame(s_interp, &_callFrame, _namespace,
	    /*isProcCallFrame*/ 0);
    if (result != TCL_OK)
	{
		LOG(Logger::LOG_ERROR, "Error activating TCL namespace %s", _page->GetId().c_str());
		return false;
	}
	return true;
}

bool PageScriptingEngine::DeactivateNamespace()
{
    /*
     * Restore the previous "current" namespace.
     */
    
    Tcl_PopCallFrame(s_interp);
	return true;
}

int PageScriptingEngine::Tcl_CmdUi(ClientData cd, Tcl_Interp * interp, int argc, CONST84 char* argv[])
{
	UiManager* manager=(UiManager*)cd;
	UiPage* page=manager->GetCurrentPage();

	if(argc < 2 || !argv[1][0]) {
		Tcl_AppendResult(interp, USAGE_UI, NULL);
		return TCL_ERROR;
	}
	if(strcmp(argv[1]+1, "et")==0){
		if(argc < 3) {
			Tcl_AppendResult(interp, USAGE_UI, NULL);
			return TCL_ERROR;
		}
		std::string elementSpecification = argv[2];
		std::string::size_type dotpos = elementSpecification.rfind('.');
		if( dotpos == std::string::npos )
		{
			Tcl_AppendResult(interp, "Could not resolve element.property in ", elementSpecification.c_str(), NULL);
			return TCL_ERROR;
		}
		std::string propertyId = elementSpecification.substr( dotpos+1 );
		elementSpecification = elementSpecification.substr(0, dotpos);
		std::string elementId;
		dotpos = elementSpecification.rfind('.');
		if( dotpos != std::string::npos )
		{
			elementId = elementSpecification.substr(dotpos+1);
			std::string pageId = elementSpecification.substr(0, dotpos);
			page = manager->GetPage(pageId);
			if(!page)
			{
				Tcl_AppendResult(interp, "Could not resolve page ", pageId.c_str(), " in ", argv[2], NULL);
				return TCL_ERROR;
			}
		}else{
			elementId = elementSpecification;
		}

		UiElement* uiElement = page->GetUiElement( elementId );
		if( !uiElement )
		{
			Tcl_AppendResult(interp, "Unknown UI element ", page->GetId().c_str(), ".", elementId.c_str(), NULL);
			return TCL_ERROR;
		}
		if(strcmp(argv[1], "get")==0){
			std::string propertyValue;
			if( !uiElement->GetProperty( propertyId, &propertyValue ) )
			{
				Tcl_AppendResult(interp, "Unknown property ", propertyId.c_str(), " on element ", elementId.c_str(), NULL);
				return TCL_ERROR;
			}
			Tcl_SetResult(interp, const_cast<char*>(propertyValue.c_str()), TCL_VOLATILE);
		}else if(strcmp(argv[1], "set")==0){
			if(argc < 4) {
				Tcl_AppendResult(interp, USAGE_UI, NULL);
				return TCL_ERROR;
			}
			if( !uiElement->SetProperty( propertyId, argv[3] ) )
			{
				Tcl_AppendResult(interp, "Could not set property ", propertyId.c_str(), " on element ", elementId.c_str(), " to ", argv[3], NULL);
				return TCL_ERROR;
			}
		}else{
			Tcl_AppendResult(interp, USAGE_UI, NULL);
			return TCL_ERROR;
		}
	}else if(strcmp(argv[1], "load")==0){
		if(argc < 3) {
			Tcl_AppendResult(interp, USAGE_UI, NULL);
			return TCL_ERROR;
		}
		if( !manager->ShowPage(argv[2]))
		{
			Tcl_AppendResult(interp, "Error loading page ", argv[2], NULL);
			return TCL_ERROR;
		}
	}else if(strcmp(argv[1], "back")==0){
		if( !manager->Back() )
		{
			Tcl_AppendResult(interp, "Error loading last page", NULL);
			return TCL_ERROR;
		}
	}else if(strcmp(argv[1], "basedir")==0){
		std::string basedir = manager->GetAbsoluteFilename("");
		Tcl_AppendResult(interp, basedir.c_str(), NULL);
	}else{
		Tcl_AppendResult(interp, USAGE_UI, NULL);
		return TCL_ERROR;
	}
	return TCL_OK;
}

int PageScriptingEngine::Tcl_CmdControlValue(ClientData cd, Tcl_Interp * interp, int argc, CONST84 char* argv[])
{
	UiManager* manager=(UiManager*)cd;
	UiPage* page=manager->GetCurrentPage();
	if(argc < 3) {
		Tcl_AppendResult(interp, USAGE_CONTROL_VALUE, NULL);
		return TCL_ERROR;
	}
	if(strcmp(argv[1], "get")==0){
		unsigned long value = manager->GetIcClient()->GetValue(argv[2]);
		char buffer[32];
		snprintf(buffer, 32, "%ld", value);
		Tcl_SetResult(interp, buffer, TCL_VOLATILE);
		return TCL_OK;
	}else if(strcmp(argv[1], "set")==0){
		if(argc < 4) {
			Tcl_AppendResult(interp, USAGE_CONTROL_VALUE, NULL);
			return TCL_ERROR;
		}
		char* endp;
		errno = 0;
		unsigned long ulValue = strtol( argv[3], &endp, 0 );
		if( errno || *endp )
		{
			Tcl_AppendResult(interp, "Error parsing ", argv[3], " to an unsigned long", NULL);
			return TCL_ERROR;
		}
		if( !manager->GetIcClient()->SetValue( argv[2], ulValue ) )
		{
			Tcl_AppendResult(interp, "Error setting value ", argv[2], NULL);
			return TCL_ERROR;
		}
		return TCL_OK;
	}else if(strcmp(argv[1], "trigger")==0){
		if(argc < 4) {
			Tcl_AppendResult(interp, USAGE_CONTROL_VALUE, NULL);
			return TCL_ERROR;
		}
		page->AddTrigger( argv[2], argv[3], true, true );
	}else{
		Tcl_AppendResult(interp, USAGE_CONTROL_VALUE, NULL);
		return TCL_ERROR;
	}
	return TCL_OK;
}

int PageScriptingEngine::Tcl_CmdAudio(ClientData cd, Tcl_Interp * interp, int argc, CONST84 char* argv[])
{
	UiManager* manager=(UiManager*)cd;
	if(argc < 2) {
		Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
		return TCL_ERROR;
	}
	if(strcmp(argv[1], "volume")==0){
		if(argc < 3) {
			Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
			return TCL_ERROR;
		}
		if( argc >= 4 )
		{
			if( strcmp(argv[3], "up")==0 || strcmp(argv[3], "down")==0 )
			{
				if( !manager->GetAudioManager()->GetMixer()->StepVolume( argv[2], argv[3][0] == 'u' ? 1 : -1 ) )
				{
					Tcl_AppendResult(interp, "Error stepping audio volume for channel ", argv[2], NULL);
					return TCL_ERROR;
				}
			}else{
				char* endp;
				errno = 0;
				int percent = strtol( argv[3], &endp, 0 );
				if( errno || *endp )
				{
					Tcl_AppendResult(interp, "Error parsing ", argv[3], " to an integer", NULL);
					return TCL_ERROR;
				}
				if( percent < 0 || percent > 100 )
				{
					Tcl_AppendResult(interp, "Volume must be in range 0-100", NULL);
					return TCL_ERROR;
				}
				if( !manager->GetAudioManager()->GetMixer()->SetVolumePercent( argv[2], percent ) )
				{
					Tcl_AppendResult(interp, "Error setting audio volume for channel ", argv[2], " to ", argv[3], "%", NULL);
					return TCL_ERROR;
				}
			}
		}
		int percent;
		if( !manager->GetAudioManager()->GetMixer()->GetVolumePercent( argv[2], &percent ) )
		{
			Tcl_AppendResult(interp, "Error getting audio volume for channel ", argv[2], "%", NULL);
			return TCL_ERROR;
		}
		char buffer[32];
		sprintf( buffer, "%d", percent );
		Tcl_AppendResult( interp, buffer, NULL );
	}else if(strcmp(argv[1], "switch")==0){
		if(argc < 3) {
			Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
			return TCL_ERROR;
		}
		if( argc >= 4 )
		{
			bool on;
			if( argv[2][0] == 'o' )
			{
				if( strcmp(argv[2], "on")==0 )on=1;
				else if( strcmp(argv[2], "off")==0 )on=0;
				else 
				{
					Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
					return TCL_ERROR;
				}
			}else{
				char* endp;
				errno = 0;
				int iValue = strtol( argv[3], &endp, 0 );
				if( errno || *endp )
				{
					Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
					return TCL_ERROR;
				}
				on = iValue != 0;
				if( !manager->GetAudioManager()->GetMixer()->SwitchChannel( argv[2], on ) )
				{
					Tcl_AppendResult(interp, "Error switching audio channel ", argv[2], on?"on":"off", NULL);
					return TCL_ERROR;
				}
			}
		}
		bool on;
		if( !manager->GetAudioManager()->GetMixer()->GetChannelSwitch( argv[2], &on ) )
		{
			Tcl_AppendResult(interp, "Error getting switch status for audio channel ", argv[2], NULL);
			return TCL_ERROR;
		}
		Tcl_AppendResult( interp, on ? "1" : "0", NULL );
	}else if(strcmp(argv[1], "play")==0){
		if(argc < 3) {
			Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
			return TCL_ERROR;
		}
		bool add = false;
		bool repeat = false;
		int skipMillis = 0;
		int durationMillis = 0;
		for( int i=2; i<argc-1; i++ )
		{
			if( strcmp( argv[i], "-add" ) == 0 )
			{
				add = true;
			}else if( strcmp( argv[i], "-repeat" ) == 0 )
			{
				repeat = true;
			}else if( strcmp( argv[i], "-skip" ) == 0 && (i < argc-2) )
			{
				i++;
				char* endp;
				errno = 0;
				skipMillis = strtol( argv[i], &endp, 0 );
				if( errno || *endp )
				{
					Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
					return TCL_ERROR;
				}
			}else if( strcmp( argv[i], "-duration" ) == 0 && (i < argc-2) )
			{
				i++;
				char* endp;
				errno = 0;
				durationMillis = strtol( argv[i], &endp, 0 );
				if( errno || *endp )
				{
					Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
					return TCL_ERROR;
				}
			}else{
				Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
				return TCL_ERROR;
			}
		}
		if( !add ) manager->GetAudioManager()->ClearPlaylist();
		manager->GetAudioManager()->SetPlaylistRepetition( repeat );
		if( !manager->GetAudioManager()->AddToPlaylist( argv[argc-1], skipMillis, durationMillis ) )
		{
			Tcl_AppendResult(interp, "Error adding file ", argv[argc-1], " to audio playlist", NULL);
			return TCL_ERROR;
		}
	}else if(strcmp(argv[1], "stop")==0){
		manager->GetAudioManager()->ClearPlaylist();
	}else if(strcmp(argv[1], "info")==0){
		ArrayType info = manager->GetAudioManager()->GetInfo();
		std::string list;
		if( !ArrayToList( info, &list ) )
		{
			Tcl_AppendResult(interp, "Error getting MP3 info", NULL);
			return TCL_ERROR;
		}
		Tcl_AppendResult(interp, list.c_str(), NULL);
	}else{
		Tcl_AppendResult(interp, USAGE_AUDIO, NULL);
		return TCL_ERROR;
	}
	return TCL_OK;
}

int PageScriptingEngine::Tcl_CmdBacklight(ClientData cd, Tcl_Interp * interp, int argc, CONST84 char* argv[])
{
	UiManager* manager=(UiManager*)cd;
	if(argc < 2) {
		Tcl_AppendResult(interp, USAGE_BACKLIGHT, NULL);
		return TCL_ERROR;
	}
	if(strcmp(argv[1], "range")==0){
		if(argc == 3) {
			Tcl_AppendResult(interp, USAGE_BACKLIGHT, NULL);
			return TCL_ERROR;
		}
		if( argc >= 4 )
		{
			char* endp;
			errno = 0;
			int minBrightness = strtol( argv[2], &endp, 0 );
			if( errno || *endp )
			{
				Tcl_AppendResult(interp, "Error parsing ", argv[2], " to an integer", NULL);
				return TCL_ERROR;
			}

			int maxBrightness = strtol( argv[3], &endp, 0 );
			if( errno || *endp )
			{
				Tcl_AppendResult(interp, "Error parsing ", argv[3], " to an integer", NULL);
				return TCL_ERROR;
			}
			
			manager->GetBacklight()->SetRange( minBrightness, maxBrightness );
		}
		int minBrightness, maxBrightness;

		manager->GetBacklight()->GetRange( &minBrightness, &maxBrightness );

		char* valp[2];
		char bufferMin[8];
		char bufferMax[8];
		valp[0] = bufferMin;
		valp[1] = bufferMax;
		sprintf( bufferMin, "%d", minBrightness );
		sprintf( bufferMax, "%d", maxBrightness );

		char* tcl_str=Tcl_Merge(2, valp);
		if(tcl_str){
			Tcl_AppendResult( interp, tcl_str, NULL );
			Tcl_Free(tcl_str);
		}
	}else if(strcmp(argv[1], "time")==0){
		if( argc >= 3 )
		{
			char* endp;
			errno = 0;
			int timeoutSeconds = strtol( argv[2], &endp, 0 );
			if( errno || *endp )
			{
				Tcl_AppendResult(interp, "Error parsing ", argv[2], " to an integer", NULL);
				return TCL_ERROR;
			}

			manager->GetBacklight()->SetTimeout( timeoutSeconds );
		}

		int timeoutSeconds = manager->GetBacklight()->GetTimeout();
		char buffer[32];
		sprintf( buffer, "%d", timeoutSeconds );
		Tcl_AppendResult( interp, buffer, NULL );
	}else{
		Tcl_AppendResult(interp, USAGE_BACKLIGHT, NULL);
		return TCL_ERROR;
	}
	return TCL_OK;
}

/*static*/ bool PageScriptingEngine::ArrayToList( const ArrayType &arr, std::string* l )
{
	bool success = false;
	ArrayType::const_iterator it;

    int s = arr.size() * 2;
    char** valp=new char*[s];
    memset(valp, 0, s * sizeof(char*));
	int i=0;
	for( it=arr.begin(); it != arr.end(); it++ )
	{
        valp[i]=new char[it->first.size()+1];
        strcpy(valp[i], it->first.c_str());
		i++;
        valp[i]=new char[it->second.size()+1];
        strcpy(valp[i], it->second.c_str());
		i++;
    }
    char* tcl_str=Tcl_Merge(s, valp);
	std::string result;
    if(tcl_str){
		*l = tcl_str;
		success = true;
	    Tcl_Free(tcl_str);
	}
    for (int i=0; i<s; ++i)
    {
        if(valp[i])delete[] valp[i];
    }
    delete[] valp;
	return success;
}
