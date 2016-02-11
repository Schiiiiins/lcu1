#ifndef _OSCOMPAT_H_
#define _OSCOMPAT_H_

#include "dllexport.h"
#include <string>

//! Kapselung von betriebssystemspezifischen Verhaltensweisen
class ELVUTILS_DLLEXPORT OSCompat
{
private:
    OSCompat(void){};
    ~OSCompat(void){};
public:
    class ELVUTILS_DLLEXPORT DirectoryLister
    {
    public:
        enum{
            FLAG_LIST_FILES=1,
            FLAG_LIST_DIRS=2
        };
        DirectoryLister(const char* path, int flags=FLAG_LIST_FILES);
        ~DirectoryLister();
        std::string NextEntry();
    private:
        void* private_data;
        int flags;
        std::string path;
    };
    static const char PATH_SEPARATOR;
    static void RaiseThreadPriority();
    static std::string FixPath(const std::string& s);
	static bool MakeDirectory(const char* s);
	static std::string Dirname( const std::string& s );
private:
    static std::string ResolvePathConstants( const std::string& s);
};

#endif
