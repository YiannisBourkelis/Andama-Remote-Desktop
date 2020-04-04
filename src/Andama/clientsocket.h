/* ***********************************************************************
 * Andama
 * (C) 2017 by Yiannis Bourkelis (hello@andama.org)
 *
 * This file is part of Andama.
 *
 * Andama is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Andama is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Andama.  If not, see <http://www.gnu.org/licenses/>.
 * ***********************************************************************/

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#ifdef WIN32
//#define NOMINMAX
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
    bool stopThreadFlag = false;
    int localPort = 0; //otan syndeetai ston proxy, pernei tin local port pou dinei aftomata to systhma

    //public methods
    void messageArrived(std::array<char,1> message);
    void connectToServer();
    void error(const char *msg);
    void stopThread();

    //events
    //void event_messageReceived(msgType messageType);
    //void event_exception(QString exceptionDescription);

private:
    //private variables

signals:
    void sig_messageReceived(const clientServerProtocol *_protocol, const int msgType,const std::vector<char> &vdata = std::vector<char>());
    void sig_exception(QString ex);
};

#endif // CLIENTSOCKET_H
