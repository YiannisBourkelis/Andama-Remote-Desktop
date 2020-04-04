/* ***********************************************************************
 * Andama
 * (C) 2014 by Yiannis Bourkelis (yiannis -{at}- grbytes.com)
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

#ifndef P2PSERVER_H
#define P2PSERVER_H

#include "helperfuncs.h"
#include <iostream> //std::cout
#include "vector"
#include <mutex>
#include <atomic>
#include <QThread>
#include "qdebug.h"
#include <errno.h>
#include <thread>
#include <array>
#include <random>
#include <future>
#include <atomic>

#include "clientserver.h" //TODO: isws den xreiazetai
#include "clientserverprotocol.h"
#include "../Shared/Cryptography/openssl_aes.h"


using namespace helperfuncs;

class P2PServer : public QThread
{
    Q_OBJECT

public:

    int PORT_NUMBER = 17332; //server listening port
    const int MAX_PORT_NUMBER = PORT_NUMBER + 1000; //i maximum server bind port pou tha ginei dokimi

    P2PServer();
    void setRemoteComputerOS(OS os);
    void setConnectionState(connectionState state);
    void stopThread();
    void setPassword(std::string password);
    const std::vector<openssl_aes::byte> &getLocalPasswordHash() const;
    const std::vector<openssl_aes::byte> &getLocalPasswordDoubleHash() const;
    const std::vector<openssl_aes::byte> &getLocalPasswordDoubleMD5() const;

    #ifdef WIN32
    void accept_client_messages(const SOCKET socketfd, const unsigned long clientIP);
    void future_thread_accept_client_messages(const SOCKET socketfd, const unsigned long clientIP);
    SOCKET activeClientSocket;
    #else
    void accept_client_messages(const int socketfd, const in_addr_t clientIP);
    void future_thread_accept_client_messages(const int socketfd, const in_addr_t clientIP);
    int activeClientSocket;
    #endif

    void start_p2pserver();

    bool isP2PCientConnected;

    bool hasConnectedClientThreadsRunning();
    void sendMouseCursorType(Qt::CursorShape cursorShape);
private:
    //variables
    std::mutex send_mutex; //sygxronismos sockets.TODO (xreiazetai sygxronismos wste se kamia periptwsi na mi ginetai apo diaforetika thread lipsi i apostoli sto idio socket
    OS _remoteComputerOS;
    bool _stopThread = false;
    std::string m_password;
    std::vector<openssl_aes::byte> m_passwordHash256;
    std::vector<openssl_aes::byte> m_passwordDoubleHash256;
    std::vector<openssl_aes::byte> m_localPasswordDoubleMD5;

#ifdef WIN32
    SOCKET listensocket = INVALID_SOCKET;
#else
    int listensocket;
#endif

    connectionState m_connection_state = disconnected;
    std::mutex connection_state_mutex;

    void sendDisconnectFromRemoteComputer(int socket);
signals:
    void sig_messageReceived(const clientServerProtocol *client, const int msgType, const std::vector<char> &vdata = std::vector<char>());
    void sig_exception(QString ex);

protected:
    void run(void);

};

#endif // P2PSERVER_H
