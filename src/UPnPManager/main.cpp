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

#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include "../Shared/UPnP/upnpengine.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    UPnPEngine upnpEngine;

    //upnpEngine.AddPortMappingAsync();
    upnpEngine.AddPortMappingPeriodicallyAsync("",5980,"TCP",8092,"",1,"AndamaRemoteDesktop",10,3);
    //upnpEngine.stopAddPortMappingAsyncThread = true;
    return a.exec();
}
