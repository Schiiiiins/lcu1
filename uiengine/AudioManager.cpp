#include "AudioManager.h"
#include <Logger.h>
#include "UiManager.h"

AudioManager::AudioManager(UiManager* uiManager) : _player( this ), _uiManager( uiManager )
{
	_card = "default";
}

AudioManager::~AudioManager(void)
{
}

bool AudioManager::Init()
{
	return _mixer.Init( _card.c_str() );
}

bool AudioManager::IsInitialized()
{
	return _mixer.IsInitialized();
}

AlsaMixer* AudioManager::GetMixer()
{
	return &_mixer;
}

int AudioManager::GetFd()
{
	return _player.GetFd();
}

void AudioManager::ClearPlaylist()
{
	_playlist.Clear();
	if( _player.IsPlaying() )_player.Stop();
}

void AudioManager::SetPlaylistRepetition( bool repeat )
{
	_playlist.SetRepetition( repeat );
}

bool AudioManager::AddToPlaylist( const char* filename, int skipMillis, int durationMillis )
{
	_playlist.Add( _uiManager->GetAbsoluteFilename(filename).c_str(), skipMillis, durationMillis );
	DoPendingWork();
	return true;
}

void AudioManager::DoPendingWork()
{
	_player.DoPendingWork();
	if( !_player.IsPlaying() )
	{
		LOG( Logger::LOG_DEBUG, "Player is not playing");
		if( _playlist.Next() )
		{
			_player.Start( _playlist.GetCurrentFilename().c_str(), _playlist.GetCurrentSkipMillis(), _playlist.GetCurrentDurationMillis() );
		}else{
			LOG( Logger::LOG_DEBUG, "Playlist is empty");
		}
	}
}

UiManager* AudioManager::GetUiManager()
{
	return _uiManager;
}

std::map<std::string, std::string> AudioManager::GetInfo()
{
	return _player.GetInfo();
}
