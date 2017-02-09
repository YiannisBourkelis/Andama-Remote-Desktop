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

#ifndef DEVICERESPONSE_H
#define DEVICERESPONSE_H

#include <string>
#include <QUrl>

//apothikevetai to responce tou kathe device pou epistrefete meta apo tin anazitisi syskevvn upnp
class DeviceResponse
{
public:
    DeviceResponse();
    std::string rawResponse;//to minima opws epistrafike
    QUrl descriptionUrl;//to description url. Mas leei poy vrisketai to xml me ta capabilities tou sygekrimenou device
    std::string controlURL;//to control url gia tin ektelesi mias entolis p.x. AddPortMapping
    std::string serviceName; //Apothikevei to service name tis SOAP entolis pou tha ektelesti p.x. WANPPPConnection:1
    std::string ServerTag;
    void resolveServerTag();

};

#endif // DEVICERESPONSE_H
