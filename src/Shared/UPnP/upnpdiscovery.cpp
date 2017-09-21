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


std::vector<std::string> UPnPDiscovery::discoverDevices(const std::string &searchTarget, const char *localIPv4)
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


    // Bind to port MULTICAST_DISCOVERY_BIND_PORT on all interfaces
    if (bind(udpSocket, (struct sockaddr*)&localSock, sizeof(localSock)) < 0 ){
        perror("bind");
    }

    // Join the multicast group on the local interface. Note that this IP_ADD_MEMBERSHIP option must be called for each local interface over which the multicast datagrams are to be received.
    group.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    group.imr_interface.s_addr = inet_addr(localIPv4);
    setsockopt(udpSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));

    char buff[] = "M-SEARCH * HTTP/1.1\r\n"
                  "HOST:239.255.255.250:1900\r\n"
                  "MAN:\"ssdp:discover\"\r\n"
                  "ST:ssdp:all\r\n"
                  "MX:1\r\n\r\n";
    unsigned buff_size = strlen(buff);
    auto disc_send_res = sendto(udpSocket, buff, strlen(buff), 0, (struct sockaddr*)&groupSock, sizeof groupSock);
    if (disc_send_res < 0) {
        perror("sendto");
        return devices;
    } else if (disc_send_res != buff_size){
        perror("sendto - send bytes not equal to buffer");
        return devices;
    }


    //thetw to recv timeout
#ifdef WIN32
    int iTimeout = 2000;
    setsockopt(udpSocket,
                       SOL_SOCKET,
                       SO_RCVTIMEO,
                       (const char *)&iTimeout,
                       sizeof(iTimeout) );
#else
    struct timeval tv;
    tv.tv_sec = 2;  // 90 Secs Timeout
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(udpSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
#endif

    char discovery_response_buffer[1024] = {};
    struct sockaddr_in si_other;
    socklen_t slen = sizeof(si_other);


    while(true){

        if (recvfrom(udpSocket, discovery_response_buffer, 1024, 0, (struct sockaddr *) &si_other, &slen) < 0) {
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
                std::cout << discovery_response_buffer << std::endl;
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
