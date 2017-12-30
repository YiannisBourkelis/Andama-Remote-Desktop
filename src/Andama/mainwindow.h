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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "screenshotsworker.h"
//#include "clientserver.h"
//#include "clientserverprotocol.h"
#include "protocolsupervisor.h"
#include <QMainWindow>
#include <QString>
#include "qdebug.h"
#include "qbytearray.h"
#include "qpixmap.h"
#include <QObject>
#include <QEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QDesktopWidget>
#include <math.h>       /* nearbyint */
#include <QMessageBox>
#include "mousekeyb/Mouse.h"
#include "mousekeyb/Keyboard.h"
#include <QMessageBox>
#include "keepalive.h"
#include <iostream>
#include "About.h"
#include <QDesktopServices>
#include "p2pserver.h"
//#include <appnapcontroller.h>
#include "../Shared/UPnP/upnpengine.h"
#include "tbllogmodel.h"
#include "mousekeyb/MouseCursorHook.h"

#ifdef Q_OS_MAC
#include <IOKit/pwr_mgt/IOPMLib.h> //gia na ksypnaei to monitor
#endif


typedef std::vector<char> MyArray;
typedef std::string MyString;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void connectToServer();
    ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *e);
    //clientServerProtocol protocol;
    protocolSupervisor protocol_supervisor;
    clientserver p2pclient;
    P2PServer p2pserver;
    QImage lastScreenshot;
    screenshotsWorker screenshotWrk;
    keepalive keepAlive;
    About about;
    tblLogModel tbllogmodel;


private slots:
    void mymessageReceived(const clientServerProtocol *client, const int msgType, const std::vector<char>& vdata);
    void non_UI_thread_messageReceived(const clientServerProtocol *client,const int msgType, const std::vector<char>& vdata);
    void protocol_exception(QString ex);
    void on_btnConnectToRemoteClient_clicked();
    void on_actionAbout_triggered();
    void slot_protocol_finished_or_terminated();
    void slot_addPortMappingResponse(const AddPortMappingResponse &addPortMappingRes);

    void on_connectWidget_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    void setDisabledRemoteControlWidgets(bool flag);
    QPoint lastMainWindowPosition;
    void setDefaultGUI();
    bool firstRun;
    UPnPEngine upnpengine;
    MouseCursorHook mouseCursorHook;


protected:
    void closeEvent(QCloseEvent *event);


};


#endif // MAINWINDOW_H
