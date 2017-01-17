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

#ifndef SCREENSHOTSWORKER_H
#define SCREENSHOTSWORKER_H

#include "helperfuncs.h"
#include <QThread>
//#include "clientserver.h"
#include "protocolsupervisor.h"
#include <QImage>
#include <atomic>
#include <QMutexLocker>
#include <QBuffer>

//ta includes afta xreiazontai gia ti lipsi screenshot
#include <QGuiApplication>
#include <QScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <qpaintengine.h>
#include "p2pserver.h"

class screenshotsWorker: public QThread
{
        Q_OBJECT
public:
    screenshotsWorker();
    void setScreenshot(QImage newScreenshot, const int msg);
    protocolSupervisor * protocol_supervisor;
    P2PServer * p2pServer;
    std::atomic<int> imageQuality;
    std::atomic<bool> stopThread {false};

protected:
    void run(void);

private:
    QImage pendingScreenshot;
    QImage pendingScreenshot_working_copy;
    QImage previousScreenshot;
    std::atomic<int> pendingMsg;
    //std::recursive_mutex pendingScreenshot_mutex;
    std::mutex pendingScreenshot_mutex;
    std::atomic<bool> isDirty;
    void sendScreenshot(QImage outimg, int x, int y);
    void prepareAndSendScreenshotDiff();
};

#endif // SCREENSHOTSWORKER_H
