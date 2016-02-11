#include "AudioPlayer.h"
#include "AudioManager.h"
#include "UiManager.h"
#include "PageScriptingEngine.h"
#include <Logger.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <vector>
#include <string>
#include <map>

AudioPlayer::AudioPlayer( AudioManager* audioManager ) : _audioManager ( audioManager )
{
	_playerPid = -1;
	_playerPtyFd = -1;
	_secondsRemaining = 0;
}

AudioPlayer::~AudioPlayer(void)
{
	Stop();
}

bool AudioPlayer::IsPlaying()
{
	return _playerPtyFd >= 0;
}

int AudioPlayer::GetFd()
{
	return _playerPtyFd;
}

bool AudioPlayer::Start( const char* filename, int skip, int duration )
{
	LOG( Logger::LOG_DEBUG, "AudioPlayer::Start(%s, %d, %d)", filename, skip, duration);
	if( IsPlaying() )
	{
		LOG( Logger::LOG_WARNING, "Cannot start madplay because it is already running");
		return false;
	}
	return StartPlayerProcess( filename, skip, duration );
}

bool AudioPlayer::Stop()
{
	LOG( Logger::LOG_DEBUG, "AudioPlayer::Stop()");
	if( IsPlaying() )
	{
		write( _playerPtyFd, "q", 1 );
		//kill( _playerPid, SIGTERM );
	}
	return true;
}

void AudioPlayer::DoPendingWork()
{
	//LOG( Logger::LOG_DEBUG, "AudioPlayer::DoPendingWork()");
	char buffer[256];
	while( _playerPtyFd >= 0 )
	{
		int count = read(_playerPtyFd, buffer, sizeof(buffer)-1);
		if( count <= 0 )
		{
			if( count < 0 && errno == EAGAIN )
			{
				//nothing to do. return.
				return;
			}
			//termination of madplay
			int status;
			if( waitpid( _playerPid, &status, WNOHANG ) == _playerPid )
			{
				close( _playerPtyFd );
				_playerPtyFd = -1;
				_playerPid = -1;
				if( WIFEXITED(status) )
				{
					LOG( Logger::LOG_DEBUG, "madplay exited with exit code %d", WEXITSTATUS(status) );
				}else{
					LOG( Logger::LOG_DEBUG, "madplay exited abnormally" );
				}
                _playingFileInfo = AudioFileInfo();
				SendStopEvent();
			}
		}else{
			buffer[count] = 0;
			ProcessStdout( buffer );
			//LOG( Logger::LOG_DEBUG, "madplay output:%s", buffer );
		}
	}
}

void AudioPlayer::ProcessStdout( const char* s )
{
	_playerStdoutBuffer.append( s );
	std::string::size_type line_start = 0;
	std::string::size_type line_end = _playerStdoutBuffer.find_first_of( "\n\r", line_start );
	while( line_end != std::string::npos )
	{
		ProcessStdoutLine( _playerStdoutBuffer.substr( line_start, line_end - line_start ) );
		line_start = _playerStdoutBuffer.find_first_not_of( "\n\r", line_end );
		if( line_start == std::string::npos ) break;
		line_end = _playerStdoutBuffer.find_first_of( "\n\r", line_start );
	}
	_playerStdoutBuffer.erase(0, line_start);
}

void AudioPlayer::ProcessStdoutLine( std::string l )
{
	unsigned int minutes, hours, seconds;
	if( sscanf( l.c_str(), "-%u:%u:%u ", &hours, &minutes, &seconds ) == 3 )
	{
		_secondsRemaining = hours * 3600 + minutes * 60 + seconds;
		if( !_playingFileInfo.TotalTimeSeconds )
		{
			_playingFileInfo.TotalTimeSeconds = _secondsRemaining;
			SendStartEvent();
		}
		//LOG( Logger::LOG_DEBUG, "%u/%u", _secondsRemaining, _playingFileInfo.TotalTimeSeconds );
		SendProgressEvent();
	} else {
		std::string::size_type pos=l.find_first_not_of(" \t");
		if(pos)l.erase(0, pos);
		pos = l.find( ": " );
		if( pos != std::string::npos )
		{
			std::string key = l.substr(0, pos );
			std::string value = l.substr( pos + 2 );
			_playingFileInfo.SetValue( key, value );
		}
	}
	
}

bool AudioPlayer::StartPlayerProcess( const char* filename, int skipMillis, int durationMillis )
{
	_playerStdoutBuffer.clear();
	_playingFileInfo = AudioFileInfo();
	_secondsRemaining = 0;

	int ptyMaster = posix_openpt( O_RDWR | O_NOCTTY );
	if( ptyMaster < 0 )
	{
		return false;
	}

	grantpt( ptyMaster );
	unlockpt( ptyMaster );

	char ptySlaveName[64];
	const char* name = ptsname( ptyMaster );
	if( !name )
	{
		close( ptyMaster );
		return false;
	}
	strncpy( ptySlaveName, name, sizeof(ptySlaveName) );

	struct termios ttyAttr;
	int ttyAttrOk = tcgetattr(STDIN_FILENO, &ttyAttr);

	struct winsize ttyWinsize;
	int ttyWinsizeOk = ioctl(STDIN_FILENO, TIOCGWINSZ, (char*)&ttyWinsize);

/*
	struct termios rawTermios = ttyAttr;
	cfmakeraw(&rawTermios);
	rawTermios.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawTermios);
*/
	pid_t pid = vfork();
	if( pid==-1 )
	{
		return false;
	}
	if( pid )
	{
		//parent
		//set the reading end of the pty to nonblocking mode
		fcntl(ptyMaster, F_SETFL, fcntl(ptyMaster, F_GETFL) | O_NONBLOCK);
		_playerPtyFd = ptyMaster;
		_playerPid = pid;
		_playingFileInfo.FilePath = filename;
		return true;
	}else{
		//child
		close(ptyMaster); /* close pty master */
		/* open pty slave to fd 0,1,2 */
		close(0);
		open(ptySlaveName, O_RDWR, 0); /* uses fd 0 (STDIN_FILENO)*/
		dup2(0, STDOUT_FILENO);
		dup2(0, STDERR_FILENO);
		/* copy our original stdin tty's parameters to pty */
		if (ttyAttrOk == 0)
		{
			tcsetattr(0, TCSAFLUSH, &ttyAttr);
		}
		if (ttyWinsizeOk == 0)
		{
			ioctl(0, TIOCSWINSZ, (char *)&ttyWinsize);
		}
		/* set pty as a controlling tty */
		setsid();
		ioctl(STDIN_FILENO, TIOCSCTTY, 0 /* 0: don't forcibly steal */);

		std::vector<std::string> argvec;
		argvec.push_back(std::string("madplay"));
		argvec.push_back(std::string("--sample-rate=48500"));
		argvec.push_back(std::string("-v"));
		argvec.push_back(std::string("--display-time=remaining"));
		if( skipMillis > 0 )
		{
			char buffer[64];
			sprintf(buffer, "--start=%02d:%02d:%02d.%03d", (skipMillis / 3600000) % 60, (skipMillis / 60000) % 60, (skipMillis / 1000) % 60, skipMillis % 1000);
			argvec.push_back(std::string(buffer));
		}
		if( durationMillis > 0 )
		{
			char buffer[64];
			sprintf(buffer, "--time=%02d:%02d:%02d.%03d", (durationMillis / 3600000) % 60, (durationMillis / 60000) % 60, (durationMillis / 1000) % 60, durationMillis % 1000);
			argvec.push_back(std::string(buffer));
		}
		argvec.push_back(filename);
		char* argv[16];
		for( unsigned int i=0; i<argvec.size(); i++ )
		{
			argv[i] = const_cast<char*>(argvec[i].c_str());
			LOG( Logger::LOG_DEBUG, "argv[%d] = %s", i, argv[i] );
		}
		argv[argvec.size()] = NULL;
		execvp( argv[0], argv );
		//dummy statement, should never be executed
		exit(-1);
		//make compiler happy
		return false;
	}
}

void AudioPlayer::SendStartEvent()
{
	PageScriptingEngine::ArrayType playingFileInfo = GetInfo();

	std::vector<std::string> params;
	params.resize(1);
	if( PageScriptingEngine::ArrayToList( playingFileInfo, &params.back() ) )
	{
		_audioManager->GetUiManager()->ExecuteAudioEvent( "start", params );
	}
}

void AudioPlayer::SendProgressEvent()
{
	std::vector<std::string> params;
	char buffer[32];
	sprintf( buffer, "%u", _playingFileInfo.TotalTimeSeconds );
	params.push_back( buffer );
	sprintf( buffer, "%u", _secondsRemaining );
	params.push_back( buffer );
	_audioManager->GetUiManager()->ExecuteAudioEvent( "progress", params );
}

void AudioPlayer::SendStopEvent()
{
	std::vector<std::string> params;
	_audioManager->GetUiManager()->ExecuteAudioEvent( "stop", params );
}

std::map<std::string, std::string> AudioPlayer::GetInfo()
{
	std::map<std::string, std::string> info = _playingFileInfo.ToDictionary();
	char buffer[32];
	sprintf( buffer, "%u", _secondsRemaining );
	info["RemainingTime"] = buffer;
	return info;
}
