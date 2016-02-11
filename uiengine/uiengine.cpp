#include <stdio.h>
#include <unistd.h>
#include <directfb.h>
#include "UiManager.h"
#include <ConsoleLogger.h>
#include <SyslogLogger.h>
#include <TimerTarget.h>

#define DESCDIR "/usr/share/ui/"

static void usage(const char* procname)
{
    printf("%s [-c] [-l loglevel] [-s startpage] [-d description dir] [-p splash image]\n", procname);
    printf("    -c: log to console instead of syslog\n");
    printf("    -l: set log level\n");
    printf("    -s: set start page\n");
    printf("    -d: set description base directory, default /usr/share/ui\n");
    printf("    -p: show splash image during startup\n");
	if(logger)delete logger;
	exit(0);
}

int main (int argc, char **argv)
{
	logger=new SyslogLogger("simpleui");
    Logger::LogLevel loglevel=Logger::LOG_INFO;
	std::string startpage;
	std::string splashimage;
	std::string descriptionDir = DESCDIR;

    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i], "-l")==0&&argc>i+1){
            i++;
            loglevel=(Logger::LogLevel)atoi(argv[i]);
		}else if(strcmp(argv[i], "-s")==0&&argc>i+1){
            i++;
            startpage=argv[i];
		}else if(strcmp(argv[i], "-p")==0&&argc>i+1){
            i++;
            splashimage=argv[i];
		}else if(strcmp(argv[i], "-d")==0&&argc>i+1){
            i++;
            descriptionDir=argv[i];
        }else if(strcmp(argv[i], "-c")==0){
			delete logger;
			logger=new ConsoleLogger();
        }else{
            usage(argv[0]);
        }
    }

	logger->SetLevel(loglevel);

  DirectFBInit (&argc, &argv);
  UiManager manager;
  if( !splashimage.empty() )
  {
	  if( !manager.ShowSplash( splashimage ) )
	  {
		  return -1;
	  }
  }
  if( !manager.Init( descriptionDir ) )
  {
	  LOG(Logger::LOG_ERROR, "Initializing UI engine failed");
	  return -1;
  }
  if( !startpage.empty() )
  {
	  //load command line supplied start page
	  if(!manager.ShowPage(startpage))
	  {
		  LOG(Logger::LOG_ERROR, "Error loading start page %s", startpage.c_str());
	  }
  }
  LOG( Logger::LOG_DEBUG, "Startup finished");
  while(true)
  {
	long timeout=TimerTarget::s_timerQueue.TimeBeforeNextDue();
	while(!timeout){
		TimerTarget::s_timerQueue.Execute();
		timeout=TimerTarget::s_timerQueue.TimeBeforeNextDue();
	}
	if( !manager.Work(timeout) )break;
  }
  return 0;
}