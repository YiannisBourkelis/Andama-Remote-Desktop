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

std::vector<std::string> UPnPDiscovery::discoverDevices(std::string searchTarget)
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
    if (inet_pton(AF_INET, "239.255.255.250", &destadd.sin_addr) < 1) {
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
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));//TODO: isws den xreiazetai
    }

    char discovery_request_buffer[1024];
    strcpy(discovery_request_buffer,   "M-SEARCH * HTTP/1.1\r\n"
                                       "Host: 239.255.255.250:1900\r\n"
                                       "Man: \"ssdp:discover\"\r\n"
                                       "ST: upnp:rootdevice\r\n"
                                       "MX: 3\r\n"
                                       "\r\n");

    for (int i = 0;i < 2; i++){
        int disc_send_res = sendto(sock, discovery_request_buffer, strlen(discovery_request_buffer), 0, (struct sockaddr*)&destadd,
                   sizeof(destadd));
        if (disc_send_res < 0) {
            perror("sendto");
            return devices;
            //throw std::runtime_error("sendto");
        } else if (disc_send_res != strlen(discovery_request_buffer)){
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
    tv.tv_sec = 3;  /* 90 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
#endif
    char discovery_response_buffer[1024];
    bzero(discovery_response_buffer,1024);
    while(true){
    if (recvfrom(sock, discovery_response_buffer, sizeof(discovery_response_buffer)-1, 0, NULL, NULL) < 0) { //TODO: edw na valw recv timeout
        #ifdef WIN32
            std::cout <<
                         "###### [source: displayErrno: " << strerror(WSAGetLastError()) << std::endl;
        #else
            std::cout << getTime() << " " << std::this_thread::get_id() << " " <<
                         "###### [source: " << source <<  "]  displayErrno: " << strerror(errno) << std::endl;
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
        bzero(discovery_response_buffer,1024);
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
