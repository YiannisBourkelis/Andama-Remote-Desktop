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

#include "upnpaddportmapping.h"

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

#include "../Shared/AndamaHeaders/socket_functions.h"


#include <iostream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

UPnPAddPortMapping::UPnPAddPortMapping()
{

}

AddPortMappingResponse UPnPAddPortMapping::AddPortMapping(const std::string &newRemoteHost,
                                  const int newExternalPort,
                                  const std::string &newProtocol,
                                  const int newInternalPort,
                                  const std::string &newInternalClient,
                                  const int newEnabled,
                                  const std::string &newPortMappingDescription,
                                  const int newLeaseDuration,
                                  const DeviceResponse &portmappingDevice)
{
    int sock = getClientSocket(portmappingDevice.descriptionUrl.host().toStdString(), portmappingDevice.descriptionUrl.port());

    AddPortMappingResponse addPortMappingResponse;
    addPortMappingResponse.statusCode = 0;

    std::cout << "\r\n\r\n ========================== \r\n\r\n";

    std::string soapbody( "<?xml version=\"1.0\"?>"
                          "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                          "<s:Body><m:AddPortMapping xmlns:m=\"urn:schemas-upnp-org:service:" + portmappingDevice.serviceName + "\">"
                          "<NewRemoteHost>" + newRemoteHost + "</NewRemoteHost>"
                          "<NewExternalPort>" + std::to_string(newExternalPort) + "</NewExternalPort>"
                          "<NewProtocol>" + newProtocol + "</NewProtocol>"
                          "<NewInternalPort>" + std::to_string(newInternalPort) + "</NewInternalPort>"
                          "<NewInternalClient>" + newInternalClient + "</NewInternalClient>"
                          "<NewEnabled>" + std::to_string(newEnabled) + "</NewEnabled>"
                          "<NewPortMappingDescription>" + newPortMappingDescription + "</NewPortMappingDescription>"
                          "<NewLeaseDuration>" + std::to_string(newLeaseDuration) + "</NewLeaseDuration>"
                          "</m:AddPortMapping>"
                          "</s:Body>"
                          "</s:Envelope>"
                          );


    std::string soapbodylength (std::to_string(soapbody.length()));

    std::string headerAndSoapBody ( "POST " + portmappingDevice.controlURL + " HTTP/1.1\r\n"
                                    "Host: " + portmappingDevice.descriptionUrl.host().toStdString() + ":" + std::to_string(portmappingDevice.descriptionUrl.port()) + "\r\n"
                                    "User-Agent: AndamaUPnP, UPnP/1.0\r\n"
                                    "Content-Length: " + soapbodylength + "\r\n"
                                    "Content-Type: text/xml\r\n"
                                    "SOAPAction: ""urn:schemas-upnp-org:service:" + portmappingDevice.serviceName + "#AddPortMapping""\r\n"
                                    "Connection: Close\r\n"
                                    "Cache-Control: no-cache\r\n"
                                    "Pragma: no-cache\r\n\r\n" + soapbody
                                   );


    std::cout << "====== controlURL " << portmappingDevice.controlURL << std::endl;
    std::cout << "\r\n========== will send soap message \r\n" << headerAndSoapBody << "\r\n";

    std::vector<char> vector_char_headerAndSoapBody(headerAndSoapBody.begin(),headerAndSoapBody.end());
    _sendmsgPlain(sock, vector_char_headerAndSoapBody);

    //thetw to recv timeout
#ifdef WIN32
    int iTimeout = 10000;
    setsockopt(sock,
                       SOL_SOCKET,
                       SO_RCVTIMEO,
                       (const char *)&iTimeout,
                       sizeof(iTimeout) );
#else
    struct timeval tv;
    tv.tv_sec = 10;  // 10 Secs Timeout
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
#endif

    char addPortMappingReply[1024];
    std::string addportreply;
    int recv_bytes = 0;
    while ((recv_bytes = recv(sock, addPortMappingReply, sizeof(addPortMappingReply), 0)) > 0){ //TODO: edw na valw recv timeout
        addportreply += addPortMappingReply;
    }
    std::cout << "\r\n server reply \r\n" << addportreply;

    if (recv_bytes < 0){
#ifdef WIN32
        std::cout << "###### UPnPAddPortMapping::AddPortMapping >> recv displayErrno: " << strerror(WSAGetLastError()) << std::endl;
#else
        std::cout << "###### UPnPAddPortMapping::AddPortMapping >> recv displayErrno: " << strerror(errno) << std::endl;
#endif
        perror("recv - UPnPAddPortMapping::AddPortMapping");
    }

    addPortMappingResponse.deviceInfo = portmappingDevice;

    if (addportreply.find("200 OK") < addportreply.length()){
        std::cout << "found index: " << addportreply.find("200 OK") << " of length: " << addportreply.length();
        addPortMappingResponse.statusCode=200;
        addPortMappingResponse.internalPort = newInternalPort;
        addPortMappingResponse.portMappingDescription = newPortMappingDescription;
        addPortMappingResponse.remotePort = newExternalPort;
        addPortMappingResponse.rawResponse = std::string(addportreply);
    }else {
        unsigned rn = addportreply.find(" ");
        if (rn + 4 < addportreply.length()){
            std::string status = addportreply.substr(rn + 1, 3);
            QString s = QString::fromStdString(status);
            if (s.toShort()){
                addPortMappingResponse.statusCode = s.toShort();
            }
        }
    }


    #ifdef WIN32
    closesocket(sock);
    #else
    close(sock);
    #endif

    return addPortMappingResponse;
}
