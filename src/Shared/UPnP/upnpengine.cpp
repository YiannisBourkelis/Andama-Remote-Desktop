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
#include "upnpcommands.h"
#include "../General/finally.h"
#include <QtNetwork>

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
            _lastAddNewPortMappingTimePoint = std::chrono::high_resolution_clock::now();
            AddPortMapping(NewRemoteHost,NewExternalPort,NewProtocol,NewInternalPort,NewInternalClient,NewEnabled,NewPortMappingDescription,NewLeaseDuration);
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

        auto future_getNetworkInterface = std::async(std::launch::async , &UPnPEngine::getNetworkInterface,this);

        UPnPDiscovery upnpdiscovery;
        auto future_getDeviceLocationXmlUrl = std::async(std::launch::async ,&UPnPDiscovery::getDeviceLocationXmlUrl,&upnpdiscovery);

        QHostAddress locan_lan_ip;
        if (NewInternalClient.empty()){
            locan_lan_ip = future_getNetworkInterface.get();
        }else{
            locan_lan_ip = QHostAddress(QString::fromStdString(NewInternalClient));
        }

        QUrl deviceLocationXmlUrl;
        if (future_getDeviceLocationXmlUrl.wait_for(std::chrono::seconds(10)) == std::future_status::ready){
            deviceLocationXmlUrl=future_getDeviceLocationXmlUrl.get();
        } else {
            std::cout << "future_getDeviceLocationXmlUrl timeout" << std::endl;
        }

        std::cout << "local_lan-wifi_ip: " << locan_lan_ip.toString().toStdString() << std::endl;
        std::cout << "deviceLocationXmlUrl.host: " << deviceLocationXmlUrl.host().toStdString() << std::endl;
        std::cout << "deviceLocationXmlUrl: " << deviceLocationXmlUrl.toString().toStdString() << std::endl;


        UPnPCommands upnpcommands;
        auto future_AddPortMapping = std::async(std::launch::async ,&UPnPCommands::AddPortMapping,&upnpcommands,
                                  NewRemoteHost, // ""
                                  NewExternalPort, //5980
                                  NewProtocol, // "TCP"
                                  NewInternalPort, //8092
                                  locan_lan_ip.toString().toStdString(),
                                  NewEnabled, // 1
                                  NewPortMappingDescription, // "AndamaRemoteDesktop",
                                  NewLeaseDuration, // 10 (se seconds)
                                  deviceLocationXmlUrl.host().toStdString(),
                                  (unsigned short)deviceLocationXmlUrl.port(),
                                  deviceLocationXmlUrl);

        addPortMappingResp = future_AddPortMapping.get();
        std::cout << "\r\naddPortMapping result: " << (addPortMappingResp.statusCode == 200) << std::endl;
    }
    catch (...)
    {
        //TODO: pros to paron den kanw kati me to exception
    }

    emit sig_addPortMappingResponse(addPortMappingResp);
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

