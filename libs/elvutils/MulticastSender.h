#ifndef _MULTICAST_SENDER_H_
#define _MULTICAST_SENDER_H_

#include "dllexport.h"
#include <string>
#include <vector>

struct in_addr;

class ELVUTILS_DLLEXPORT MulticastSender
{
public:
    MulticastSender(const char* mcast_group, unsigned int port, bool broadcast=false);
    ~MulticastSender(void);
	typedef bool (*CheckResponseCallback)(void*, const std::string&);
    bool SendUDPMessage(const std::string& msg, std::vector<std::string>* responses, unsigned int timeout, unsigned int max_response_count, CheckResponseCallback check_response_callback = NULL, void* check_response_callback_arg = NULL);
private:
	static std::vector<in_addr> GetOwnAddresses();
    static void perror(const char* func);
    enum
    {
        UDP_RX_BUFFER_SIZE = 2000
    };
    std::string group;
    int port;
    bool use_broadcast;
};

#endif
