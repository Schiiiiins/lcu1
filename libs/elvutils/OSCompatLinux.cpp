#ifndef WIN32

#include "OSCompat.h"
#include <string>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "Logger.h"

/*static*/ const char OSCompat::PATH_SEPARATOR='/';

/*static*/ void OSCompat::RaiseThreadPriority()
{
    setpriority(PRIO_PROCESS, 0, -5);
}

/*static*/ std::string OSCompat::ResolvePathConstants(const std::string& s)
{
    std::string path=s;
    std::string::size_type left;
    std::string::size_type right=0;
    while(true){
        left=path.find("${", right);
        if(left == std::string::npos)break;
        right=path.find("}", left+2);
        if(right == std::string::npos)break;
        std::string constant=path.substr(left+2, right-left-2);
        std::string value;
        char buffer[256];
        if(constant == "Bindir"){
            //directory of executable file
            sprintf(buffer, "/proc/%d/exe", getpid());
            int count=readlink(buffer, buffer, sizeof(buffer));
            if(count>0){
                value.assign(buffer, count);
                std::string::size_type p=value.rfind('/');
                value.erase(p);
            }
        }else if(constant == "Home"){
            value = getenv("HOME");
        }
        path.replace(left, right-left+1, value);
        right=left+value.size();
    }
    return path;
}

/*static*/ std::string OSCompat::FixPath(const std::string& s)
{
    std::string path;
    if( s.empty() || ( s[0] != '/' && s[0] != '$') ){
        path = "${Bindir}/";
    }
    path+=s;
    std::string fixed_path;
    fixed_path.reserve(path.size());
    std::string::size_type left=0;
    std::string::size_type right=0;
    while(left!=std::string::npos)
    {
        right=path.find_first_of("/\\", left);
        fixed_path+=ResolvePathConstants(path.substr(left, right-left));
        if(right==std::string::npos)break;
        fixed_path+=PATH_SEPARATOR;
        left=path.find_first_not_of("/\\", right);
    }
    return fixed_path;
}

/*static*/ bool OSCompat::MakeDirectory(const char* s)
{
    if (mkdir(s, 0777) >= 0) {
        return true;
    }else if(errno == ENOENT){
        //recursively create the base directories
        std::string base_dir=s;
        std::string::size_type pos = base_dir.rfind(PATH_SEPARATOR);
        if(pos != std::string::npos){
            base_dir.erase(pos);
            if( MakeDirectory(base_dir.c_str()) ){
                return mkdir(s, 0777) >= 0;
            }else{
                return false;
            }
        } else {
            return false;
        }
    }else{
        return (errno == EEXIST);
    }
}

/*static*/ std::string OSCompat::Dirname( const std::string& s )
{
	std::string::size_type pos = s.find_last_of( "\\/" );
	if( pos == std::string::npos )return ".";
	if( pos == 0 )return "/";
	return s.substr(0, pos);
}

OSCompat::DirectoryLister::DirectoryLister(const char* path, int flags)
{
    this->flags=flags;
    this->path=path;
    if(this->path[this->path.size()-1]!=PATH_SEPARATOR)this->path+=PATH_SEPARATOR;
    private_data=NULL;
}

OSCompat::DirectoryLister::~DirectoryLister()
{
    if(private_data){
        DIR *pDirectory = (DIR*)private_data;
        closedir(pDirectory);
    }
}

std::string OSCompat::DirectoryLister::NextEntry()
{
    if(!private_data){
        DIR *pDirectory = opendir(path.c_str());
        if(!pDirectory){
            LOG(Logger::LOG_WARNING, "opendir(%s) failed", path.c_str());
            return "";
        }
        private_data=pDirectory;
        return NextEntry();
    }else{
        DIR *pDirectory = (DIR*)private_data;
        struct dirent *pEntry = readdir( pDirectory );
        if(!pEntry)return "";

        std::string filename=path;
        filename+="/";
        filename+=pEntry->d_name;
		struct stat st;
		if(stat(filename.c_str(), &st)==0 && S_ISREG(st.st_mode)){
            if((flags & FLAG_LIST_FILES)!=0)return pEntry->d_name;
        }else{
            if((flags & FLAG_LIST_DIRS)!=0)return pEntry->d_name;
        }
        return NextEntry();
    }
}

#endif
