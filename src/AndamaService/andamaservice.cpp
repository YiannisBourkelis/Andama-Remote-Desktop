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

#include "andamaservice.h"

AndamaService::AndamaService(int argc, char **argv) : QtService<QApplication>(argc,argv,"AndamaService"), engine(new QObject())
{
    try{
        setServiceDescription("Andama Remote Desktop Service");
        setServiceFlags(QtService::CanBeSuspended);
    }
    catch (...)
    {
        qCritical() << "Unknown error in ansamaservice.cpp constructor";
    }
}

AndamaService::~AndamaService()
{

}

void AndamaService::start()
{
    QCoreApplication *app = application();
    qDebug() << "Service started in: " << app->applicationFilePath();

    engine.start();
}

void AndamaService::pause()
{

}

void AndamaService::resume()
{

}

void AndamaService::stop()
{

}

