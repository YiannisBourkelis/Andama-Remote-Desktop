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

#ifndef ANDAMASERVICE_H
#define ANDAMASERVICE_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include "qtservice.h"
#include "engine.h"

class AndamaService : public QtService<QApplication>
{
public:
    AndamaService(int argc, char **argv);

    ~AndamaService();

    void start();

    void pause();

    void resume();

    void stop();

private:
    Engine engine;
};

#endif // ANDAMASERVICE_H
