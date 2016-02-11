#include "UiPage.h"
#include "UiManager.h"
#include "UiElement.h"
#include <Logger.h>
#include "dynamic.h"

UiPage::UiPage(UiManager* manager) : _manager(manager), _scriptingEngine(this)
{
	_defaultFontHeight = 0;
	_backgroundColor = 0xffffff;
}

UiPage::~UiPage(void)
{
	ClearUiElements();
}

UiManager* UiPage::GetManager()
{
	return _manager;
}

void UiPage::Invalidate()
{
	_manager->Invalidate();
}

void UiPage::ClearUiElements()
{
	for( t_uiElements::iterator it = _uiElements.begin(); it != _uiElements.end(); it++ )
	{
		delete *it;
	}
	_uiElements.clear();
}

UiElement* UiPage::GetUiElement( const std::string& id )
{
	for( t_uiElements::iterator it = _uiElements.begin(); it != _uiElements.end(); it++ )
	{
		if( (*it)->GetId() == id )return *it;
	}
	return NULL;
}

bool UiPage::InitFromXml( XMLNode& rootNode )
{
	ClearUiElements();
	const char* temp = rootNode.getAttribute("id");
	if(!temp)
	{
		LOG(Logger::LOG_ERROR, "<page> missing id attribute");
		return false;
	}
	_id = temp;

	temp = rootNode.getAttribute("onload");
	if(temp)
	{
		_onload = temp;
	}

	temp = rootNode.getAttribute("font");
	if(temp)
	{
		_defaultFontFace = temp;
	}

	temp = rootNode.getAttribute("font_height");
	if(temp)
	{
		_defaultFontHeight = strtol(temp, NULL, 0);
	}

	temp = rootNode.getAttribute("waitcursor");
	if(temp)
	{
		_waitCursor = temp;
	}

	temp = rootNode.getAttribute("onAudioStart");
	if(temp)
	{
		_audioEventHandlers["start"] = temp;
	}

	temp = rootNode.getAttribute("onAudioProgress");
	if(temp)
	{
		_audioEventHandlers["progress"] = temp;
	}

	temp = rootNode.getAttribute("onAudioStop");
	if(temp)
	{
		_audioEventHandlers["stop"] = temp;
	}


	XMLNode elementsNode = rootNode.getChildNode("elements");
	for( int i=0; i<elementsNode.nChildNode(); i++ )
	{
		XMLNode elementNode = elementsNode.getChildNode(i);
		UiElement* uiElement = dynamic_cast<UiElement*>((UiElement*)dynamic::create( elementNode.getName() ));
		if( !uiElement )
		{
			LOG( Logger::LOG_ERROR, "Could not instantiate class for element %s", elementNode.getName() );
			return false;
		}
		if( !uiElement->Init(this) || !uiElement->InitFromXml( elementNode ) )
		{
			delete uiElement;
			return false;
		}
		//LOG( Logger::LOG_DEBUG, "UI element created");
		_uiElements.push_back( uiElement );
	}

	XMLNode triggersNode = rootNode.getChildNode("triggers");
	for( int i=0; i<triggersNode.nChildNode(); i++ )
	{
		XMLNode triggerNode = triggersNode.getChildNode(i);
		temp = triggerNode.getAttribute("value");
		if(!temp)
		{
			LOG( Logger::LOG_ERROR, "<trigger> missing \"value\" attribute");
			return false;
		}
		std::string valueId = temp;
		temp = triggerNode.getAttribute("method");
		if(!temp)
		{
			LOG( Logger::LOG_ERROR, "<trigger value=\"%s\"> missing \"method\" attribute", valueId.c_str());
			return false;
		}
		//LOG( Logger::LOG_DEBUG, "Trigger %s=%s", valueId.c_str(), temp);
		AddTrigger(valueId, temp, false, false);

		temp = triggerNode.getAttribute("global");
		if(temp && temp[0]=='t')
		{
			GetManager()->RegisterGlobalTrigger( valueId, this );
		}

	}

	XMLNode scriptNode = rootNode.getChildNode("code");
	if( !scriptNode.isEmpty() )
	{
		if( !_scriptingEngine.InitFromXml(scriptNode) )
		{
			return false;
		}
	}
	return true;
}

void UiPage::OnLoad(LoadReason loadReason, UiPage* lastPage)
{
	RemoveDynamicTriggers();
	RegisterValueTriggers();
	if( !_onload.empty() )
	{
		std::string loadReasonString;
		switch( loadReason )
		{
		case LoadReasonShow:
			loadReasonString = "SHOW";
			break;
		case LoadReasonBack:
			loadReasonString = "BACK";
			break;
		}
		_scriptingEngine.CallPageCommand( _onload, lastPage, loadReasonString );
	}
}

void UiPage::Render(IDirectFBSurface* sfc)
{
	DFBResult err = sfc->Clear (sfc, (_backgroundColor>>16)&0xff, (_backgroundColor>>8)&0xff, (_backgroundColor>>0)&0xff, 0xff);
	if( err != DFB_OK )
	{
		LOG( Logger::LOG_ERROR, "DFB error %d", err );
		return;
	}

	for( t_uiElements::iterator it = _uiElements.begin(); it != _uiElements.end(); it++ )
	{
		if((*it)->IsVisible())(*it)->Render();
	}
}

void UiPage::HandlePress( int x, int y )
{
	for( t_uiElements::iterator it = _uiElements.begin(); it != _uiElements.end(); it++ )
	{
		(*it)->HandlePress(x, y);
	}
}

void UiPage::CodeEvent(UiElement* sender, const std::string& evt)
{
	LOG(Logger::LOG_DEBUG, "Event %s from %s", evt.c_str(), sender->GetId().c_str());
	_manager->ShowWaitCursor();
	_scriptingEngine.CallElementCommand(evt, sender);
}

const std::string& UiPage::GetId()
{
	return _id;
}

IDirectFBFont* UiPage::GetFont( std::string face, int height )
{
	if( face.empty() )face = _defaultFontFace;
	if( !height )height = _defaultFontHeight;
	return _manager->GetFont( face, height);
}

void UiPage::HandleValueTrigger( const std::string id, unsigned long value )
{
	LOG(Logger::LOG_DEBUG, "HandleValueTrigger( %s, %lu )", id.c_str(), value);
	t_triggers::iterator it = _triggers.find( id );
	if( it == _triggers.end() )return;
	std::vector<std::string> commands;
	commands.push_back( it->second.method );
	commands.push_back( it->first );
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%lu", value );
	commands.push_back( buffer );
	_scriptingEngine.CallCommand( commands );
}

void UiPage::HandleAudioEvent( const std::string id, const std::vector<std::string>& params )
{
	t_audioEventHandlers::iterator it = _audioEventHandlers.find( id );
	if( it == _audioEventHandlers.end() )return;
	std::vector<std::string> commands;
	commands.resize( params.size() + 2 );
	commands[0] = it->second;
	commands[1] = it->first;
	std::copy( params.begin(), params.end(), commands.begin() + 2 );
	_scriptingEngine.CallCommand( commands );
}

void UiPage::RegisterValueTriggers()
{
	for(t_triggers::iterator it = _triggers.begin(); it != _triggers.end(); it++)
	{
		_manager->GetIcClient()->RegisterChannel( it->first, false );
	}
}

void UiPage::AddTrigger(const std::string& controlValueId, const std::string& method, bool dynamic, bool registerChannel)
{
	t_triggers::iterator it = _triggers.find(controlValueId);
	if( it == _triggers.end() )
	{
		t_trigger& trigger = _triggers[controlValueId];
		trigger.method = method;
		trigger.dynamic = dynamic;
	}
	if(registerChannel)_manager->GetIcClient()->RegisterChannel( controlValueId, false );
}

void UiPage::RemoveDynamicTriggers()
{
	std::list<std::string> removeList;
	for(t_triggers::iterator it = _triggers.begin(); it != _triggers.end(); it++)
	{
		if( it->second.dynamic )
		{
			removeList.push_back(it->first);
		}
	}
	for(std::list<std::string>::iterator removeIt=removeList.begin(); removeIt!=removeList.end(); removeIt++)
	{
		_triggers.erase(*removeIt);
	}
}

const std::string& UiPage::GetWaitCursor()
{
	return _waitCursor;
}
