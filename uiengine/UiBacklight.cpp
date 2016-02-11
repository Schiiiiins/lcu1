#include "UiBacklight.h"
#include <fstream>
#include <Logger.h>

#define BRIGHTNESS_FILE "/sys/class/backlight/backlight/brightness"


UiBacklight::UiBacklight(void)
{
	_isOn = false;
    _currentBrightness = 0;
    _maxBrightness = 255;
    _minBrightness = 0;
    _lastTriggerTime = time(NULL);
	_backlightTimeout = 120;
}

UiBacklight::~UiBacklight(void)
{
}

bool UiBacklight::Trigger()
{
	KillTimer( TimerSwitchOff );
	if( _backlightTimeout )RequestTimer( _backlightTimeout * 1000, TimerSwitchOff );
	bool wasOn = _isOn;
	if( !_isOn )Switch( true );
    _lastTriggerTime = time(NULL);
	return wasOn;
}

void UiBacklight::OnTimer( unsigned long cookie )
{
    switch( cookie )
    {
        case TimerSwitchOff:
            if( (time(NULL) - _backlightTimeout - _lastTriggerTime) > 10 )
            {
                //looks like a time warp, retrigger
                Trigger();
            }else{
                Switch( false );
            }
        break;
        case TimerDimmDown:
            SetBrightness( _currentBrightness - DimmStep );
            if( _currentBrightness > _minBrightness )
            {
                RequestTimer( DimmInterval, TimerDimmDown );
            }
        break;
    }
}

void UiBacklight::Switch( bool on )
{
    if( on )
    {
        SetBrightness( _maxBrightness );
        KillTimer( TimerDimmDown );
    } else {
        RequestTimer( DimmInterval, TimerDimmDown );
    }
	_isOn = on;
}

void UiBacklight::SetBrightness( int brightness )
{
    if( brightness < _minBrightness )brightness = _minBrightness;
    if( brightness > _maxBrightness ) brightness=_maxBrightness;
	std::ofstream os(BRIGHTNESS_FILE, std::ios::trunc);
	if( os.good() )
	{
		os<<brightness;
	}
    _currentBrightness = brightness;
}

void UiBacklight::GetRange( int* minBrightness, int* maxBrightness )
{
	*minBrightness = _minBrightness;
	*maxBrightness = _maxBrightness;
}

void UiBacklight::SetRange( int minBrightness, int maxBrightness )
{
	LOG( Logger::LOG_DEBUG, "UiBacklight::SetRange( %d, %d )", minBrightness, maxBrightness);
	if( minBrightness < 0 )minBrightness = 0;
	if( minBrightness > 255 )minBrightness = 255;
	if( maxBrightness < minBrightness )maxBrightness = minBrightness;
	if( maxBrightness < 10 )maxBrightness = 10;
	if( maxBrightness > 255 )maxBrightness = 255;
	_minBrightness = minBrightness;
	_maxBrightness = maxBrightness;
	LOG( Logger::LOG_DEBUG, "adjusted range is ( %d, %d )", minBrightness, maxBrightness);
	SetBrightness( _maxBrightness );
}

int UiBacklight::GetTimeout()
{
	return _backlightTimeout;
}

void UiBacklight::SetTimeout( int timeout )
{
	LOG( Logger::LOG_DEBUG, "UiBacklight::SetTimeout( %d )", timeout);
	_backlightTimeout = timeout;
	Trigger();
}
