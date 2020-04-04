/* ***********************************************************************
 * Andama
 * (C) 2014 by Yiannis Bourkelis (hello@andama.org)
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

#ifndef HELPERFUNCS_H
#define HELPERFUNCS_H

#include <vector>
#include <sstream>
#include <QtGlobal> // gia ta Q_OS_*

namespace helperfuncs{
    enum OS {
        Unknown = 0,
        Windows = 1,
        MacOS   = 2,
        Linux   = 3
    };


    enum connectionState {
        disconnected = 0,
        connectedWithProxy = 1,
        connectedWithOtherAsClient = 2,
        connectedWithOtherAsServer = 3
    };


    //void intToBytes(unsigned int value, std::vector<char> &buffer);
    //unsigned int bytesToInt(const std::vector<char> &buffer);
    //unsigned int bytesToInt(const std::vector<char> &buffer, unsigned int begin, unsigned int bytes);
    OS getOS();
    //std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);
}

#endif // HELPERFUNCS_H
