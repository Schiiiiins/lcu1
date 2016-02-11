#include "AlsaMixer.h"
#include <Logger.h>

AlsaMixer::AlsaMixer(void)
{
	_handle = NULL;
}

AlsaMixer::~AlsaMixer(void)
{
	if( _handle )
	{
		snd_mixer_close(_handle);
		_handle = NULL;
	}
}

bool AlsaMixer::IsInitialized()
{
	return _handle != NULL;
}

bool AlsaMixer::Init( const char* card )
{
	LOG( Logger::LOG_DEBUG, "AlsaMixer::Init( %s )", card);
	int err = 0;
	bool success = false;

	if ( (err = snd_mixer_open(&_handle, 0)) < 0 ) {
		LOG( Logger::LOG_ERROR, "Mixer %s open error: %s", card, snd_strerror(err));
		_handle = NULL;
		goto end;
	}
	if( (err = snd_mixer_attach(_handle, card)) < 0) {
		LOG( Logger::LOG_ERROR, "Mixer %s attach error: %s", card, snd_strerror(err));
		goto end;
	}
	if ((err = snd_mixer_selem_register(_handle, NULL, NULL)) < 0) {
		LOG( Logger::LOG_ERROR, "Mixer register error: %s", snd_strerror(err));
		goto end;
	}
	if ((err = snd_mixer_load(_handle)) < 0) {
		LOG( Logger::LOG_ERROR, "Mixer %s load error: %s", card, snd_strerror(err));
		goto end;
	}
	success = true;
 end:
	if( !success )
	{
		if( _handle ) {
			snd_mixer_close(_handle);
			_handle = NULL;
		}
	}
	return success;
}

bool AlsaMixer::SetVolumePercent( const char* channel, int value )
{
	LOG( Logger::LOG_DEBUG, "AlsaMixer::SetVolume( %s, %d )", channel, value);
	bool success = false;
	snd_mixer_elem_t *elem;
	snd_mixer_selem_id_t *sid;
	snd_mixer_selem_id_alloca(&sid);

	snd_mixer_selem_id_set_index( sid, 0);
	snd_mixer_selem_id_set_name( sid, channel);
	elem = snd_mixer_find_selem(_handle, sid);
	if (!elem) {
		LOG( Logger::LOG_ERROR, "Unable to find simple control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
		goto end;
	}
	if( snd_mixer_selem_has_playback_volume(elem) )
	{
		long min, max;
		if( snd_mixer_selem_get_playback_volume_range(elem, &min, &max) < 0 )
		{
			LOG( Logger::LOG_ERROR, "Unable to get playback volume range for control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
			goto end;
		}
		int volume = ((max - min) * value) / 100 + min;
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			if (snd_mixer_selem_set_playback_volume(elem, (snd_mixer_selem_channel_id_t)chn, volume ) >= 0)
			{
				success = true;
			}
		}
	}else if( snd_mixer_selem_has_capture_volume(elem) )
	{
		long min, max;
		if( snd_mixer_selem_get_capture_volume_range(elem, &min, &max) < 0 )
		{
			LOG( Logger::LOG_ERROR, "Unable to get capture volume range for control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
			goto end;
		}
		int volume = ((max - min) * value) / 100 + min;
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			if (snd_mixer_selem_set_capture_volume(elem, (snd_mixer_selem_channel_id_t)chn, volume ) >= 0)
			{
				success = true;
			}
		}
	}
	if( !success )
	{
		LOG( Logger::LOG_ERROR, "Error setting control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
	}
 end:
	return success;
}

bool AlsaMixer::StepVolume( const char* channel, int step )
{
	LOG( Logger::LOG_DEBUG, "AlsaMixer::StepVolume( %s, %d )", channel, step);
	bool success = false;
	snd_mixer_elem_t *elem;
	snd_mixer_selem_id_t *sid;
	snd_mixer_selem_id_alloca(&sid);

	snd_mixer_selem_id_set_index( sid, 0);
	snd_mixer_selem_id_set_name( sid, channel);
	elem = snd_mixer_find_selem(_handle, sid);
	if (!elem) {
		LOG( Logger::LOG_ERROR, "Unable to find simple control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
		goto end;
	}
	if( snd_mixer_selem_has_playback_volume(elem) )
	{
		long min, max;
		if( snd_mixer_selem_get_playback_volume_range(elem, &min, &max) < 0 )
		{
			LOG( Logger::LOG_ERROR, "Unable to get playback volume range for control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
			goto end;
		}

		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			long volume;
			if (snd_mixer_selem_get_playback_volume(elem, (snd_mixer_selem_channel_id_t)chn, &volume ) >= 0)
			{
				volume += step;
				if( volume > max )volume = max;
				if( volume < min )volume = min;

				if (snd_mixer_selem_set_playback_volume(elem, (snd_mixer_selem_channel_id_t)chn, volume ) >= 0)
				{
					success = true;
				}
			}
		}
	}else if( snd_mixer_selem_has_capture_volume(elem) )
	{
		long min, max;
		if( snd_mixer_selem_get_capture_volume_range(elem, &min, &max) < 0 )
		{
			LOG( Logger::LOG_ERROR, "Unable to get capture volume range for control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
			goto end;
		}
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			long volume;
			if (snd_mixer_selem_get_capture_volume(elem, (snd_mixer_selem_channel_id_t)chn, &volume ) >= 0)
			{
				volume += step;
				if( volume > max )volume = max;
				if( volume < min )volume = min;

				if (snd_mixer_selem_set_capture_volume(elem, (snd_mixer_selem_channel_id_t)chn, volume ) >= 0)
				{
					success = true;
				}
			}
		}
	}
	if( !success )
	{
		LOG( Logger::LOG_ERROR, "Error setting control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
	}
 end:
	return success;
}

bool AlsaMixer::GetVolumePercent( const char* channel, int* value )
{
	LOG( Logger::LOG_DEBUG, "AlsaMixer::GetVolume( %s )", channel);
	bool success = false;
	snd_mixer_elem_t *elem;
	snd_mixer_selem_id_t *sid;
	snd_mixer_selem_id_alloca(&sid);

	snd_mixer_selem_id_set_index( sid, 0);
	snd_mixer_selem_id_set_name( sid, channel);
	elem = snd_mixer_find_selem(_handle, sid);
	if (!elem) {
		LOG( Logger::LOG_ERROR, "Unable to find simple control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
		goto end;
	}
	if( snd_mixer_selem_has_playback_volume(elem) )
	{
		long min, max;
		if( snd_mixer_selem_get_playback_volume_range(elem, &min, &max) < 0 )
		{
			LOG( Logger::LOG_ERROR, "Unable to get playback volume range for control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
			goto end;
		}
		long volume;
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			if (snd_mixer_selem_get_playback_volume(elem, (snd_mixer_selem_channel_id_t)chn, &volume ) >= 0)
			{
				*value = (volume - min) * 100 / ( max - min );
				success = true;
				break;
			}
		}
	}else if( snd_mixer_selem_has_capture_volume(elem) )
	{
		long min, max;
		if( snd_mixer_selem_get_capture_volume_range(elem, &min, &max) < 0 )
		{
			LOG( Logger::LOG_ERROR, "Unable to get capture volume range for control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
			goto end;
		}
		long volume;
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			if (snd_mixer_selem_get_capture_volume(elem, (snd_mixer_selem_channel_id_t)chn, &volume ) >= 0)
			{
				*value = (volume - min) * 100 / ( max - min );
				success = true;
				break;
			}
		}
	}
	if( !success )
	{
		LOG( Logger::LOG_ERROR, "Error getting control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
	}
 end:
	return success;
}

bool AlsaMixer::SwitchChannel( const char* channel, bool value )
{
	bool success = false;
	snd_mixer_elem_t *elem;
	snd_mixer_selem_id_t *sid;
	snd_mixer_selem_id_alloca(&sid);

	snd_mixer_selem_id_set_index( sid, 0);
	snd_mixer_selem_id_set_name( sid, channel);
	elem = snd_mixer_find_selem(_handle, sid);
	if (!elem) {
		LOG( Logger::LOG_ERROR, "Unable to find simple control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
		goto end;
	}

	if( snd_mixer_selem_has_playback_switch(elem) )
	{
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			if (snd_mixer_selem_set_playback_switch(elem, (snd_mixer_selem_channel_id_t)chn, value ? 1 : 0 ) >= 0)
			{
				success = true;
			}
		}
	}else if( snd_mixer_selem_has_capture_switch(elem) )
	{
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			if (snd_mixer_selem_set_capture_switch(elem, (snd_mixer_selem_channel_id_t)chn, value ? 1 : 0 ) >= 0)
			{
				success = true;
			}
		}
	}
	if( !success )
	{
		LOG( Logger::LOG_ERROR, "Error setting control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
	}
 end:
	return success;
}

bool AlsaMixer::GetChannelSwitch( const char* channel, bool* on )
{
	bool success = false;
	snd_mixer_elem_t *elem;
	snd_mixer_selem_id_t *sid;
	snd_mixer_selem_id_alloca(&sid);

	snd_mixer_selem_id_set_index( sid, 0);
	snd_mixer_selem_id_set_name( sid, channel);
	elem = snd_mixer_find_selem(_handle, sid);
	if (!elem) {
		LOG( Logger::LOG_ERROR, "Unable to find simple control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
		goto end;
	}

	if( snd_mixer_selem_has_playback_switch(elem) )
	{
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			int value;
			if (snd_mixer_selem_get_playback_switch(elem, (snd_mixer_selem_channel_id_t)chn, &value) >= 0)
			{
				*on = value != 0; 
				success = true;
				break;
			}
		}
	}else if( snd_mixer_selem_has_capture_switch(elem) )
	{
		for (int chn = 0; chn <= SND_MIXER_SCHN_LAST; chn++) {
			int value;
			if (snd_mixer_selem_get_capture_switch(elem, (snd_mixer_selem_channel_id_t)chn, &value ) >= 0)
			{
				*on = value != 0; 
				success = true;
				break;
			}
		}
	}
	if( !success )
	{
		LOG( Logger::LOG_ERROR, "Error getting control '%s',%i\n", snd_mixer_selem_id_get_name(sid), snd_mixer_selem_id_get_index(sid));
	}
 end:
	return success;
}
