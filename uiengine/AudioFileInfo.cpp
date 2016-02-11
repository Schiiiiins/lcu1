#include "AudioFileInfo.h"

AudioFileInfo::AudioFileInfo(void)
{
		TotalTimeSeconds = 0;
}

void AudioFileInfo::SetValue( const std::string& key, const std::string& value )
{
	if( key == "Title" ) Title = value;
	else if( key == "Artist" ) Artist = value;
	else if( key == "Album" ) Album = value;
}

std::map<std::string, std::string> AudioFileInfo::ToDictionary()
{
	std::map<std::string, std::string> d;
	d["FilePath"] = FilePath;
	d["Title"] = Title;
	d["Artist"] = Artist;
	d["Album"] = Album;
	char buffer[32];
	sprintf(buffer, "%u", TotalTimeSeconds);
	d["TotalTimeSeconds"] = buffer;
	return d;
}
