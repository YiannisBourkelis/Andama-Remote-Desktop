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

#include "serversocket.h"

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
#include <csignal> // xreiazetai gia na ginei compile se linux gia tin signal
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

serverSocket::serverSocket()
{

}

void serverSocket::startServer(int listen_port)
{
    int newsockfd;

    //std::set_terminate(onTerminate);

    //gia na mi prokaleitai crash otan paw na grapsw se socket pou exei kleisei
    //http://stackoverflow.com/questions/108183/how-to-prevent-sigpipes-or-handle-them-properly
    #ifdef WIN32
        SOCKET socketfd = INVALID_SOCKET;
    #else
        int socketfd;
    signal(SIGPIPE, SIG_IGN);
    #endif

    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;


#ifdef WIN32
    // Initialize Winsock
    int iResult;
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return;
    }
#endif

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
#ifdef WIN32
        if (socketfd == INVALID_SOCKET) {
#else
        if (socketfd < 0){
#endif
        // >>>>>>>>>>> error("ERROR opening socket");
            std::cout << "ERROR opening socket";
            return;
        }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(listen_port);

    //Xreiazetai wste se periptwsi crash na ginetai reuse to socket pou einai se state CLOSE_WAIT
    //mporw na to vrw se macos me: netstat -anp tcp | grep port_number
    int reuse = 1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

#ifdef SO_REUSEPORT
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEPORT) failed");
#endif

    if (bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        // >>>>>>>>>>> error("ERROR on binding");
        std::cout << "p2pserver ERROR on binding" << std::endl;
        return;
    }

    listen(socketfd, 5);
    std::cout << "Listenning for connections on port: " << listen_port << std::endl;

    clilen = sizeof(cli_addr);

    while (true) {
        try {
            newsockfd = accept(socketfd, (struct sockaddr *) &cli_addr, &clilen);
            //int pHandle=new int;
            if (newsockfd < 0) {
                // >>>>>>>>>>> error("ERROR on accept");
            }

            std::cout << " New client accepted. Kalw accept_client_messages while loop se neo thread." << std::endl;

            //thetw to recv timeout
#ifdef WIN32
            int iTimeout = 90000;
            setsockopt(newsockfd,
                               SOL_SOCKET,
                               SO_RCVTIMEO,
                               (const char *)&iTimeout,
                               sizeof(iTimeout) );
#else
            struct timeval tv;
            tv.tv_sec = 90;  /* 90 Secs Timeout */
            tv.tv_usec = 0;  // Not init'ing this can cause strange errors
            setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
#endif
            //SIMANTIKO: kanw disable to nagle algorithm. meiwnei to latency.
            int flag = 1;
            setsockopt(newsockfd,                    /* socket affected */
                                    IPPROTO_TCP,     /* set option at TCP level */
                                    TCP_NODELAY,     /* name of option */
                                    (char *) &flag,  /* the cast is historical cruft */
                                    sizeof(int));    /* length of option value */


            //diaxeirizomai ton neo client se neo thread.
            //auto t = std::thread(&p2pServerProtocol.proccessMessage();, this, newsockfd, cli_addr.sin_addr.s_addr);
            //t.detach();
        } catch ( std::exception& ex) {
            std::cout << "Accept loop exception: " << ex.what() << std::endl;
        }
        catch ( ... ) {
            std::cout << "Unknown error in main loop" << std::endl;
        }
    } /* end of while */
}
