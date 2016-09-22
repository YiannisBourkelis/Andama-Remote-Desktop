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

#include <clientserver.h> //TODO: isws den xreiazetai


using namespace helperfuncs;

class P2PServer : public QThread
{
    Q_OBJECT

public:

    //server listening port
    const int PORT_NUMBER = 8095;


    //ta commands pou anagnwrizei o proxy
    const std::array<char,1> CMD_PROTOCOL = {{'P'}};
    const std::array<char,1> CMD_CONNECT = {{'C'}};
    const std::array<char,1> CMD_CONNECT_ID_NOT_FOUND = {{'c'}}; // stelnetai apo ton server otan den vrethei to id pou zitithike apo CMD_CONNECT
    //const std::array<char,1> CMD_QUIT = {{'Q'}};
    const std::array<char,1> CMD_ACCEPT = {{'A'}};
    const std::array<char,1> CMD_ID = {{'I'}};
    const std::array<char,1> CMD_SCREENSHOT = {{'S'}};
    const std::array<char,1> CMD_SCREENSHOT_DIFF = {{'s'}};
    const std::array<char,1> CMD_REQUEST_SCREENSHOT = {{'R'}};
    const std::array<char,1> CMD_REQUEST_SCREENSHOT_DIFF = {{'r'}};
    const std::array<char,1> CMD_MOUSE = {{'M'}}; // mouse
    const std::array<char,1> CMD_KEYBOARD = {{'K'}}; // KEYBOARD
    const std::array<char,1> CMD_DISCONNECT_FROM_REMOTE_COMPUTER = {{'D'}}; // disconnect from computer
    const std::array<char,1> CMD_HEART_BEAT = {{'H'}};
    const std::array<char,1> CMD_BAN_IP_WRONG_ID = {{'B'}}; // synolika 5 bytes
    const std::array<char,1> CMD_WARNING_BAN_IP_WRONG_ID= {{'W'}}; // synolika 2 bytes
    const std::array<char,1> CMD_CONNECT_PASSWORD_NOT_CORRECT= {{'p'}}; // synolika 1 byte
    const std::array<char,1> CMD_BAN_IP_WRONG_PWD = {{'b'}}; //synolika ena byte (mono to command)
    const std::array<char,1> CMD_WARNING_BAN_IP_WRONG_PWD= {{'w'}}; // synolika 2 bytes
    const std::array<char,1> CMD_ERROR_APP_VERSION_NOT_ACCEPTED = {{'V'}}; // epistrefei to URL pros ti selida pou tha katevasei o xristis tin neoteri ekdosi tis efarmogis
    //const std::array<char,1> CMD_ERROR_PROTOCOL_VERSION_NOT_ACCEPTED = {{'v'}}; // (mono command)

    const std::array<char,1> CMD_P2P_PROTOCOL_OK = {{'d'}}; //o p2pserver stelnei afto to command enos byte otan to client protocol einai ok
    const std::array<char,1> CMD_P2P_CONNECT = {{'N'}}; //o p2pserver stelnei afto to command enos byte otan to client protocol einai ok


    P2PServer();
    void setRemoteComputerOS(OS os);
    void setConnectionState(connectionState state);

    #ifdef WIN32
    void accept_client_messages(const SOCKET socketfd, const unsigned long clientIP);
    SOCKET activeClientSocket;
    #else
    void accept_client_messages(const int socketfd, const in_addr_t clientIP);
    int activeClientSocket;
    #endif

    void start_p2pserver();

    bool isP2PCientConnected;
    std::string password;

    #ifdef WIN32
    int _sendmsg(const SOCKET socketfd, const std::array<char, 1> &command, const std::vector<char> &message);
    #else
    int _sendmsg(const int socketfd,    const std::array<char, 1> &command, const std::vector<char> &message);
    #endif

private:
    std::mutex send_mutex; //sygxronismos sockets.TODO (xreiazetai sygxronismos wste se kamia periptwsi na mi ginetai apo diaforetika thread lipsi i apostoli sto idio socket
    OS _remoteComputerOS;

    #ifdef WIN32
    int _sendmsgPlain(const SOCKET socketfd, const std::array<char, 1> &command, const std::vector<char> &message = std::vector<char>());
    #else
    int _sendmsgPlain(const int socketfd, const std::array<char, 1> &command, const std::vector<char> &message = std::vector<char>());
    #endif

    #ifdef WIN32
    int _receivePlain(const SOCKET socketfd, std::vector<char> &charbuffer);
    #else
    int _receivePlain(const int socketfd, std::vector<char> &charbuffer);
    #endif

    #ifdef WIN32
    int _receive(const SOCKET socketfd, std::vector<char> &charbuffer);
    #else
    int _receive(const int socketfd, std::vector<char> &charbuffer);
    #endif

    connectionState m_connection_state = disconnected;
    std::mutex connection_state_mutex;

signals:
    void sig_messageRecieved(const clientserver *client, const int msgType,const std::vector<char> &vdata = std::vector<char>());
    void sig_exception(QString ex);

protected:
    void run(void);

};

#endif // P2PSERVER_H
