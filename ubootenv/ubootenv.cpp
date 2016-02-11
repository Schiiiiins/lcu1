#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <signal.h>
#include <string>
#include <map>
#include "crc32.h"
//u-boot config file. u-boot needs to be configured first
#include <config.h>
#include <PropertyMap.h>

const char* DEFAULT_DEVICES[]={
    "/dev/mtd0",
    "/dev/mtd/0",
    NULL
};

const char* CONFIGFILE="/etc/ubootenv.conf";

#define ENV_START CONFIG_ENV_OFFSET
#define ENV_SIZE CONFIG_ENV_SIZE

int env_start=ENV_START;
int env_size=ENV_SIZE;

const char* device = NULL;

typedef struct environment_s {
	unsigned long	crc;		/* CRC32 over data bytes	*/
	char	data[ENV_SIZE-sizeof(unsigned long)]; /* Environment data		*/
} env_t;

typedef std::map<std::string, std::string> env_map_t;

static void usage(const char* procname)
{
    fprintf(stderr, "%s [-d mtd-device] [-a start-address] [-s size] [-v] command [args]\n", procname);
    fprintf(stderr, "   -d: set mtd device to use. default: /dev/mtd0 or read from %s\n", CONFIGFILE);
    fprintf(stderr, "   -a: start address of environment. default:0x%04X\n", ENV_START);
    fprintf(stderr, "   -s: size of environment. default:0x%04X\n", ENV_SIZE);
    fprintf(stderr, "   -v: print values only\n");
    fprintf(stderr, "   command: set or print\n");
    fprintf(stderr, "   args: variables to set in 'key=value' form\n");
	exit(-1);
}

static int write_environment_struct(env_t* env)
{
    int fd=open(device, O_RDWR);
    if(fd<0){
        perror("open");
        return fd;
    }
    lseek(fd, env_start, SEEK_SET);
    int count=0;
    while((unsigned int)count<sizeof(env_t)){
        int retval=write(fd, ((unsigned char*)env)+count, sizeof(env_t)-count);
        if(retval<0){
            perror("write error");
            close(fd);
            return -1;
        }
        count+=retval;
    }
    close(fd);
    return 0;
};

static int read_environment_struct(env_t* env)
{
    int fd=open(device, O_RDONLY);
    if(fd<0)return fd;
    lseek(fd, env_start, SEEK_SET);
    int count=0;
    while((unsigned int)count<sizeof(env)){
        int retval=read(fd, ((unsigned char*)env)+count, sizeof(env_t)-count);
        if(retval<0){
            perror("read error");
            close(fd);
            return -1;
        }
        count+=retval;
    }
    close(fd);
    return 0;
};

static int read_environment(env_map_t* env_map)
{
    env_t env;
    if(read_environment_struct(&env)<0)return -1;
    unsigned long crc=crc32(0, (const unsigned char*)env.data, sizeof(env.data));
    if(crc != env.crc){
        fprintf(stderr, "CRC mismatch reading environment\n");
        return -1;
    }
    unsigned int i=0;
    while(i<sizeof(env.data) && env.data[i])
    {
        std::string key=env.data+i;
        i+=key.size()+1;
        unsigned int eq_pos=key.find('=');
        std::string value;
        if(eq_pos<std::string::npos){
            value=key.substr(eq_pos+1);
            key=key.substr(0, eq_pos);
        }
        (*env_map)[key]=value;
    }
    return 0;
};

static int write_environment(const env_map_t& env_map)
{
    env_t env;
    memset(&env, 0, sizeof(env));
    char* p=env.data;
    for(env_map_t::const_iterator it=env_map.begin();it!=env_map.end();it++)
    {
        const std::string& key=it->first;
        const std::string& value=it->second;
        if(p-env.data+key.size()+1+value.size()+1+1>=sizeof(env.data)){
            fprintf(stderr, "write environment: size exceeded\n");
            return -1;
        }
        strcpy(p, key.c_str());
        p+=key.size();
        *p='=';
        p++;
        strcpy(p, value.c_str());
        p+=value.size()+1;
    }
    env.crc=crc32(0, (const unsigned char*)env.data, sizeof(env.data));
    if(write_environment_struct(&env)<0){
        fprintf(stderr, "write environment: write failed\n");
        return -1;
    }
    return 0;
};

int main(int argc, char* argv[])
{
    char** cmd_argv=0;
    int cmd_argc=0;
    
    enum{SET, PRINT};
    int command=PRINT;
    int i;
    bool values_only=false;
    const char* dev_candidate=DEFAULT_DEVICES[0];
    
    std::string device_path;
    
    for(i=1;i<argc;i++)
    {
        if(strcmp(argv[i], "-a")==0&&argc>i+1){
            i++;
            env_start=strtoul(argv[i], NULL, 0);
        }else if(strcmp(argv[i], "-s")==0&&argc>i+1){
            i++;
            env_size=strtoul(argv[i], NULL, 0);
        }else if(strcmp(argv[i], "-d")==0&&argc>i+1){
            i++;
            device=argv[i];
        }else if(strcmp(argv[i], "-v")==0){
            values_only=true;
        }else if(strcmp(argv[i], "set")==0){
            command=SET;
            i++;
            break;
        }else if(strcmp(argv[i], "print")==0){
            command=PRINT;
            i++;
            break;
        }else{
            usage(argv[0]);
        }
    }

    if( !device )
    {
        PropertyMap configValues;
        if(configValues.ReadFromFile( CONFIGFILE ))
        {
            device_path = configValues.GetStringValue("device");
            if( !device_path.empty() ) device = device_path.c_str();
        }
    }
    while(!device){
        struct stat buf;
        if(!dev_candidate){
            fprintf(stderr, "Please specify MTD device (-d)\n");
            return -1;
        }
        if(stat(dev_candidate, &buf) == 0)device=dev_candidate;
        else dev_candidate++;
    }
    
    cmd_argv=argv+i;
    cmd_argc=argc-i;
    
    if(command==PRINT){
        env_map_t env_map;
        if(read_environment(&env_map)<0)exit(-1);
        if(cmd_argc==0){
            for(env_map_t::const_iterator it=env_map.begin();it!=env_map.end();it++)
            {
                printf("%s=%s\n", it->first.c_str(), it->second.c_str());
            }
        }else{
            for(int i=0;i<cmd_argc;i++)
            {
                env_map_t::const_iterator it=env_map.find(cmd_argv[i]);
                if(it==env_map.end()){
                    if(values_only)printf("\n");
                    else printf("%s=\n", cmd_argv[i]);
                }else{
                    if(values_only)printf("%s\n", it->second.c_str());
                    else printf("%s=%s\n", it->first.c_str(), it->second.c_str());
                }
            }
        }
    }else if(command==SET){
        env_map_t env_map;
        if(read_environment(&env_map)<0)exit(-1);
        for(int i=0;i<cmd_argc;i++)
        {
            std::string key=cmd_argv[i];
            unsigned int eq_pos=key.find('=');
            std::string value;
            if(eq_pos<std::string::npos){
                value=key.substr(eq_pos+1);
                key=key.substr(0, eq_pos);
            }
            env_map[key]=value;
        }
        if(write_environment(env_map)<0)exit(-1);
    }
    return 0;
}
