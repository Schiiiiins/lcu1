
#include "Socket.h"

#if defined(_WIN32)
# include <stdio.h>
#include <stdlib.h>
# include <winsock2.h>
//# pragma lib(WS2_32.lib)
typedef int socklen_t;

# define EINPROGRESS	WSAEINPROGRESS
# define EWOULDBLOCK	WSAEWOULDBLOCK
# define ETIMEDOUT	    WSAETIMEDOUT
# define EAGAIN		    WSAEWOULDBLOCK
# define EINTR		    WSAEINTR

#define snprintf _snprintf
#else
extern "C" {
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <netinet/in.h>
# include <netdb.h>
# include <errno.h>
# include <fcntl.h>
}
#endif  // _WIN32


#if defined(_WIN32)
  
static void initWinSock()
{
  static bool wsInit = false;
  if (! wsInit)
  {
    WORD wVersionRequested = MAKEWORD( 2, 0 );
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);
    wsInit = true;
  }
}

#else

#define initWinSock()

#endif // _WIN32


// These errors are not considered fatal for an IO operation; the operation will be re-tried.
static inline bool
nonFatalError()
{
  int err = Socket::getError();
  return (err == EINPROGRESS || err == EAGAIN || err == EWOULDBLOCK || err == EINTR);
}



int
Socket::socket()
{
  initWinSock();
  return (int) ::socket(AF_INET, SOCK_STREAM, 0);
}

int
Socket::domainSocket()
{
  initWinSock();
  return (int) ::socket(AF_UNIX, SOCK_STREAM, 0);
}


void
Socket::close(int fd)
{
#if defined(_WIN32)
  closesocket(fd);
#else
  ::close(fd);
#endif // _WIN32
}




bool
Socket::setNonBlocking(int fd)
{
#if defined(_WIN32)
  unsigned long flag = 1;
  return (ioctlsocket((SOCKET)fd, FIONBIO, &flag) == 0);
#else
  return (fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
#endif // _WIN32
}


bool
Socket::setReuseAddr(int fd)
{
  // Allow this port to be re-bound immediately so server re-starts are not delayed
  int sflag = 1;
  return (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&sflag, sizeof(sflag)) == 0);
}


// Bind to a specified port
bool 
Socket::bind(int fd, int port)
{
  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons((u_short) port);
  return (::bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)) == 0);
}

// Bind unix domain socket to a specified path
bool 
Socket::bind(int fd, const char* path)
{
#if defined(_WIN32)
	return false;//unix domain sockets are not supported on windows
#else
  struct sockaddr_un saddr;

  memset(&saddr, 0, sizeof(saddr));
  saddr.sun_family = AF_UNIX;
  strncpy(saddr.sun_path, path, sizeof(saddr.sun_path));
  unlink(saddr.sun_path);
  return (::bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)) == 0);
#endif
}

// Set socket in listen mode
bool 
Socket::listen(int fd, int backlog)
{
  return (::listen(fd, backlog) == 0);
}


int
Socket::accept(int fd)
{
  struct sockaddr_in addr;
#if defined(_WIN32)
  int
#else
  socklen_t
#endif
    addrlen = sizeof(addr);

  return (int) ::accept(fd, (struct sockaddr*)&addr, &addrlen);
}


    
// Connect a socket to a server (from a client)
bool
Socket::connect(int fd, std::string& host, int port)
{
  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;

  struct hostent *hp = gethostbyname(host.c_str());
  if (hp == 0) return false;

  saddr.sin_family = hp->h_addrtype;
  memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);
  saddr.sin_port = htons((u_short) port);

  // For asynch operation, this will return EWOULDBLOCK (windows) or
  // EINPROGRESS (linux) and we just need to wait for the socket to be writable...
  int result = ::connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));
  return result == 0 || nonFatalError();
}


// Connect a unix domain socket to a server (from a client)
bool
Socket::connect(int fd, const std::string& path)
{
#if defined(_WIN32)
	return false;//unix domain sockets are not supported on windows
#else
  struct sockaddr_un saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sun_family = AF_UNIX;
  strcpy(saddr.sun_path, path.c_str());

  // For asynch operation, this will return EWOULDBLOCK (windows) or
  // EINPROGRESS (linux) and we just need to wait for the socket to be writable...
  int result = ::connect(fd, (struct sockaddr *)&saddr, sizeof(saddr));
  return result == 0 || nonFatalError();
#endif
}



// Read available text from the specified socket. Returns false on error.
bool 
Socket::nbRead(int fd, std::string& s, bool *eof)
{
  const int READ_SIZE = 4096;   // Number of bytes to attempt to read at a time
  char readBuf[READ_SIZE];

  bool wouldBlock = false;
  *eof = false;

  while ( ! wouldBlock && ! *eof) {
#if defined(_WIN32)
    int n = recv(fd, readBuf, READ_SIZE-1, 0);
#else
    int n = read(fd, readBuf, READ_SIZE-1);
#endif
    if (n > 0) {
      readBuf[n] = 0;
      s.append(readBuf, n);
    } else if (n == 0) {
      *eof = true;
    } else if (nonFatalError()) {
      wouldBlock = true;
    } else {
      return false;   // Error
    }
  }
  return true;
}


// Write text to the specified socket. Returns false on error.
bool 
Socket::nbWrite(int fd, std::string& s, int *bytesSoFar)
{
  int nToWrite = int(s.length()) - *bytesSoFar;
  char *sp = const_cast<char*>(s.c_str()) + *bytesSoFar;
  bool wouldBlock = false;

  while ( nToWrite > 0 && ! wouldBlock ) {
#if defined(_WIN32)
    int n = send(fd, sp, nToWrite, 0);
#else
    int n = write(fd, sp, nToWrite);
#endif

    if (n > 0) {
      sp += n;
      *bytesSoFar += n;
      nToWrite -= n;
    } else if (nonFatalError()) {
      wouldBlock = true;
    } else {
      return false;   // Error
    }
  }
  return true;
}


// Returns last errno
int 
Socket::getError()
{
#if defined(_WIN32)
  return WSAGetLastError();
#else
  return errno;
#endif
}


// Returns message corresponding to last errno
std::string 
Socket::getErrorMsg()
{
  return getErrorMsg(getError());
}

// Returns message corresponding to errno... well, it should anyway
std::string 
Socket::getErrorMsg(int error)
{
  char err[60];
  snprintf(err,sizeof(err),"error %d", error);
  return std::string(err);
}

int Socket::getSocketPort(int fd)
{
    // Find out on which port the server is listening
    struct sockaddr_in saddr;
	socklen_t len = sizeof(saddr);
    if(getsockname(fd, (sockaddr*)&saddr, &len))return -1;
    else return ntohs(saddr.sin_port);
}

