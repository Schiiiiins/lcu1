#pragma once
#include <sys/types.h>
#include "AudioFileInfo.h"

class AudioManager;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Audio player. This class controls madplay running in the background to do the actual 
/// 			playing. madplay is started by supplying a valid command line. The textual output
/// 			from madplay is parsed to obtain information on the current status. </summary>
///
/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AudioPlayer
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="audioManager">	[in] Associated audio manager object. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AudioPlayer( AudioManager* audioManager );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Finaliser. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual ~AudioPlayer(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Starts playing the given file. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="filename">		 	Filename of the audio file. Absolute path or path relative to 
	/// 								UI manager's description directory</param>
	/// <param name="skipMillis">	 	Number of milliseconds to skip at the start of the audio file. </param>
	/// <param name="durationMillis">	Duration of the audio to play from the file. Use <c>0</c> for 
	/// 								the complete file. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Start( const char* filename, int skipMillis, int durationMillis );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Stops the audio player. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Stop();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets the file descriptor for reading from madplay stdout and writing to madplay
	/// 			stdin. Connected to madplay using a pseudo terminal device.</summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	The madplay file descriptor. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	int GetFd();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Should be called if the associated file descriptor (<see cref="GetFd" />)
	/// 			becomes readable. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void DoPendingWork();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if an audio file is being played. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	true if playing, false if not. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool IsPlaying();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a map containing information on the currently played file. The map contains 
	/// 			the information obtained from <see cref="AudioFileInfo::ToDictionary" /> plus 
	/// 			<c>RemainingTime</c> as an additional field. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	The current audio file information. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	std::map<std::string, std::string> GetInfo();
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Starts the audio player process (madplay). </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="filename">		 	Filename of the audio file. Absolute path or path relative to 
	/// 								UI manager's description directory</param>
	/// <param name="skipMillis">	 	Number of milliseconds to skip at the start of the audio file. </param>
	/// <param name="durationMillis">	Duration of the audio to play from the file. Use <c>0</c> for 
	/// 								the complete file. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool StartPlayerProcess( const char* filename, int skipSeconds, int duration );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Process the a string received from madplay stdout. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="s">	String read from madplay stdout. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void ProcessStdout( const char* s );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Process the a single line received from madplay stdout. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="l">	The line omitting line end characters. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void ProcessStdoutLine( std::string l );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Inform the TCL engine associated with the current UI page that a new audio file
	/// 			has been started. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void SendStartEvent();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Inform the TCL engine associated with the current UI page that the elapsed time 
	/// 			has changed while playing an audio file </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void SendProgressEvent();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Inform the TCL engine associated with the current UI page that playing the current 
	/// 			audio file has ended started. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void SendStopEvent();
	/// <summary> The madplay pid </summary>
	pid_t _playerPid;
	/// <summary> The fd connected to madplay via a pseudo terminal </summary>
	int _playerPtyFd;
	/// <summary> The number of seconds remaining for the current audio file </summary>
	unsigned int _secondsRemaining;
	/// <summary> Buffer for player stdout data </summary>
	std::string _playerStdoutBuffer;
	/// <summary> Information describing the file currently being played </summary>
	AudioFileInfo _playingFileInfo;
	/// <summary> The audio manager object </summary>
	AudioManager* _audioManager;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of AudioPlayer.h
////////////////////////////////////////////////////////////////////////////////////////////////////

