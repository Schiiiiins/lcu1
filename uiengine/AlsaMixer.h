#pragma once
#include <alsa/asoundlib.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Class for controlling the Alsa audio mixer using the Alsa userspace library. </summary>
///
/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AlsaMixer
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AlsaMixer(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Finaliser. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual ~AlsaMixer(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Initialises the mixer object opening a library connection. 
	/// 			Needs to be called before calling any other function.</summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="card">	Name of the sound card. Use <c>"default"</c> for the first card. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool Init( const char* card = "default" );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> Switch mixer channel on or off. Use Linux command <c>amixer</c> for a list 
	/// 		  of channels supported by your sound device. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="channel">	Name of the mixer channel to switch. </param>
	/// <param name="value">  	true for switching channel on, false for switching off. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool SwitchChannel( const char* channel, bool value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Gets a mixer channel switch status. Use Linux command <c>amixer</c> for a list 
	/// 			of channels supported by your sound device. </summary>
	///
	/// <remarks>	Lars Reemts, 04.05.2011. </remarks>
	///
	/// <param name="channel">	Name of the mixer channel to query. </param>
	/// <param name="value">  	[out] The switch status. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool GetChannelSwitch( const char* channel, bool* value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> Sets the volume of a mixer channel. Use Linux command <c>amixer</c> for a list 
	/// 		  of channels supported by your sound device. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="channel">	Name of the channel to set the volume for. </param>
	/// <param name="value">  	Volume value in percent. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool SetVolumePercent( const char* channel, int value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> Change the volume of a mixer channel by a relative value. 
	/// 		  Use Linux command <c>amixer</c> for a list of channels supported 
	/// 		  by your sound device. </summary>
	///
	/// <remarks>	Lars Reemts, 04.05.2011. </remarks>
	///
	/// <param name="channel">	The channel. </param>
	/// <param name="step">   	Amount to increment by. Use negative numbers to decrement. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool StepVolume( const char* channel, int step );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary> Gets the volume of a mixer channel. Use Linux command <c>amixer</c> for a list 
	/// 		  of channels supported by your sound device. </summary>
	///
	/// <remarks>	Lars Reemts, 04.05.2011. </remarks>
	///
	/// <param name="channel">	Name of the channel to query the volume for. </param>
	/// <param name="value">  	[out] The volume value in percent. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool GetVolumePercent( const char* channel, int* value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Query if this object is initialized. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	true if initialized, false if not. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	bool IsInitialized();
private:
	/// <summary> Alsa library handle for the mixer </summary>
	snd_mixer_t *_handle;
};
