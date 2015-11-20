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

#ifndef KEEPALIVE_H
#define KEEPALIVE_H

#include <QThread>
#include <thread>
#include <atomic>
#include <chrono>
#include "clientserver.h"

class keepalive : public QThread
{
        Q_OBJECT
public:
    clientserver * protocol;

protected:
    void run(void);

private:
    std::chrono::high_resolution_clock::time_point _lastHeartBeat;
    void setLastHeartBeat(std::chrono::high_resolution_clock::time_point timepoint);
    std::chrono::high_resolution_clock::time_point getLastHeartBeat();
};

#endif // KEEPALIVE_H
