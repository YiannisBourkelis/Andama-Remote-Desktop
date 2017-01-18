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


UPnPDiscovery::UPnPDiscovery()
{

}

QUrl UPnPDiscovery::getDeviceLocationXmlUrl()
{
    int loop = 1; // Needs to be on to get replies from clients on the same host
    int ttl = 4;

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
        throw std::runtime_error("WSAStartup failed");
    }
#endif

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        throw std::runtime_error("socket");
    }

    // Multicast message will be sent to 239.255.255.250:1900
    struct sockaddr_in destadd;
    memset(&destadd, 0, sizeof(destadd));
    destadd.sin_family = AF_INET;
    destadd.sin_port = htons(1900);
    if (inet_pton(AF_INET, "239.255.255.250", &destadd.sin_addr) < 1) {
        perror("inet_pton dest");
        throw std::runtime_error("inet_pton dest");
    }

    // Listen on all interfaces on port 26389. Random port TODO: make it constant
    //PROSOXI!!! paratirisa oti sta windows 7 gia kapoio logo den almvanw apantisi
    //sto discovery request, an i porta einai mikroteri apo 12000
    struct sockaddr_in interface_addr;
    memset(&interface_addr, 0, sizeof(interface_addr));
    interface_addr.sin_family = AF_INET;
    interface_addr.sin_port = htons(26389);
    interface_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Got to have this to get replies from clients on same machine
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, (char*) &loop, sizeof(loop)) < 0){
        perror("setsockopt loop");
        throw std::runtime_error("setsockopt loop");
    }

    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL,(char*) &ttl, sizeof(ttl)) < 0){
        perror("setsockopt ttl");
        throw std::runtime_error("setsockopt ttl");
    }

    // Bind to port 26389 on all interfaces
    if (bind(sock, (struct sockaddr *)&interface_addr,
        sizeof(struct sockaddr_in)) < 0) {
        perror("bind");
        throw std::runtime_error("bind");
    }

    char discovery_request_buffer[1024];
    strcpy(discovery_request_buffer,   "M-SEARCH * HTTP/1.1\r\n"
                                       "Host: 239.255.255.250:1900\r\n"
                                       "Man: \"ssdp:discover\"\r\n"
                                       "ST: upnp:rootdevice\r\n"
                                       "MX: 3\r\n"
                                       "\r\n");
    if (sendto(sock, discovery_request_buffer, strlen(discovery_request_buffer), 0, (struct sockaddr*)&destadd,
           sizeof(destadd)) < 0) {
        std::cout << WSAGetLastError() << std::endl;
        perror("sendto");
        throw std::runtime_error("sendto");
    }

    char discovery_response_buffer[1024];
    if (recvfrom(sock, discovery_response_buffer, sizeof(discovery_response_buffer)-1, 0, NULL, NULL) < 0) {
        std::cout << WSAGetLastError() << std::endl;
        perror("recvfrom");
        throw std::runtime_error("recvfrom");
    }


    printf("%s\n", discovery_response_buffer);

    std::string ret(discovery_response_buffer);
    std::string loc(ret.substr(ret.find("LOCATION:")));
    auto f = loc.find("\r\n");
    std::string loc2(loc.substr(9,f-9));
    std::cout << loc2 << std::endl;

    QUrl locationUrl(QString::fromStdString(loc2));
    std::cout << "\r\n" << "Host: " << locationUrl.host().toStdString()<<"\r\nPort: "<< locationUrl.port() << std::endl;


#ifdef WIN32
    if (closesocket(sock) < 0) {
#else
    if (close(sock) < 0) {
#endif
        perror("close");
        throw std::runtime_error("close");
    }

    return locationUrl;
}
