/* ***********************************************************************
 * Andama
 * (C) 2016 by Yiannis Bourkelis (hello@andama.org)
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
#include "andamaservice.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    auto winsta = OpenWindowStation(L"WinSta0", true, GENERIC_ALL);
        qDebug("OpenWindowStation lasterror =%u", GetLastError());


        SECURITY_ATTRIBUTES attributes = {sizeof(SECURITY_ATTRIBUTES), 0, true};
        attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
        attributes.bInheritHandle = true;
        //hNewDesktop = CreateDesktop("NewDesktopName", NULL, NULL, 0 , GENERIC_ALL,  &stSecurityAttr);

    auto hwinsta2 = SetProcessWindowStation(winsta);
        qDebug("SetProcessWindowStation lasterror =%u", GetLastError());


    auto desktop = OpenDesktop(L"default", 0, false, GENERIC_ALL);
            qDebug("OpenDesktop lasterror = %u", GetLastError());


    /*
    // Create the desktop.
    auto desktop = CreateDesktop(L"newdesktop",
                               NULL,
                               NULL,
                               0,
                               GENERIC_ALL,
                               &attributes);
    qDebug("CreateDesktop lasterror =%u", GetLastError());
*/


    SetThreadDesktop(desktop);
    //SwitchDesktop(desktop);
    qDebug("SetThreadDesktop lasterror =%u", GetLastError());
#endif //Q_OS_WIN

    AndamaService service(argc, argv);
    return service.exec();
}
