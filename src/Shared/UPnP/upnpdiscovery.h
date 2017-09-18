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

#ifndef UPNPDISCOVERY_H
#define UPNPDISCOVERY_H

#include <QUrl>
#include <vector>
#include <string>

class UPnPDiscovery
{
public:
    UPnPDiscovery();



    std::vector<std::string> discoverDevices(const std::string &searchTarget);//searchTarget can be: ssdp:all, upnp:rootdevice, etc

private:
    const unsigned short MULTICAST_DISCOVERY_BIND_PORT = 34644; //26389;
};

#endif // UPNPDISCOVERY_H
