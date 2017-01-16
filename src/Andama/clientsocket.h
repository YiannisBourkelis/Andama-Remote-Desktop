#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#ifdef WIN32
#define NOMINMAX
#include <stdio.h>
#include "winsock2.h"
#include <ws2tcpip.h>
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)
#define in_addr_t uint32_t
#pragma comment(lib, "Ws2_32.lib")

#else
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/tcp.h> //gia to TCP_NODELAY
#endif

#include "helperfuncs.h"
#include <iostream>
#include "vector"
#include <mutex>
#include <atomic>
#include "qdebug.h"
#include <errno.h>
#include <thread>
#include <array>
#include <random>
#include <string>
#include <memory>
#include <QObject>

#include "clientserverprotocol.h"
#include "../Shared/AndamaHeaders/shared_enums.h"
#include "../Shared/AndamaHeaders/exception_helper.h"
#include "../Shared/AndamaHeaders/shared_constants.h"


class clientSocket : public QObject
{
    Q_OBJECT
public:
    clientSocket() ;

    //public variables
    clientServerProtocol* protocol;
    int serverPort = PROXY_PORT_NUMBER;
    std::string serverHostName = PROXY_HOST_NAME;

    //public methods
    void messageArrived(std::array<char,1> message);
    void connectToServer();
    void error(const char *msg);

    //events
    //void event_messageRecieved(msgType messageType);
    //void event_exception(QString exceptionDescription);

signals:
    void sig_messageRecieved(const clientServerProtocol *_protocol, const int msgType,const std::vector<char> &vdata = std::vector<char>());
    void sig_exception(QString ex);
};

#endif // CLIENTSOCKET_H
