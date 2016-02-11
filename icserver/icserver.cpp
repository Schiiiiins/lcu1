#include "Manager.h"
#include "HmManager.h"
#include "HmInterface.h"
#include "HmDevice.h"
#include "HmChannel.h"
#include "HmValue.h"
#include "ConsoleLogger.h"
#include "SyslogLogger.h"
#include "OSCompat.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <fstream>

#if defined(_WIN32)
#include <windows.h>
#endif

static void usage(const char* procname)
{
    printf("%s [-d] [-c] [-l loglevel] [-p pidfile]\n", procname);
    printf("    -d: daemonize to background\n");
    printf("    -c: log to console instead of syslog\n");
    printf("    -l: set log level\n");
    printf("    -p: write pid to pidfile\n");
	if(logger)delete logger;
	exit(0);
}


int main(int argc, char* argv[])
{
#if defined(_WIN32)
    WORD wVersionRequested = MAKEWORD( 2, 0 );
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);
#endif

	logger=new SyslogLogger("icserver");
    Logger::LogLevel loglevel=Logger::LOG_INFO;
#if !defined(_WIN32)
    bool daemonize = false;
    const char* pidfile = NULL;
#endif
    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i], "-l")==0&&argc>i+1){
            i++;
            loglevel=(Logger::LogLevel)atoi(argv[i]);
#if !defined(_WIN32)
        }else if(strcmp(argv[i], "-p")==0&&argc>i+1){
            i++;
            pidfile=argv[i];
        }else if(strcmp(argv[i], "-d")==0){
            daemonize = true;
#endif
        }else if(strcmp(argv[i], "-c")==0){
			delete logger;
			logger=new ConsoleLogger();
        }else{
            usage(argv[0]);
        }
    }

	logger->SetLevel(loglevel);

#if !defined(_WIN32)
    if(pidfile){
	    umask(0000);
	    std::ofstream os(pidfile);
	    os<<getpid();
	    os.close();
    }
#endif
    
	Manager::GetSingleton()->InitServers();

#if defined(_WIN32)
	if(!Manager::GetSingleton()->LoadSystemConfig(OSCompat::FixPath("${Bindir}/icserver_system.conf").c_str(), OSCompat::FixPath("${Bindir}/icserver_devices.conf").c_str()))
	{
		exit(-1);
	}
	Manager::GetSingleton()->LoadPersistentValues("${Bindir}/icserver_values");
	HmManager::GetSingleton()->LoadUserConfig(OSCompat::FixPath("${Bindir}/icserver_user.conf").c_str());
#else
	if(!Manager::GetSingleton()->LoadSystemConfig("/etc/icserver/icserver_system.conf", "/etc/config/icserver_devices.conf"))
	{
		exit(-1);
	}
	Manager::GetSingleton()->LoadPersistentValues("/etc/config/icserver_values");
	HmManager::GetSingleton()->LoadUserConfig("/etc/config/icserver_user.conf");

    if( daemonize )
    {
        if(daemon(0, 0)){
            LOG(Logger::LOG_ERROR, "Could not daemonize, exiting");
            exit(-1);
        }
        LOG(Logger::LOG_DEBUG, "Deamonized");
    }
#endif
    
	Manager::GetSingleton()->Run();
}
