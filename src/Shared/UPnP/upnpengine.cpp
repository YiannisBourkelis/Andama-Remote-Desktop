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

#include "upnpengine.h"
#include <iostream>
#include <future>
#include <QUrl>
#include "../General/finally.h"
#include <QtNetwork>
#include <QHttpPart>
#include "upnpcommands.h"
#include <QTcpSocket>
#include <QHostAddress>

UPnPEngine::UPnPEngine(QObject *parent) : QObject(parent)
{

}

void UPnPEngine::AddPortMappingPeriodicallyAsync(std::string NewRemoteHost,
                                                 int NewExternalPort,
                                                 std::string NewProtocol,
                                                 int NewInternalPort,
                                                 std::string NewInternalClient,
                                                 int NewEnabled,
                                                 std::string NewPortMappingDescription,
                                                 int NewLeaseDuration,
                                                 int seconds_period)
{
    std::thread t(&UPnPEngine::AddPortMappingPeriodically,this,NewRemoteHost,
                                                                  NewExternalPort,
                                                                  NewProtocol,
                                                                  NewInternalPort,
                                                                  NewInternalClient,
                                                                  NewEnabled,
                                                                  NewPortMappingDescription,
                                                                  NewLeaseDuration,
                                                                  seconds_period);
    t.detach();
}

void UPnPEngine::AddPortMappingPeriodically(std::string NewRemoteHost,
                                            int NewExternalPort,
                                            std::string NewProtocol,
                                            int NewInternalPort,
                                            std::string NewInternalClient,
                                            int NewEnabled,
                                            std::string NewPortMappingDescription,
                                            int NewLeaseDuration,
                                            int seconds_period)
{

    std::chrono::milliseconds sleep_dura(10);
    std::chrono::high_resolution_clock::time_point _lastAddNewPortMappingTimePoint;

    while (true && !stopAddPortMappingAsyncThread){
        std::chrono::high_resolution_clock::time_point curr_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(curr_time - _lastAddNewPortMappingTimePoint);

        if (time_span.count() >= seconds_period)
        {
            std::cout << "will call AddPortMapping from AddPortMappingPeriodically\r\n" << std::endl;
            int addport_retries = 0;
             while (addport_retries < 10 && AddPortMapping(NewRemoteHost,NewExternalPort, NewProtocol,
                                   NewInternalPort,NewInternalClient,NewEnabled,
                                   NewPortMappingDescription,NewLeaseDuration).statusCode != 200){
                 addport_retries++;
                 NewExternalPort += addport_retries;
                std::cout << "returned from AddPortMapping called from AddPortMappingPeriodically\r\n" << std::endl;
             }

            _lastAddNewPortMappingTimePoint = std::chrono::high_resolution_clock::now();
        }
        std::this_thread::sleep_for(sleep_dura);
    }

    std::cout << "AddPortMappingAsyncThread exiting...\r\n" << std::endl;
}

void UPnPEngine::waitForAllAddPortMappingPendingRequests()
{
    while(addPortMappingPendingRequests > 0){std::this_thread::sleep_for(std::chrono::milliseconds(1));};
}


void UPnPEngine::AddPortMappingAsync()
{
    std::thread t(&UPnPEngine::AddPortMapping,this,"",5980,"TCP",8092,"",1,"AndamaRemoteDesktop",10);
    t.detach();
}

AddPortMappingResponse UPnPEngine::AddPortMapping(std::string NewRemoteHost,
                    int NewExternalPort,
                    std::string NewProtocol,
                    int NewInternalPort,
                    std::string NewInternalClient,
                    int NewEnabled,
                    std::string NewPortMappingDescription,
                    int NewLeaseDuration)
{
    AddPortMappingResponse addPortMappingResp;

    try
    {
        addPortMappingPendingRequests++;
        Finally finally([&]{addPortMappingPendingRequests--;});

        // 1. lamvanw ta network interfaces
        auto future_getNetworkInterface = std::async(std::launch::async , &UPnPEngine::getNetworkInterface,this);

        // 2. lamvanw ti lista me ta diathesima devices
        UPnPDiscovery upnpdiscovery;
        auto future_getDeviceLocationXmlUrl = std::async(std::launch::async ,&UPnPDiscovery::discoverDevices,&upnpdiscovery,"upnp:rootdevice");

        // 3. lamvanw tin topiki IP tou ypologisti pou me kalei (default) h
        // tin IP pou orise o xristis
        QHostAddress locan_lan_ip;
        if (NewInternalClient.empty()){
            locan_lan_ip = future_getNetworkInterface.get();
        }else{
            locan_lan_ip = QHostAddress(QString::fromStdString(NewInternalClient));
        }

        // 4. lamvanw ti lista me ta diathesima devices
        std::vector<std::string> devicesFound = future_getDeviceLocationXmlUrl.get();

        std::cout << "local_lan-wifi_ip: " << locan_lan_ip.toString().toStdString() << std::endl;
        //std::cout << "deviceLocationXmlUrl.host: " << deviceLocationXmlUrl.host().toStdString() << std::endl;
        //std::cout << "deviceLocationXmlUrl: " << deviceLocationXmlUrl.toString().toStdString() << std::endl;

        // 5. lamvanw ta device responses
        std::vector<DeviceResponse> unique_devices = getDeviceResponses(devicesFound);

        // 6. lamvanw ti lista me ta devices poy ypostirizoyn addportmapping. sinithws einai ena, alla mporei na einai kai parapanw
        //symfwna kai me afto to thread: (UPnP code needs cleanup and improved handling #432) https://github.com/syncthing/syncthing/issues/432
        //kai afto: (AddPortMapping with WANPPPConnection and/or WANIPConnection) http://miniupnp.tuxfamily.org/forum/viewtopic.php?t=538
        std::vector<DeviceResponse> portmapping_devices = getPortMappingCapableDevices(unique_devices);

        for (DeviceResponse portmapping_device : portmapping_devices){
            UPnPCommands upnpcommands;
            addPortMappingResp = upnpcommands.AddPortMapping(
                                      NewRemoteHost, // ""
                                      NewExternalPort, //5980
                                      NewProtocol, // "TCP"
                                      NewInternalPort, //8092
                                      locan_lan_ip.toString().toStdString(),
                                      NewEnabled, // 1
                                      NewPortMappingDescription, // "AndamaRemoteDesktop",
                                      NewLeaseDuration,
                                      portmapping_device);

            std::cout << "\r\naddPortMapping result: " << (addPortMappingResp.statusCode == 200) << std::endl;
            emit sig_addPortMappingResponse(addPortMappingResp);
        }
    }
    catch (...)
    {
        //TODO: pros to paron den kanw kati me to exception
    }

    return addPortMappingResp;
}



QHostAddress UPnPEngine::getNetworkInterface()
{
    //std::cout << "UPnPEngine::getNetworkInterface() === enter func\r\n"  << std::endl;
    for (auto& address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost)
                && !address.isLoopback())
        {
            std::string addip(address.toString().toStdString());
            if (addip.length() > 0 &&
               (addip.find(":") > addip.length())){
                return address;
            }
        }
    }
   // std::cout << "UPnPEngine::getNetworkInterface() === will exit func\r\n"  << std::endl;
    return QHostAddress();
}

std::vector<DeviceResponse> UPnPEngine::getPortMappingCapableDevices(std::vector<DeviceResponse> devices)
{
    std::vector<DeviceResponse> portmapping_devices;

    for(DeviceResponse devres : devices){
        std::string devcaps = GETRequest(devres.descriptionUrl);

        std::string devcaps_lowercase = devcaps;
        std::transform(devcaps_lowercase.begin(), devcaps_lowercase.end(), devcaps_lowercase.begin(), ::tolower);

        size_t find_serviceId_WANPPPConn1 = devcaps_lowercase.find("serviceid:wanpppconn1");
        if(find_serviceId_WANPPPConn1 < devcaps_lowercase.length()){
            size_t find_controlURL = devcaps_lowercase.find("<controlurl>",find_serviceId_WANPPPConn1);
            size_t find__controlURL = devcaps_lowercase.find("</controlurl>", find_controlURL);
            std::string addnewportmapping_control_url = devcaps.substr(find_controlURL+12,find__controlURL-find_controlURL-12);
            std::cout << "\r\nAddNewPort control url = " << addnewportmapping_control_url <<  std::endl;

            devres.controlURL = addnewportmapping_control_url;
            devres.serviceName = "WANPPPConnection:1";
            portmapping_devices.push_back(devres);
        }
        else {
            size_t find_serviceId_WANIPConn1 = devcaps_lowercase.find("serviceid:wanipconn1");
            if(find_serviceId_WANIPConn1 < devcaps_lowercase.length()){
                size_t find_controlURL = devcaps_lowercase.find("<controlurl>",find_serviceId_WANIPConn1);
                size_t find__controlURL = devcaps_lowercase.find("</controlurl>", find_controlURL);
                std::string addnewportmapping_control_url = devcaps.substr(find_controlURL+12,find__controlURL-find_controlURL-12);
                std::cout << "\r\nAddNewPort control url = " << addnewportmapping_control_url <<  std::endl;

                devres.controlURL = addnewportmapping_control_url;
                devres.serviceName = "WANIPConnection:1";
                portmapping_devices.push_back(devres);
            }
        }
    }//for

    return portmapping_devices;
}

std::string UPnPEngine::GETRequest(QUrl url)
{
    QTcpSocket qtcp;
    qtcp.connectToHost(QHostAddress(url.host()),url.port());
    qtcp.waitForConnected(3000);

    std::string device_caps_GET_request ("GET " +
                                         url.path().toStdString() +
                                         " HTTP/1.1\r\nHost: " +
                                         url.host().toStdString() +
                                         ":" +
                                         std::to_string(url.port()) +
                                         "\r\nConnection: close\r\n\r\n");

    qtcp.write(device_caps_GET_request.c_str(),device_caps_GET_request.length());
    qtcp.waitForBytesWritten(1000);
    char buf[1024];
    qtcp.waitForReadyRead(1000);
    std::string total;
    while (qtcp.read(buf,1024)>0){
        total+=buf;
        qtcp.waitForReadyRead(1000);
    }
    std::cout << total << std::endl;

    qtcp.close();
    return total;
}

std::vector<DeviceResponse> UPnPEngine::getDeviceResponses(std::vector<std::string> devices)
{
    std::vector<DeviceResponse> deviceResponses;
    //std::vector<QUrl> portmapping_devicesurl;

    for(const std::string device : devices){
        //eksagw to discovery url
        //prwta metatrepw to response se lower case
        //giati to location mporei na einai Location, LOCATION klp
        std::string device_lowercase = device;
        std::transform(device_lowercase.begin(), device_lowercase.end(), device_lowercase.begin(), ::tolower);
        size_t locfinf = device_lowercase.find("location:");
        QUrl locationUrl;

        if (locfinf < device_lowercase.length()){
            //efoson to location vrethike, anazitw to url
            //sto original response (xwris lowercase diladi), gia na min epireastei to url
            //dimiourgw neo string meta to location:
            std::string loc(device.substr(locfinf));
            auto f = loc.find("\r\n");
            std::string loc2(loc.substr(9,f-9));
            //std::cout << loc2 << std::endl;
            locationUrl = QString::fromStdString(loc2).trimmed(); //to trimmed xreiazetai giati ean yparxei keno print to http den ginetai swsto parsing sto qurl

            //efoson to url den yparxei sto portmapping_devicesurl
            if (std::find_if(deviceResponses.begin(),deviceResponses.end(), [&locationUrl](const DeviceResponse & r) { return locationUrl == r.descriptionUrl; } ) == deviceResponses.end()){
                //portmapping_devicesurl.push_back(locationUrl);
                DeviceResponse devres;
                devres.descriptionUrl = locationUrl;
                devres.rawResponse = device;
                deviceResponses.push_back(devres);
                std::cout << "\r\n" << "Host: " << locationUrl.host().toStdString()<<"\r\nPort: "<< locationUrl.port() << std::endl;
            }
        }

    }//for

    return deviceResponses;
}

