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
#include "addportmappingresponse.h"
#include <vector>
#include <string>
#include "deviceresponse.h"
#include <thread>

class UPnPEngine : public QObject
{  
    Q_OBJECT
public:
    explicit UPnPEngine(QObject *parent = 0);

    //public variables
    std::atomic<bool> _stopThread {false};
    std::thread AddPortMappingPeriodicallyAsyncThread;
    unsigned int upnpRemotePort = 0; //krataei tin telefetaia porta poy anixtike

    //public methods
    void AddPortMappingAsync();
    void AddPortMappingPeriodicallyAsync(const std::string &NewRemoteHost,
                                         const int NewExternalPort,
                                         const std::string &NewProtocol,
                                         const int NewInternalPort,
                                         const std::string &NewInternalClient,
                                         const int NewEnabled,
                                         const std::string &NewPortMappingDescription,
                                         const int NewLeaseDuration,
                                         const int seconds_period);

    void AddPortMappingPeriodically(const std::string &NewRemoteHost,
                                    const int NewExternalPort,
                                    const std::string &NewProtocol,
                                    const int NewInternalPort,
                                    const std::string &NewInternalClient,
                                    const int NewEnabled,
                                    const std::string &NewPortMappingDescription,
                                    const int NewLeaseDuration,
                                    const int seconds_period);

    AddPortMappingResponse AddPortMapping(const std::string &NewRemoteHost,
                        const int &NewExternalPort,
                        const std::string &NewProtocol,
                        const int NewInternalPort,
                        const std::string &NewInternalClient,
                        const int NewEnabled,
                        const std::string &NewPortMappingDescription,
                        const int NewLeaseDuration);

    std::string GETRequest(const QUrl &url);
    void stopThread();
    bool StopThreadPending();

private:
    //variables
    UPnPDiscovery _upnpDiscovery;
    QHostAddress getNetworkInterface();

    //methods
    std::vector<DeviceResponse> getDeviceResponses(const std::vector<std::string>& devices);
    std::vector<DeviceResponse> getPortMappingCapableDevices(const std::vector<DeviceResponse> &devices);

signals:
    void sig_addPortMappingResponse(const AddPortMappingResponse &addPortMappingRes);
};

#endif // UPNPENGINE_H
