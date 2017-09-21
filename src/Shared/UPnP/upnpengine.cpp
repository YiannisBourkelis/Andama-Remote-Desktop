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
#include "upnpaddportmapping.h"
#include <QTcpSocket>
#include <QHostAddress>

UPnPEngine::UPnPEngine(QObject *parent) : QObject(parent)
{

}

void UPnPEngine::AddPortMappingPeriodicallyAsync(const std::string &NewRemoteHost,
                                                 const int NewExternalPort,
                                                 const std::string &NewProtocol,
                                                 const int NewInternalPort,
                                                 const std::string &NewInternalClient,
                                                 const int NewEnabled,
                                                 const std::string &NewPortMappingDescription,
                                                 const int NewLeaseDuration,
                                                 const int seconds_period)
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

void UPnPEngine::AddPortMappingPeriodically(const std::string &NewRemoteHost,
                                            const int NewExternalPort,
                                            const std::string &NewProtocol,
                                            const int NewInternalPort,
                                            const std::string &NewInternalClient,
                                            const int NewEnabled,
                                            const std::string &NewPortMappingDescription,
                                            const int NewLeaseDuration,
                                            const int seconds_period)
{

    const std::chrono::milliseconds sleep_dura(5);
    std::chrono::high_resolution_clock::time_point _lastAddNewPortMappingTimePoint;

    while (true && !stopAddPortMappingAsyncThread){
        const std::chrono::high_resolution_clock::time_point curr_time = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(curr_time - _lastAddNewPortMappingTimePoint);

        if (time_span.count() >= seconds_period)
        {
            std::cout << "will call AddPortMapping from AddPortMappingPeriodically\r\n" << std::endl;
            int addport_retries = 0;
             while (addport_retries < 10 && AddPortMapping(NewRemoteHost,NewExternalPort + addport_retries, NewProtocol,
                                   NewInternalPort,NewInternalClient,NewEnabled,
                                   NewPortMappingDescription,NewLeaseDuration).statusCode != 200){
                 addport_retries++;
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

AddPortMappingResponse UPnPEngine::AddPortMapping(const std::string &NewRemoteHost,
                    const int &NewExternalPort,
                    const std::string &NewProtocol,
                    const int NewInternalPort,
                    const std::string &NewInternalClient,
                    const int NewEnabled,
                    const std::string &NewPortMappingDescription,
                    const int NewLeaseDuration)
{
    AddPortMappingResponse addPortMappingResp;

    try
    {
        addPortMappingPendingRequests++;
        Finally finally([&]{addPortMappingPendingRequests--;});

        std::cout << "1. lamvanw ta network interfaces" << std::endl;
        auto future_getNetworkInterface = std::async(std::launch::async , &UPnPEngine::getNetworkInterface,this);

        std::cout << "2. lamvanw tin topiki IP tou ypologisti pou me kalei (default) h" << std::endl;
        // tin IP pou orise o xristis
        QHostAddress locan_lan_ip;
        if (NewInternalClient.empty()){
            locan_lan_ip = future_getNetworkInterface.get();
        }else{
            locan_lan_ip = QHostAddress(QString::fromStdString(NewInternalClient));
        }
        std::cout << "local_lan-wifi_ip: " << locan_lan_ip.toString().toStdString() << std::endl;

        std::cout << "2. lamvanw ti lista me ta diathesima devices" << std::endl;
        UPnPDiscovery upnpdiscovery;
        auto future_getDeviceLocationXmlUrl = std::async(std::launch::async ,&UPnPDiscovery::discoverDevices,&upnpdiscovery,"upnp:rootdevice",locan_lan_ip.toString().toStdString().c_str());

        std::cout << "4. lamvanw ti lista me ta diathesima devices" << std::endl;
        const std::vector<std::string> devicesFound = future_getDeviceLocationXmlUrl.get();

        //std::cout << "deviceLocationXmlUrl.host: " << deviceLocationXmlUrl.host().toStdString() << std::endl;
        //std::cout << "deviceLocationXmlUrl: " << deviceLocationXmlUrl.toString().toStdString() << std::endl;

        std::cout << "5. lamvanw ta device responses" << std::endl;
        const std::vector<DeviceResponse> unique_devices = getDeviceResponses(devicesFound);

        std::cout << "6. lamvanw ti lista me ta devices poy ypostirizoyn addportmapping. sinithws einai ena, alla mporei na einai kai parapanw" << std::endl;
        //symfwna kai me afto to thread: (UPnP code needs cleanup and improved handling #432) https://github.com/syncthing/syncthing/issues/432
        //kai afto: (AddPortMapping with WANPPPConnection and/or WANIPConnection) http://miniupnp.tuxfamily.org/forum/viewtopic.php?t=538
        const std::vector<DeviceResponse> portmapping_devices = getPortMappingCapableDevices(unique_devices);

        std::cout << "7. kataxwrw to portmapping AddPortMapping" << std::endl;
        for (const DeviceResponse &portmapping_device : portmapping_devices){
            UPnPAddPortMapping addportmapping;
            addPortMappingResp = addportmapping.AddPortMapping(
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
    for (const auto& address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol
                && address != QHostAddress(QHostAddress::LocalHost)
                && !address.isLoopback())
        {
            const std::string addip(address.toString().toStdString());
            if (addip.length() > 0 &&
               (addip.find(":") > addip.length())){
                return address;
            }
        }
    }
   // std::cout << "UPnPEngine::getNetworkInterface() === will exit func\r\n"  << std::endl;
    return QHostAddress();
}

//epistrefei ta devices sta opoia nporei na ginei addportmapping mazi me tis plirofories pou xreiazontai.
//gia ena device mporei na ginoun kai perissoteres apo mia kataxwriseis ena yparxei p.x. WANPPPConnection:1 kai WANIPConnection:1
//sto idio device, parola afta den to exw testarei gia panw apo ena device
std::vector<DeviceResponse> UPnPEngine::getPortMappingCapableDevices(const std::vector<DeviceResponse> &devices)
{
    std::vector<DeviceResponse> portmapping_devices;

    for(const DeviceResponse &devres : devices){
        const std::string devcaps = GETRequest(devres.descriptionUrl);

        std::cout << " devres.descriptionUrl " << devres.descriptionUrl.toString().toStdString() << std::endl;

        std::string devcaps_lowercase = devcaps;
        std::transform(devcaps_lowercase.begin(), devcaps_lowercase.end(), devcaps_lowercase.begin(), ::tolower);

        const size_t find_serviceId_WANPPPConn1 = devcaps_lowercase.find("serviceid:wanpppconn1");
        if(find_serviceId_WANPPPConn1 < devcaps_lowercase.length()){
            const size_t find_controlURL = devcaps_lowercase.find("<controlurl>",find_serviceId_WANPPPConn1);
            const size_t find__controlURL = devcaps_lowercase.find("</controlurl>", find_controlURL);
            const std::string addnewportmapping_control_url(devcaps.substr(find_controlURL+12,find__controlURL-find_controlURL-12));
            std::cout << "\r\nAddNewPort control url 1 = " << addnewportmapping_control_url <<  std::endl;

            DeviceResponse newDevRes = devres;
            newDevRes.controlURL = std::string(addnewportmapping_control_url);
            newDevRes.serviceName = "WANPPPConnection:1";
            portmapping_devices.push_back(newDevRes);

            //efoson exei vrei wanpppconn1 den psanxw gia wanipconn1
            continue;
        }

        const size_t find_serviceId_WANIPConn1  = devcaps_lowercase.find("serviceid:wanipconn1");
        if (find_serviceId_WANIPConn1 < devcaps_lowercase.length()){
            const size_t find_controlURL = devcaps_lowercase.find("<controlurl>",find_serviceId_WANIPConn1);
            const size_t find__controlURL = devcaps_lowercase.find("</controlurl>", find_controlURL);
            const std::string addnewportmapping_control_url(devcaps.substr(find_controlURL+12,find__controlURL-find_controlURL-12));
            std::cout << "\r\nAddNewPort control url 2 = " << addnewportmapping_control_url <<  std::endl;

            DeviceResponse newDevRes = devres;
            newDevRes.controlURL = std::string(addnewportmapping_control_url);
            newDevRes.serviceName = "WANIPConnection:1";
            portmapping_devices.push_back(newDevRes);
        }

    }//for

    return portmapping_devices;
}

std::string UPnPEngine::GETRequest(const QUrl &url)
{
    QTcpSocket qtcp;
    qtcp.connectToHost(QHostAddress(url.host()),url.port());
    qtcp.waitForConnected(3000);

    const std::string device_caps_GET_request ("GET " +
                                         url.path().toStdString() +
                                         " HTTP/1.1\r\nHost: " +
                                         url.host().toStdString() +
                                         ":" +
                                         std::to_string(url.port()) +
                                         "\r\nConnection: close\r\n\r\n");

    qtcp.write(device_caps_GET_request.c_str(),device_caps_GET_request.length());
    const int bufsize = 4096;
    char buf[bufsize] = {};
    qtcp.waitForReadyRead(1000);
    std::string getResponse;
    while (qtcp.read(buf,bufsize)>0){
        getResponse+=buf;
        memset(buf, 0, bufsize);//TODO: isws kai na mi xreiazetai, alla to evala giati to qtcp.read nomizw kataxwrei ta dedomena sto telos tou buffer kathe fora
        qtcp.waitForReadyRead(1000);
    }

    qtcp.close();
    return getResponse;
}

std::vector<DeviceResponse> UPnPEngine::getDeviceResponses(const std::vector<std::string>& devices)
{
    std::vector<DeviceResponse> deviceResponses;

    for(const std::string &device : devices){
        std::cout << device << std::endl;
        //eksagw to discovery url
        //prwta metatrepw to response se lower case
        //giati to location mporei na einai Location, LOCATION klp
        std::string device_lowercase = device;
        std::transform(device_lowercase.begin(), device_lowercase.end(), device_lowercase.begin(), ::tolower);
        const size_t locfinf = device_lowercase.find("location:");

        if (locfinf < device_lowercase.length()){
            //efoson to location vrethike, anazitw to url
            //sto original response (xwris lowercase diladi), gia na min epireastei to url
            //dimiourgw neo string meta to location:
            const std::string loc(device.substr(locfinf));
            const auto f = loc.find("\r\n");
            const std::string loc2(loc.substr(9,f-9));
            const QUrl locationUrl(QString::fromStdString(loc2).trimmed()); //to trimmed xreiazetai giati ean yparxei keno print to http den ginetai swsto parsing sto qurl

            //efoson to url den yparxei sto portmapping_devicesurl
            if (std::find_if(deviceResponses.begin(),deviceResponses.end(), [&locationUrl](const DeviceResponse & r) { return locationUrl == r.descriptionUrl; } ) == deviceResponses.end()){
                //portmapping_devicesurl.push_back(locationUrl);
                DeviceResponse devres;
                const QUrl durl(locationUrl.toString());
                devres.descriptionUrl = durl;
                devres.rawResponse = std::string(device);
                devres.resolveServerTag();
                deviceResponses.push_back(devres);
                std::cout << "\r\n" << "Host: " << locationUrl.host().toStdString()<<"\r\nPort: "<< locationUrl.port() << std::endl;
            }
        }

    }//for

    return deviceResponses;
}

