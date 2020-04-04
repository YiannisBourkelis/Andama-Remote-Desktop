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

#include "deviceresponse.h"

DeviceResponse::DeviceResponse()
{

}

void DeviceResponse::resolveServerTag()
{
    std::string device_lowercase = rawResponse;
    std::transform(device_lowercase.begin(), device_lowercase.end(), device_lowercase.begin(), ::tolower);
    const size_t locfinf = device_lowercase.find("server:");

    if (locfinf < device_lowercase.length()){
        //efoson to location vrethike, anazitw to url
        //sto original response (xwris lowercase diladi), gia na min epireastei to url
        //dimiourgw neo string meta to location:
        const std::string loc(rawResponse.substr(locfinf));
        const auto f = loc.find("\r\n");
        const std::string loc2(loc.substr(7,f-7));
        ServerTag = std::move(loc2);
    }
}
