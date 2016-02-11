#include "InputManager.h"
#include "UiManager.h"
#include <Logger.h>

InputManager::InputManager(UiManager* uiManager):_uiManager(uiManager)
{
	_flags = 0;
	_penX = 0;
	_penY = 0;
}

InputManager::~InputManager(void)
{
}

void InputManager::HandleInputEvent( DFBInputEvent& evt )
{
	if( evt.type == DIET_BUTTONPRESS )
	{
		_flags = FlagPendown | FlagPendownPending;
	}else if( evt.type == DIET_BUTTONRELEASE )
	{
		if( _flags & FlagPendown )
		{
			if( !(_flags & FlagPendownPending) )
			{
				LOG( Logger::LOG_DEBUG, "Pen release, (%d,%d)", _penX, _penY );
			}
		}
		_flags = 0;
	}else if( evt.type == DIET_AXISMOTION )
	{
		switch( evt.axis )
		{
		case DIAI_X:
			_penX = evt.axisabs;
			_flags |= FlagXreceived;
			//LOG( Logger::LOG_DEBUG, "X %d", evt.axisabs );
			break;
		case DIAI_Y:
			_penY = evt.axisabs;
			_flags |= FlagYreceived;
			//LOG( Logger::LOG_DEBUG, "Y %d", evt.axisabs );
			break;
		default:
			break;
		}
		if( (_flags & (FlagXreceived | FlagYreceived | FlagPendownPending)) == (FlagXreceived | FlagYreceived | FlagPendownPending) )
		{
			LOG( Logger::LOG_DEBUG, "Pen press, (%d,%d)", _penX, _penY );
			_uiManager->HandlePress(_penX, _penY );
			_flags &= ~FlagPendownPending;
		}
	}
}

