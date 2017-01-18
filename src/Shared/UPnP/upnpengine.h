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

#ifndef UPNPENGINE_H
#define UPNPENGINE_H

#include <QObject>
#include "upnpdiscovery.h"
#include <QNetworkInterface>
#include <atomic>
#include <chrono>

class UPnPEngine : public QObject
{  
    Q_OBJECT
public:
    explicit UPnPEngine(QObject *parent = 0);
    void AddPortMappingAsync();
    void wait();
    void AddPortMappingPeriodicallyAsync(std::string NewRemoteHost,
                                         int NewExternalPort,
                                         std::string NewProtocol,
                                         int NewInternalPort,
                                         std::string NewInternalClient,
                                         int NewEnabled,
                                         std::string NewPortMappingDescription,
                                         int NewLeaseDuration,
                                         int seconds_period);

    void AddPortMappingPeriodically(std::string NewRemoteHost,
                                    int NewExternalPort,
                                    std::string NewProtocol,
                                    int NewInternalPort,
                                    std::string NewInternalClient,
                                    int NewEnabled,
                                    std::string NewPortMappingDescription,
                                    int NewLeaseDuration,
                                    int seconds_period);

private:
    std::atomic<int> pendingRequests {0};

    bool AddPortMapping(std::string NewRemoteHost,
                        int NewExternalPort,
                        std::string NewProtocol,
                        int NewInternalPort,
                        std::string NewInternalClient,
                        int NewEnabled,
                        std::string NewPortMappingDescription,
                        int NewLeaseDuration);

    UPnPDiscovery upnpDiscovery;
    QHostAddress getNetworkInterface();

signals:
};

#endif // UPNPENGINE_H
