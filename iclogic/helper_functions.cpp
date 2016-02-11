#include <time.h>

extern "C" {
    int get_local_time();
}

int get_local_time()
{
    time_t now = time(NULL);
    struct tm utc_now = *gmtime( &now );
    struct tm local_now = *localtime( &now );
    int offset = (local_now.tm_hour-utc_now.tm_hour) * 3600 + (local_now.tm_min-utc_now.tm_min) * 60;
    offset %= 86400;
    if( offset > 43200 ) offset -= 86400;
    return now + offset;
}
