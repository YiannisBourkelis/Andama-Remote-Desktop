/* ***********************************************************************
 * Andama
 * (C) 2014 by Yiannis Bourkelis (yiannis@grbytes.com)
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

#ifndef CLIENTSERVER_H
#define CLIENTSERVER_H

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
#include "PasswordProtection.h"

using namespace helperfuncs;

const unsigned int PASSWORD_LENGTH = 4; // default 6

enum connectionState {
    disconnected = 0,
    connectedWithProxy = 1,
    connectedWithOtherAsClient = 2,
    connectedWithOtherAsServer = 3
};

//prosdiorizei ean o client syndeetai ston host mesw tou proxy
//h apeftheias (peer-to-peer)
enum connectMessageType {
    proxy = 0,
    direct = 1
};

class clientserver : public QThread
{
        Q_OBJECT

public:
    clientserver();
    const int PORT_NUMBER = 8099;
    static const int MSG_ID = 0;
    static const int MSG_CONNECTION_ACCEPTED = 1; //to remote pc me enimerwse oti to aitima syndesis egine apodekto
    static const int MSG_REMOTE_CLIENT_ACCEPTED = 2; //apodextika to aitima syndesis pou ekane to remote pc.ksekinw na stelnw screenshots
    static const int MSG_SCREENSHOT = 3;
    static const int MSG_SCREENSHOT_DIFF = 4;
    static const int MSG_SCREENSHOT_REQUEST = 5;
    static const int MSG_SCREENSHOT_DIFF_REQUEST = 6;
    static const int MSG_MOUSE = 7;
    static const int MSG_KEYBOARD = 8;
    static const int MSG_CONNECT_ID_NOT_FOUND = 9; // epistrefetai apo ton server otan to Remote ID pou zitithike apo to connect den vrethike
    static const int MSG_NO_INTERNET_CONNECTION = 10;
    static const int MSG_NO_PROXY_CONNECTION = 11;
    static const int MSG_REMOTE_COMPUTER_DISCONNECTED = 12;
    static const int MSG_LOCAL_PASSWORD_GENERATED = 13;
    static const int MSG_BAN_IP_WRONG_ID = 14; // synolika 5 bytes
    static const int MSG_WARNING_BAN_IP_WRONG_ID = 15; // synolika 2 bytes
    static const int MSG_CONNECT_PASSWORD_NOT_CORRECT = 16;
    static const int MSG_BAN_IP_WRONG_PWD = 17; // synolika 1 byte
    static const int MSG_WARNING_BAN_IP_WRONG_PWD = 18; // synolika 2 byte
    static const int MSG_ERROR_CANNOT_CONNECT_SAME_ID = 19; // synolika 2 byte
    static const int MSG_ERROR_APP_VERSION_NOT_ACCEPTED = 20; // (mono command)
    static const int MSG_ERROR_PROTOCOL_VERSION_NOT_ACCEPTED = 21; // (mono command)

    //password brute force attack protection
    const int  MAX_WRONG_PWD_TRIES         = 10;  //10; //default: diplasio apo to MAX_WRONG_ID_TRIES_WARNING
    const int  MAX_WRONG_PWD_TRIES_WARNING = 5;  //5;

    void start_protocol();
    void Connect(const std::vector<char> &remoteID, const std::vector<char> &remotePassword);
    void RequestScreenshot();
    void RequestScreenshotDiff();
    void setConnectionState(connectionState state);
    connectionState getConnectionState();
    void sendDisconnectFromRemoteComputer();
    void sendHeartBeat();
    void setRemoteComputerOS(OS os);
    OS getRemoteComputerOS();

    std::string password;

#ifdef WIN32
    void setActiveSocket(const SOCKET socket);
    SOCKET getActiveSocket();

    int _sendmsg(const SOCKET socketfd,      const std::array<char, 1> &command, const std::vector<char> &message = std::vector<char>());
    int _sendmsgPlain(const SOCKET socketfd, const std::array<char, 1> &command, const std::vector<char> &message = std::vector<char>());
    void cleanup(const SOCKET socketfd);
#else
    int getActiveSocket();
    void setActiveSocket(const int socket);

    int _sendmsg(const int socketfd,      const std::array<char, 1> &command, const std::vector<char> &message = std::vector<char>());
    int _sendmsgPlain(const int socketfd, const std::array<char, 1> &command, const std::vector<char> &message = std::vector<char>());
    void cleanup(const int socketfd);
#endif
    void sendMouse(int x, int y, int button, int mouseEvent, int wheelDelta, int wheelDeltaSign, int wheelOrientation);
    void sendKeyboard(int portableVKey, int portableModifiers, int keyEvent);
    //std::recursive_mutex sendmutex;
    std::mutex sendmutex;


signals:
    void sig_messageRecieved(int msgType,const std::vector<char> &cdata = std::vector<char>(),const std::vector<char> &vdata = std::vector<char>());
    void sig_exception(QString ex);

protected:
    void run(void);

private:
#ifdef WIN32
    SOCKET _activeSocket = INVALID_SOCKET;
    int _receivePlain(const SOCKET socketfd, std::vector<char> &charbuffer);
    int _receive(const SOCKET socketfd, std::vector<char> &charbuffer);
#else
    int _activeSocket;
    int _receivePlain(const int socketfd, std::vector<char> &charbuffer);
    int _receive(const int socketfd, std::vector<char> &charbuffer);
#endif

    std::atomic<long> diffRequestCounter;

    void error(const char *msg);
    void proccesCommand(const std::array<char, 1> &command);
    void displayErrno(std::string source);
    void createConnectCommandData(std::vector<char> &all_data, const std::vector<char> &remoteComputerID, const std::vector<char> &remoteComputerPassword);
    bool addWrongPasswordIPProtection(in_addr_t clientIP);
    bool isIPBannedForWrongPasswords(in_addr_t clientIP);
    void resetWrongPasswordIPProtection(in_addr_t clientIP);
    OS _remoteComputerOS;

    std::string generateRandomPassword(int length = PASSWORD_LENGTH);

    connectionState m_connection_state = disconnected;
    std::mutex connection_state_mutex;
    std::mutex protect_password_mutex;
    std::map<in_addr_t,PasswordProtection> protect_password;//krataei tis ip pou exoun kanei apopeira syndesis me password pou einai lathos

    //random password generator
    //random ID generator
    std::string passwordCharset = "123456789123456789abcdefgkmnprstxz123456789"; //52 chars. Thelw na emfanizontai oi arithmpoi me megalyteri syxnotita. Exw aferaisei grammata poy moiazoun me arithoums p.x. o. Exw aferesei grammata poy mporei na mperdepsoun to xristi
    std::random_device rd;
    std::mt19937 mt = std::mt19937(rd());
    std::uniform_int_distribution<unsigned int> distribution = std::uniform_int_distribution<unsigned int>(0, passwordCharset.length() - 1);

    std::vector<char> cachedID; //apothikevei to cachedID pou stalthike apo ton server
};

#endif // CLIENTSERVER_H

