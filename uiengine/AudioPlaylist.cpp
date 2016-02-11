#include "AudioPlaylist.h"
#include <Logger.h>

AudioPlaylist::AudioPlaylist(void)
{
	_curIndex = 0;
	_nextIndex = 0;
}

AudioPlaylist::~AudioPlaylist(void)
{
}

void AudioPlaylist::Clear()
{
	LOG( Logger::LOG_DEBUG, "AudioPlaylist::Clear()");
	_playlist.clear();
	_curIndex = 0;
	_nextIndex = 0;
}

void AudioPlaylist::SetRepetition( bool repeat )
{
	_repeat = repeat;
}

bool AudioPlaylist::Add( const char* filename, int skipMillis, int durationMillis )
{
	LOG( Logger::LOG_DEBUG, "AudioPlaylist::Add(%s, %d, %d)", filename, skipMillis, durationMillis);
	_playlist.push_back( PlaylistEntry( filename, skipMillis, durationMillis ) );
	return true;
}

const std::string& AudioPlaylist::GetCurrentFilename()
{
	static std::string emptyString;
	if( _curIndex >= _playlist.size() )return emptyString;
	return _playlist[_curIndex].Filename;
}

int AudioPlaylist::GetCurrentSkipMillis()
{
	if( _curIndex >= _playlist.size() )return -1;
	return _playlist[_curIndex].SkipMillis;
}

int AudioPlaylist::GetCurrentDurationMillis()
{
	if( _curIndex >= _playlist.size() )return -1;
	return _playlist[_curIndex].DurationMillis;
}

bool AudioPlaylist::Next()
{
	_curIndex = _nextIndex;
	_nextIndex++;
	if( _curIndex >= _playlist.size() )
	{
		if( _repeat )
		{
			_curIndex = 0;
			_nextIndex = 1;
		}else{
			Clear();
			return false;
		}
	}
	return true;
}
