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
#include "QThread"

UPnPEngine::UPnPEngine(QObject *parent) : QObject(parent)
{
}

void UPnPEngine::AddPortMappingAsync()
{
    pendingRequests++;
    std::thread t(&UPnPEngine::AddPortMapping,this);
    t.detach();
}

std::string testthread(std::string input_s){
    for(int i=0;i<4;i++){
        std::cout << "thread id:" << std::this_thread::get_id()  << " i=" << i << std::endl;
        QThread::sleep(1);
    }

    return input_s;
}

void UPnPEngine::AddPortMapping()
{
    auto future1 = std::async(std::launch::async , &UPnPEngine::getNetworkInterface,this);

    UPnPDiscovery upnpdiscovery;
    auto future2 = std::async(std::launch::async ,&UPnPDiscovery::getDeviceLocationXmlUrl,&upnpdiscovery);


    QHostAddress simple1 = future1.get();
    QUrl simple2 = future2.get();

    std::cout << "simple1: " << simple1.toString().toStdString() << std::endl;
    std::cout << "simple2: " << simple2.host().toStdString() << std::endl;
    std::cout << "simple2: " << simple2.toString().toStdString() << std::endl;


    UPnPCommands upnpcommands;
    auto future3 = std::async(std::launch::async ,&UPnPCommands::AddPortMapping,&upnpcommands,
                              "",
                              5980,
                              "TCP",
                              8092,
                              simple1.toString().toStdString(),
                              1,
                              "AndamaRemoteDesktop",
                              10,
                              simple2.host().toStdString(),
                              (unsigned short)simple2.port(),
                              simple2);

    bool simple3 = future3.get();
    std::cout << "simple3: " << simple3 << std::endl;

    pendingRequests--;
}




void UPnPEngine::wait()
{
    while(pendingRequests >0){};
}

QHostAddress UPnPEngine::getNetworkInterface()
{
    QHostAddress last;
    for (auto& address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             last = address;
            // break; //TODO: endexomenos edw na yparxoun perisoteres IP sto mixanima alla egw lamvanw mono tin prwti
    }

    return last;
}
