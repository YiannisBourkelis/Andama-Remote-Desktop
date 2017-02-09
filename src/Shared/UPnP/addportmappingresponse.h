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

#ifndef ADDPORTMAPPINGRESPONSE_H
#define ADDPORTMAPPINGRESPONSE_H

#include <string>
#include "deviceresponse.h"

//otan stelnw upnp aitima gia AddPortMapping, apothikevw stadiaka
//se afti tin class ta stoixeia twn devices
class AddPortMappingResponse
{
public:
    AddPortMappingResponse();

    unsigned short statusCode; //to apotelesma tou AddPortMapping
    unsigned short remotePort; //i external port pou dimiourgithike sto device. mporei na einaidiaforetiki apo to arxiko aitima, ean einai idi katilimeni
    unsigned short internalPort; //i internal port. Idia me ton p2pserver pou kanei listen
    std::string portMappingDescription; //to portmapping description opws stalthike arxika
    std::string rawResponse; //i pliris apantisi
    DeviceResponse deviceInfo;
};

#endif // ADDPORTMAPPINGRESPONSE_H
