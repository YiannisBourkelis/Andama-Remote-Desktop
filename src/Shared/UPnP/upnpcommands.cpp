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

#include "upnpcommands.h"

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

UPnPCommands::UPnPCommands()
{

}

AddPortMappingResponse UPnPCommands::AddPortMapping(std::string newRemoteHost,
                                  int newExternalPort,
                                  std::string newProtocol,
                                  int newInternalPort,
                                  std::string newInternalClient,
                                  int newEnabled,
                                  std::string newPortMappingDescription,
                                  int newLeaseDuration,
                                  const DeviceResponse &portmappingDevice)
{
    int sock = getSocket(portmappingDevice.descriptionUrl.host().toStdString(), portmappingDevice.descriptionUrl.port());

    AddPortMappingResponse addPortMappingResponse;
    addPortMappingResponse.statusCode = 0;

    char buffer[1024];

    std::cout << "\r\n\r\n ========================== \r\n\r\n";

    std::string soapbody(   "<?xml version=""1.0""?>"
                            "<s:Envelope xmlns:s=""http://schemas.xmlsoap.org/soap/envelope/"" s:encodingStyle=""http://schemas.xmlsoap.org/soap/encoding/"">"
                            "<s:Body>"
                            "<u:AddPortMapping xmlns:u=""urn:schemas-upnp-org:service:" + portmappingDevice.serviceName + """>"
                            "<NewRemoteHost></NewRemoteHost>"
                            "<NewExternalPort>" + std::to_string(newExternalPort) + "</NewExternalPort>"
                            "<NewProtocol>" + newProtocol + "</NewProtocol>"
                            "<NewInternalPort>" + std::to_string(newInternalPort) + "</NewInternalPort>"
                            "<NewInternalClient>" + newInternalClient + "</NewInternalClient>"
                            "<NewEnabled>" + std::to_string(newEnabled) + "</NewEnabled>"
                            "<NewPortMappingDescription>" + newPortMappingDescription + "</NewPortMappingDescription>"
                            "<NewLeaseDuration>" + std::to_string(newLeaseDuration) + "</NewLeaseDuration>"
                            "</u:AddPortMapping>"
                            "</s:Body>"
                            "</s:Envelope>"
                            "\r\n");

    std::string soapbodylength (std::to_string(soapbody.length()-10));

    std::string headerAndSoapBody ("POST " + portmappingDevice.controlURL + " HTTP/1.0\r\n"
                        "Host: " + portmappingDevice.descriptionUrl.host().toStdString() + ":" + std::to_string(portmappingDevice.descriptionUrl.port()) + "\r\n"
                        "Content-Length: " + soapbodylength + "\r\n"
                        "Content-Type: text/xml\r\n"
                        "SOAPAction: ""urn:schemas-upnp-org:service:" + portmappingDevice.serviceName + "#AddPortMapping""\r\n"
                        "Connection: Close\r\n"
                        "Cache-Control: no-cache\r\n"
                        "Pragma: no-cache\r\n"
                        "\r\n" +
                        soapbody);


    std::cout << "====== controlURL " << portmappingDevice.controlURL << std::endl;
    std::cout << "\r\n========== will send soap message \r\n" << headerAndSoapBody << "\r\n";

    //send(sock,headerAndSoapBody.c_str(),headerAndSoapBody.length(),0);
    std::vector<char> vector_char_headerAndSoapBody(headerAndSoapBody.begin(),headerAndSoapBody.end());
    _sendmsgPlain(sock, vector_char_headerAndSoapBody);

    char addPortMappingReply[1024];
    std::string addportreply;
    while (recv(sock,addPortMappingReply,sizeof(addPortMappingReply),0)>0){ //TODO: edw na valw recv timeout
        addportreply += addPortMappingReply;
    }
    std::cout << "\r\n server reply \r\n" << addportreply;

    if (addportreply.find("200 OK") < addportreply.length()){
        std::cout << "found index: " << addportreply.find("200 OK") << " of length: " << addportreply.length();
        addPortMappingResponse.statusCode=200;
        addPortMappingResponse.internalPort = newInternalPort;
        addPortMappingResponse.portMappingDescription = newPortMappingDescription;
        addPortMappingResponse.remotePort = newExternalPort;
        addPortMappingResponse.rawResponse = std::string(addportreply);
    }


    #ifdef WIN32
    closesocket(sock);
    #else
    close(sock);
    #endif

    return addPortMappingResponse;
}

int UPnPCommands::getSocket(std::string deviceIP, unsigned short int devicePort)
{
        int bytes_recv;
        struct sockaddr_in serv_addr;

        #ifdef WIN32
            // Initialize Winsock
            int iResult;
            WSADATA wsaData;
            iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
            if (iResult != 0) {
                std::cout << "WSAStartup failed: " << iResult << std::endl;
                return -1;
            }
        #endif

        int sock = socket(AF_INET, SOCK_STREAM, 0);
#ifdef WIN32
        if (sock == INVALID_SOCKET) {
#else
        if (sock < 0){
#endif
            perror("ERROR opening socket");
            return -1;
        }


        memset((char *) &serv_addr,0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr=inet_addr(deviceIP.data());
        serv_addr.sin_port = htons(devicePort);

        int conres = ::connect(sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
        if (conres < 0)
        {
            std::cout << "ERROR connecting. result: " << conres << "\n";

#ifdef WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            //edw xtypaei ean yparxei syndesi sto internet alla o proxy den trexei
            //error("ERROR connecting");

            return -1;
         }

        return sock;
}
