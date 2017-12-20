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

#include "clientsocket.h"

clientSocket::clientSocket()
{
    //protocol = p;
}

void clientSocket::messageArrived(std::array<char, 1> message)
{
    protocol->proccessMessage(message);
}

void clientSocket::error(const char *msg)
{
    std::cout << "ERROR " << msg <<  "\n";
    perror(msg);
    //exit(0);
}

/*
void clientSocket::event_messageReceived(msgType messageType)
{
}

void clientSocket::event_exception(QString exceptionDescription)
{
}
*/

//TODO: einai static afto?
//Detecting endianness programmatically in a C++ program
//http://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
void _displayEndianness()
{
    if ( htonl(47) == 47 ) {
        qDebug("Big endian");
    } else {
        qDebug("Little endian");
    }
}

void clientSocket::connectToServer()
{
    int bytes_recv;
    struct sockaddr_in serv_addr;
    struct hostent *SERVER;

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

    protocol->activeSocket = socket(AF_INET, SOCK_STREAM, 0);
#ifdef WIN32
    if (protocol->activeSocket == INVALID_SOCKET) {
#else
    if (protocol->activeSocket < 0){
#endif
        error("ERROR opening socket");
        return;
    }

    //SERVER = gethostbyname("mailgate.filoxeni.com");
    SERVER = gethostbyname("andamaproxy-us-west.filoxeni.com");
    //SERVER = gethostbyname("localhost");

    if (SERVER == NULL) {
        //event_messageReceived(MSG_NO_INTERNET_CONNECTION);
        emit sig_messageReceived(protocol,MSG_NO_INTERNET_CONNECTION);
        fprintf(stderr,"ERROR, no such host\n");

#ifdef WIN32
        closesocket(protocol->activeSocket);
#else
        close(protocol->activeSocket);
#endif

        //kathistewrw ton termtismo tou thread
        //wste na min termatizei tin cpu
        std::chrono::milliseconds sleep_dura(1000);
        std::this_thread::sleep_for(sleep_dura);

        //edw xtypaei ean den exei syndesi sto internet o ypologistis
        return;
        //exit(0);
    }

    memset((char *) &serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;


    bcopy((char *)SERVER->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         SERVER->h_length);

    //serv_addr.sin_addr.s_addr=inet_addr("192.168.32.20"); // <------------- local server

    //SIMANTIKO: kanw disable to nagle algorithm. meiwnei to latency.
    int flag = 1;

    setsockopt(protocol->activeSocket,      /* socket affected */
                            IPPROTO_TCP,     /* set option at TCP level */
                            TCP_NODELAY,     /* name of option */
                            (char *) &flag,  /* the cast is historical cruft */
                            sizeof(int));    /* length of option value */

        //sybdesi mesw proxy
        serv_addr.sin_port = htons(PROXY_PORT_NUMBER);

    int conres = ::connect(protocol->activeSocket,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
    if (conres < 0)
    {
        std::cout << "ERROR connecting. result: " << conres << "\n";

#ifdef WIN32
        closesocket(protocol->activeSocket);
#else
        close(protocol->activeSocket);
#endif
        //if (remotePort == 0){
         //   event_messageReceived(MSG_NO_PROXY_CONNECTION);
        //}
        //edw xtypaei ean yparxei syndesi sto internet alla o proxy den trexei
        //error("ERROR connecting");

        return;
     }

    qDebug("Remote Cotrol BETA - (c) 2015 Yiannis Bourkelis \n\n");
    _displayEndianness();

    //ksekinw tin epesksergasia twn commands
    std::array<char,1> cmdbuffer;

    //thetw socket options
    //int send_timeout = 30000;
    //int intlen = sizeof(int);
   // setsockopt(this->getActiveSocket(), SOL_SOCKET, SO_SNDTIMEO, (char*)&send_timeout, intlen);
    //getsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, &ilen);

    while(true){
        try
        {
            //qDebug("1 -----> Waitting for command...");

            bytes_recv = recv(protocol->activeSocket, &cmdbuffer[0], 1, 0);

            if (bytes_recv == 0){
                std::cout << std::this_thread::get_id() << " " <<
                             "######### --- Main command loop disconnected from server. ---- ########" << " " <<
                           "####  recv return 0 bytes. [MAIN command loop]. Returning from function." << std::endl;

    #ifdef WIN32
                closesocket(protocol->activeSocket);
    #else
                close(protocol->activeSocket);
    #endif
                //event_messageReceived(MSG_NO_PROXY_CONNECTION);
                emit sig_messageReceived(protocol,MSG_NO_PROXY_CONNECTION);
                return;
            }
            else if (bytes_recv == -1){
                displayErrno("void clientsocket::connectToServer() ## bytes_recv == -1 ## [MAIN command loop]. Returning from function.");

    #ifdef WIN32
                closesocket(protocol->activeSocket);
    #else
                close(protocol->activeSocket);
    #endif
                //event_messageReceived(MSG_NO_PROXY_CONNECTION);
                emit sig_messageReceived(protocol,MSG_NO_PROXY_CONNECTION);
                return;
            }

            //qDebug("2 -----> Command received: %s. Bytes: %i. Ksekina epeksergasia tou command",cmdbuffer.data(),bytes_recv);
            messageArrived(cmdbuffer);

        }
        catch(std::exception& ex)
        {
            qDebug("----> EXCEPTION sto start_protocol ::: %s",ex.what());
            //event_exception(QString::fromUtf8(ex.what()));
            emit sig_exception(QString::fromUtf8(ex.what()));
        }

        catch( ... )
        {
            qDebug("----> EXCEPTION :: start_protocol unhandled exception");
        }

    } // while


    #ifdef WIN32
    //closesocket(sockfd);
    #else
    //close(sockfd);
    #endif

    return;
}

