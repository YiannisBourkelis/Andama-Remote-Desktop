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

#include "upnpdiscovery.h"
#include <stdexcept>
#include <algorithm>
#include "../AndamaHeaders/socket_functions.h"


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
#include <arpa/inet.h>
#endif

#include <iostream>
#include <thread>
#include <chrono>


UPnPDiscovery::UPnPDiscovery()
{

}


std::vector<std::string> UPnPDiscovery::discoverDevices(const std::string &searchTarget)
{
    // Structs needed
    struct in_addr localInterface;
    struct sockaddr_in groupSock;
    struct sockaddr_in localSock;
    struct ip_mreq group;

    std::vector<std::string> devices; // devices found

#ifdef WIN32
    SOCKET udpSocket;
#else
    int udpSocket;
#endif

#ifdef WIN32
    // Initialize Winsock
    int iResult;
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return devices;
        //throw std::runtime_error("WSAStartup failed");
    }
#endif

    // Create the Socket
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // Enable SO_REUSEADDR to allow multiple instances of this application to receive copies of the multicast datagrams.
    int reuse = 1;
    setsockopt(udpSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse));

    // Initialize the group sockaddr structure with a group address of 239.255.255.250 and port 1900.
    memset((char *) &groupSock, 0, sizeof(groupSock));

    groupSock.sin_family = AF_INET;
    groupSock.sin_addr.s_addr = inet_addr("239.255.255.250");
    groupSock.sin_port = htons(1900);

    // Disable loopback so you do not receive your own datagrams.
    char loopch = 0;
    setsockopt(udpSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch));

    // Set local interface for outbound multicast datagrams. The IP address specified must be associated with a local, multicast capable interface.
    localInterface.s_addr = inet_addr("192.168.1.228");

    setsockopt(udpSocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface));

    // Bind to the proper port number with the IP address specified as INADDR_ANY.
    memset((char *) &localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(33589);
    localSock.sin_addr.s_addr = INADDR_ANY;
    bind(udpSocket, (struct sockaddr*)&localSock, sizeof(localSock));

    // Join the multicast group on the local interface. Note that this IP_ADD_MEMBERSHIP option must be called for each local interface over which the multicast datagrams are to be received.
    group.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    group.imr_interface.s_addr = inet_addr("192.168.1.228");
    setsockopt(udpSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));

    //char *request = "M-SEARCH * HTTP/1.1\r\nHOST:239.255.255.250:1900\r\nMAN:\"ssdp:discover\"\r\nST:ssdp:all\r\nMX:3\r\n\r\n";
    char buff[]= "M-SEARCH * HTTP/1.1\r\n"\
    "HOST: 239.255.255.250:1900\r\n"\
    "MAN: \"ssdp:discover\"\r\n"\
    "MX: 3\r\n"\
    "ST: udap:rootservice\r\n"\
    "USER-AGENT: RTLINUX/5.0 UDAP/2.0 printer/4\r\n\r\n";

    int  ret1 = sendto(udpSocket, buff, strlen(buff), 0, (struct sockaddr*)&groupSock, sizeof groupSock);


    char discovery_response_buffer[1024] = {};
    struct sockaddr_in si_other;
    socklen_t slen = sizeof(si_other);
    while(true){

    if (recvfrom(udpSocket, discovery_response_buffer, 1024, 0, (struct sockaddr *) &si_other, &slen) < 0) { //TODO: edw na valw recv timeout
        #ifdef WIN32
            std::cout << "###### displayErrno: " << strerror(WSAGetLastError()) << std::endl;
        #else
            std::cout << "###### displayErrno: " << strerror(errno) << std::endl;
        #endif
            perror("recvfrom - device discovery");
            break;
        }
        //std::cout << "==== discovery_response_buffer=====\r\n"<< discovery_response_buffer << std::endl;
        //i
        if (std::find(devices.begin(),devices.end(),discovery_response_buffer) == devices.end()){
            devices.push_back(discovery_response_buffer);
        }
    }

#ifdef WIN32
    if (closesocket(udpSocket) < 0) {
#else
    if (close(udpSocket) < 0) {
#endif
        perror("close");
    }


    return devices;
}

/*
std::vector<std::string> UPnPDiscovery::discoverDevices(const std::string &searchTarget)
{
    int loop = 1; // Needs to be on to get replies from clients on the same host
    int ttl = 4;

    std::vector<std::string> devices; // oi syskeves pou vrethikan

#ifdef WIN32
    SOCKET sock;
#else
    int sock;
#endif

#ifdef WIN32
    // Initialize Winsock
    int iResult;
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return devices;
        //throw std::runtime_error("WSAStartup failed");
    }
#endif

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return devices;
        //throw std::runtime_error("socket");
    }

    // Multicast message will be sent to 239.255.255.250:1900
    struct sockaddr_in destadd;
    memset(&destadd, 0, sizeof(destadd));
    destadd.sin_family = AF_INET;
    destadd.sin_port = htons(1900);

    if (inet_pton_mingw(AF_INET, "239.255.255.250", inet_ntoa(destadd.sin_addr)) < 1) {
        perror("inet_pton dest");
        return devices;
        //throw std::runtime_error("inet_pton dest");
    }

    // Listen on all interfaces on port MULTICAST_DISCOVERY_BIND_PORT.
    //PROSOXI!!! paratirisa oti sta windows 7 gia kapoio logo den almvanw apantisi
    //sto discovery request, an i porta einai mikroteri apo 12000
    struct sockaddr_in interface_addr;
    memset(&interface_addr, 0, sizeof(interface_addr));
    interface_addr.sin_family = AF_INET;
    interface_addr.sin_port = htons(MULTICAST_DISCOVERY_BIND_PORT);
    interface_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Got to have this to get replies from clients on same machine
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*) &loop, sizeof(loop)) < 0){
        perror("setsockopt loop");
        return devices;
        //throw std::runtime_error("setsockopt loop");
    }

    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,(char*) &ttl, sizeof(ttl)) < 0){
        perror("setsockopt ttl");
        return devices;
        //throw std::runtime_error("setsockopt ttl");
    }


    //Xreiazetai wste se periptwsi crash na ginetai reuse to socket pou einai se state CLOSE_WAIT
    //mporw na to vrw se macos me: netstat -anp tcp | grep port_number
    int reuse = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

#ifdef SO_REUSEPORT
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0)
        perror("setsockopt(SO_REUSEPORT) failed");
#endif


    // Bind to port MULTICAST_DISCOVERY_BIND_PORT on all interfaces
    unsigned short bindPortBindIncrementer = 0;
    while (bind(sock, (struct sockaddr *)&interface_addr,
        sizeof(struct sockaddr_in)) < 0 && bindPortBindIncrementer < 100) {
        perror("bind");
        bindPortBindIncrementer++;
        interface_addr.sin_port = htons(MULTICAST_DISCOVERY_BIND_PORT + bindPortBindIncrementer);
    }

    std::string discovery_request_buffer ("M-SEARCH * HTTP/1.1\r\n"
                                          "Host: 239.255.255.250:1900\r\n"
                                          "Man: ""ssdp:discover""\r\n"
                                          "ST: " + searchTarget + "\r\n"
                                          "MX: 3\r\n"
                                          "\r\n");

    int _discovery_request_buffer_size = discovery_request_buffer.length();

    for (int i = 0;i < 2; i++){
        int disc_send_res = sendto(sock, discovery_request_buffer.c_str(), _discovery_request_buffer_size, 0, (struct sockaddr*)&destadd,
                   sizeof(destadd));
        if (disc_send_res < 0) {
            perror("sendto");
            return devices;
        } else if (disc_send_res != _discovery_request_buffer_size){
            perror("sendto - send bytes not equal to buffer");
            return devices;
        }
    }


    //thetw to recv timeout
#ifdef WIN32
    int iTimeout = 3000;
    setsockopt(sock,
                       SOL_SOCKET,
                       SO_RCVTIMEO,
                       (const char *)&iTimeout,
                       sizeof(iTimeout) );
#else
    struct timeval tv;
    tv.tv_sec = 3;  // 90 Secs Timeout
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
#endif
    char discovery_response_buffer[1024] = {};
    while(true){
    if (recvfrom(sock, discovery_response_buffer, sizeof(discovery_response_buffer)-1, 0, NULL, NULL) < 0) { //TODO: edw na valw recv timeout
        #ifdef WIN32
            std::cout << "###### displayErrno: " << strerror(WSAGetLastError()) << std::endl;
        #else
            std::cout << "###### displayErrno: " << strerror(errno) << std::endl;
        #endif
            perror("recvfrom - device discovery");
            break;
        }
        //std::cout << "==== discovery_response_buffer=====\r\n"<< discovery_response_buffer << std::endl;
        //logo UDP, mporei to idio device na stalei parapanw apo mia fora,
        //opote an yparxei idi den to ksanakataxwrw
        if (std::find(devices.begin(),devices.end(),discovery_response_buffer) == devices.end()){
            devices.push_back(discovery_response_buffer);
        }
    }

#ifdef WIN32
    if (closesocket(sock) < 0) {
#else
    if (close(sock) < 0) {
#endif
        perror("close");
    }


    return devices;
}
*/
