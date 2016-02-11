#pragma once

#include <string>
#include <map>

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Information about a audio file.  </summary>
///
/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class AudioFileInfo
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	AudioFileInfo();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// 			<summary>	
	/// 			Converts this object to a dictionary with key value pairs.
	/// 			The key names are the same as the names of the private member variables.
	/// 			</summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <returns>	This object as a std::map&lt;std::string,std::string&gt; </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	std::map<std::string, std::string> ToDictionary();
private:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Sets a value given as key value pair. Used to parse madplay output like <c>Title: My Song</c> </summary>
	///
	/// <remarks>	Lars Reemts, 02.05.2011. </remarks>
	///
	/// <param name="key">  	The key. Supported keys are <c>Title</c>, <c>Artist</c>, <c>Album</c>.</param>
	/// <param name="value">	The value. </param>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	void SetValue( const std::string& key, const std::string& value );
	/// <summary> Full pathname of the audio file </summary>
	std::string FilePath;
	/// <summary> The title </summary>
	std::string Title;
	/// <summary> The artist </summary>
	std::string Artist;
	/// <summary> The album </summary>
	std::string Album;
	/// <summary> The total time in seconds </summary>
	int TotalTimeSeconds;
	/// <summary> Give the audio player access to our private members</summary>
	friend class AudioPlayer;
};
