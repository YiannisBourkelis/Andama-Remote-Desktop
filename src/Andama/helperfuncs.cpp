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

#include "helperfuncs.h"

namespace helperfuncs {
    void intToBytes(unsigned int value, std::vector<char> &buffer)
    {
        if(buffer.size() == 4){
            buffer[0] = (value >> 24) & 0xff;
            buffer[1] = (value >> 16) & 0xff;
            buffer[2] = (value >> 8)  & 0xff;
            buffer[3] = value & 0xff;
        }
        else if(buffer.size() == 2){
            buffer[0] = (value >> 8)  & 0xff;
            buffer[1] = value & 0xff;
        }
        else if(buffer.size() == 1){
            buffer[0] = value & 0xff;
        }
    }

    unsigned int bytesToInt(const std::vector<char> &buffer)
    {
        if (buffer.size() == 4){
            return
                (((unsigned char)buffer[0]) << 24) +
                (((unsigned char)buffer[1]) << 16) +
                (((unsigned char)buffer[2]) << 8) +
                  (unsigned char)buffer[3];
        }
        if (buffer.size() == 2){
            return
                (((unsigned char)buffer[0]) << 8) +
                  (unsigned char)buffer[1];
        }
        else if (buffer.size() == 1){
            return
                  (unsigned char)buffer[0];
        }
        return 0;
    }

    unsigned int bytesToInt(const std::vector<char> &buffer, unsigned int begin, unsigned int bytes)
    {
        std::vector<char> ret(buffer.begin() + begin,buffer.begin() + begin + bytes);
        return bytesToInt(ret);
    }

    OS getOS()
    {
    #ifdef Q_OS_WIN
        return OS::Windows;
    #elif defined Q_OS_MAC
        return OS::MacOS;
    #elif defined Q_OS_LINUX
        return OS::Linux;
    #else
        return OS::Unknown;
    #endif
    }

    /*
    std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
    */
}
