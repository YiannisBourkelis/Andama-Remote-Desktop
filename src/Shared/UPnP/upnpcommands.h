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

#ifndef UPNPCOMMANDS_H
#define UPNPCOMMANDS_H

#include <string>
#include <QUrl>
#include "addportmappingresponse.h"

class UPnPCommands
{
public: 
    UPnPCommands();
    AddPortMappingResponse AddPortMapping(std::string NewRemoteHost,
                        int NewExternalPort,
                        std::string NewProtocol,
                        int NewInternalPort,
                        std::string NewInternalClient,
                        int NewEnabled,
                        std::string NewPortMappingDescription,
                        int NewLeaseDuration,
                        std::string deviceIP,
                        unsigned short devicePort,
                        QUrl deviceCapabilitiesXMLURL);

private:
    int getSocket(std::string deviceIP, unsigned short int devicePort);
};

#endif // UPNPCOMMANDS_H
