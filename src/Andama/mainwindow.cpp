/* *************    GNU General Public License    **********************
 *
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
 *
 * ***********************************************************************/

#ifdef WIN32
#define NOMINMAX
#endif

#include "mainwindow.h"
#include "osxobjectivecbridge.h"
#include "chrono"
#include "../Shared/AndamaHeaders/byte_functions.h"
#include "../Shared/General/bench.h"
#include "tbllogsortfilterproxymodel.h"
#include "tbllogdata.h"
//#include "imageconfig.h"


//xcode-select --install
//https://bugreports.qt-project.org/browse/QTCREATORBUG-13077?page=com.atlassian.jira.plugin.system.issuetabpanels:all-tabpanel

static auto last = std::chrono::steady_clock::now();

bool MainWindow::eventFilter(QObject *watched, QEvent* e)
{
    if (watched == ui->lblDesktop &&
            (e->type() == QEvent::MouseMove
             || e->type() == QEvent::MouseButtonPress
             || e->type() == QEvent::MouseButtonRelease
             || e->type() == QEvent::MouseButtonDblClick))
    {
        QMouseEvent *ke = static_cast<QMouseEvent*>(e);
        double xscale=(double)ui->lblDesktop->width() / lastScreenshot.width();
        double yscale=(double)ui->lblDesktop->height() / lastScreenshot.height();
        double x = (double)ke->pos().x() / (double)xscale;
        double y = (double)ke->pos().y() / (double)yscale;

        if (p2pclient.isClientConnected){
            p2pclient.sendMouse(nearbyint(x),
                               nearbyint(y),
                               ke->button(), //0 no btn, 1 left, 2 right, 4 mid, 8 xbt1, 0x00000010 xbtn2, http://qt-project.org/doc/qt-4.8/qt.html#MouseButton-enum

                               e->type() == QEvent::MouseMove           ? 1:
                               e->type() == QEvent::MouseButtonPress    ? 2:
                               e->type() == QEvent::MouseButtonRelease  ? 3:
                               e->type() == QEvent::MouseButtonDblClick ? 4: 0,

                               0,
                               0,
                               0);
            //return true;
        }else{
            protocol_supervisor.protocol.sendMouse(nearbyint(x),
                               nearbyint(y),
                               ke->button(), //0 no btn, 1 left, 2 right, 4 mid, 8 xbt1, 0x00000010 xbtn2, http://qt-project.org/doc/qt-4.8/qt.html#MouseButton-enum

                               e->type() == QEvent::MouseMove           ? 1:
                               e->type() == QEvent::MouseButtonPress    ? 2:
                               e->type() == QEvent::MouseButtonRelease  ? 3:
                               e->type() == QEvent::MouseButtonDblClick ? 4: 0,

                               0,
                               0,
                               0);
            //return true;
        }

    }

    else if (watched == ui->lblDesktop && e->type() == QEvent::Wheel){
        QWheelEvent *qe = static_cast<QWheelEvent*>(e);

       qDebug("wheel:%i, direction:%i",qe->delta(),qe->orientation() == Qt::Orientation::Horizontal ? 1:2);

       if (p2pclient.isClientConnected){
           p2pclient.sendMouse(0,
                              0,
                              0, //0 no btn, 1 left, 2 right, 4 mid, 8 xbt1, 0x00000010 xbtn2, http://qt-project.org/doc/qt-4.8/qt.html#MouseButton-enum
                              5,
                              nearbyint(abs(qe->delta() / 10)),
                              qe->delta() < 0 ? 1 : 2,  // 1 arnitikos, 2 thetikos
                              qe->orientation() == Qt::Orientation::Vertical ? 1 : 2); // 1 vertical, 2 horizontal
       }else{
           protocol_supervisor.protocol.sendMouse(0,
                              0,
                              0, //0 no btn, 1 left, 2 right, 4 mid, 8 xbt1, 0x00000010 xbtn2, http://qt-project.org/doc/qt-4.8/qt.html#MouseButton-enum
                              5,
                              nearbyint(abs(qe->delta() / 10)),
                              qe->delta() < 0 ? 1 : 2,  // 1 arnitikos, 2 thetikos
                              qe->orientation() == Qt::Orientation::Vertical ? 1 : 2); // 1 vertical, 2 horizontal
       }
    }

    else if (watched == ui->lblDesktop &&
             (e->type() == QEvent::KeyPress
              || e->type() == QEvent::KeyRelease))
    {
        QKeyEvent *ke = static_cast<QKeyEvent*>(e);

        qDebug("key:%i, modifiers:%i, nativeVirtualKey:%i, event:%s",ke->key(),ke->modifiers(),ke->nativeVirtualKey(),
               e->type() == QEvent::KeyPress ? "KeyPress" :
               e->type() == QEvent::KeyRelease ? "KeyRelease" : "unknown");

        //QApplication::beep();

        //sta windows to ke->nativeVirtualKey() epistrefei panta kapoio key
        //enw sto mac, gia shift,cmd,alt,ctrl,CAPS to ke->nativeVirtualKey() epistrefei 0

#ifdef Q_OS_LINUX
        portableVKey natPKey;
        if (ke->nativeVirtualKey() >= 0x30 && ke->nativeVirtualKey() <= 0x5A){
           natPKey = Keyboard::getPortableVKey(ke->nativeVirtualKey(), ke->key());
        } else {
           natPKey = Keyboard::getPortableVKey(ke->key(), ke->nativeVirtualKey());
        }
#else
        portableVKey natPKey = Keyboard::getPortableVKey(ke->nativeVirtualKey(),ke->key());
#endif

        portableKeyboardModifier modPKey =  Keyboard::getPortableModifier(ke->key());

        if (natPKey != -1 || modPKey != portableKeyboardModifier::NoModifier){
            if (p2pclient.isClientConnected)
            {
                p2pclient.sendKeyboard(natPKey, modPKey,
                                      e->type() == QEvent::KeyPress   ? 1 :
                                      e->type() == QEvent::KeyRelease ? 2 : 0);
            }else{
               protocol_supervisor.protocol.sendKeyboard(natPKey, modPKey,
                                      e->type() == QEvent::KeyPress   ? 1 :
                                      e->type() == QEvent::KeyRelease ? 2 : 0);
            }
        }

    }

    else if ((watched == ui->txtRemotePCID || watched == ui->txtRemotePassword) &&
             (e->type() == QEvent::KeyRelease))
    {
        QKeyEvent *ke = static_cast<QKeyEvent*>(e);
        ui->lblRemoteIDError->clear();
        ui->lblRemotePasswordError->clear();
        if (ke->key() == Qt::Key_Return){
            ui->btnConnectToRemoteClient->click();
        }
    }
    return QWidget::eventFilter(watched, e);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<MyArray>("MyArray");
    qRegisterMetaType<clientserver *>("myClientServer");
    qRegisterMetaType<AddPortMappingResponse>("AddPortMappingResponse");
    //printsomething();

    firstRun = true; //flag pou deixnei oti i efarmogi trexei gia prwti fora.

    ui->setupUi(this);

    ui->remoteConnectProgressBar->setHidden(true);
    ui->lblDesktop->setHidden(true);
    ui->lblP2PPort->setHidden(true);

    ui->lblLocalHead->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblLocalDesc->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblLocalID->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblLocalPassword->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblP2PPort->setAttribute(Qt::WA_TranslucentBackground);

    QFont txtLocalPasswordFont = ui->txtLocalPassword->font();
    txtLocalPasswordFont.setLetterSpacing(QFont::SpacingType::AbsoluteSpacing,3);
    ui->txtLocalPassword->setFont(txtLocalPasswordFont);

    ui->lblRemoteHead->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblRemoteDesc->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblRemoteID->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblRemoteIDError->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblRemotePassword->setAttribute(Qt::WA_TranslucentBackground);
    ui->lblRemotePasswordError->setAttribute(Qt::WA_TranslucentBackground);
#if  defined(Q_OS_WIN) || defined(Q_OS_LINUX)
    ui->menuBar->setHidden(true);

    QFont lblFont = ui->lblLocalID->font();
    lblFont.setPointSize(lblFont.pointSize() * 0.9);

    QFont lblSmallFont = ui->lblLocalDesc->font();
    lblSmallFont.setPointSize(lblSmallFont.pointSize() * 0.9);

    QFont lblBigFont = ui->lblLocalHead->font();
    lblBigFont.setPointSize(lblBigFont.pointSize() * 0.9);

    QFont txtFont = ui->txtRemotePassword->font();
    txtFont.setPointSize(txtFont.pointSize() * 0.9);

    QFont buttonFont = ui->btnConnectToRemoteClient->font();
    buttonFont.setPointSize(buttonFont.pointSize() * 0.9);
    ui->btnConnectToRemoteClient->setFont(buttonFont);


    ui->lblLocalHead->setFont(lblBigFont);
    ui->lblLocalDesc->setFont(lblSmallFont);
    ui->lblLocalID->setFont(lblFont);
    ui->lblLocalPassword->setFont((lblFont));
    ui->lblID->setFont(txtFont);
    ui->txtLocalPassword->setFont(txtFont);
    ui->lblP2PPort->setFont(lblSmallFont);

    ui->lblRemoteHead->setFont(lblBigFont);
    ui->lblRemoteDesc->setFont(lblSmallFont);
    ui->lblRemoteID->setFont(lblFont);
    ui->lblRemoteIDError->setFont(lblSmallFont);
    ui->lblRemotePassword->setFont((lblFont));
    ui->lblRemotePasswordError->setFont(lblSmallFont);
    ui->txtRemotePCID->setFont(txtFont);
    ui->txtRemotePassword->setFont(txtFont);

    ui->tblLog->setFont(lblSmallFont);

    ui->lblStatus->setFont(lblFont);
#endif


    // ======== proxy protocol ============
    this->connect(&protocol_supervisor.protocol,
                  SIGNAL(sig_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  this,
                  SLOT(mymessageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&protocol_supervisor.protocol,
                  SIGNAL(sig_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  this,
                  SLOT(non_UI_thread_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  Qt::ConnectionType::DirectConnection);

    this->connect(&protocol_supervisor.protocol,
                  SIGNAL(sig_exception(QString)),
                  this,
                  SLOT(protocol_exception(QString)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&protocol_supervisor,
                  SIGNAL(finished()),
                  this,
                  SLOT(slot_protocol_finished_or_terminated()),
                  Qt::ConnectionType::AutoConnection);
   // ======== end proxy protocol ============

    // ======== p2p client protocol ============
    this->connect(&p2pclient,
                  SIGNAL(sig_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  this,
                  SLOT(mymessageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&p2pclient,
                  SIGNAL(sig_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  this,
                  SLOT(non_UI_thread_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  Qt::ConnectionType::DirectConnection);

    this->connect(&p2pclient,
                  SIGNAL(sig_exception(QString)),
                  this,
                  SLOT(protocol_exception(QString)),
                  Qt::ConnectionType::AutoConnection);

    /*
    this->connect(&p2pclient,
                  SIGNAL(finished()),
                  this,
                  SLOT(slot_protocol_finished_or_terminated()),
                  Qt::ConnectionType::AutoConnection);
                  */
   // ======== end p2p client protocol ============

    // ======== client socket  ============
    this->connect(&protocol_supervisor.clientsocket,
                  SIGNAL(sig_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  this,
                  SLOT(mymessageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&protocol_supervisor.clientsocket,
                  SIGNAL(sig_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  this,
                  SLOT(non_UI_thread_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  Qt::ConnectionType::DirectConnection);

    this->connect(&protocol_supervisor.clientsocket,
                  SIGNAL(sig_exception(QString)),
                  this,
                  SLOT(protocol_exception(QString)),
                  Qt::ConnectionType::AutoConnection);
    // ======== client socket  ============

    // ======== upnp ======================
    this->connect(&upnpengine,
                  SIGNAL(sig_addPortMappingResponse(AddPortMappingResponse)),
                  this,
                  SLOT(slot_addPortMappingResponse(AddPortMappingResponse)),
                  Qt::ConnectionType::AutoConnection);

    // ======== upnp ======================

    this->connect(&p2pserver,
                  SIGNAL(sig_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  this,
                  SLOT(mymessageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&p2pserver,
                  SIGNAL(sig_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  this,
                  SLOT(non_UI_thread_messageRecieved(const clientServerProtocol*, const int, const std::vector<char>&)),
                  Qt::ConnectionType::DirectConnection);


    ui->tblLog->setModel(&tbllogmodel);
    ui->tblLog->verticalHeader()->hide();
    ui->tblLog->horizontalHeader()->setStretchLastSection(true);
    ui->tblLog->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tblLog->horizontalHeader()->hide();
    //ui->tblLog->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    //ui->tblLog->horizontalHeader()->setResizeContentsPrecision(0);
    //ui->tblLog->resizeColumnToContents(0); //de doulevei
    ui->tblLog->setColumnWidth(0,150);
    ui->tblLog->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tblLog->horizontalHeader()->setStyleSheet("::section {background-color: lightblue };"); //lightblue

    screenshotWrk.protocol_supervisor = &protocol_supervisor;
    screenshotWrk.p2pServer = &p2pserver;
    screenshotWrk.imageQuality=50; //default JPG 80; --kai to 30 einai ok ws low quality kai exei to 1/3 se megethos peripou
    screenshotWrk.start();

    keepAlive.protocol_supervisor = &protocol_supervisor;
    keepAlive.start();

    protocol_supervisor.start();
    //protocol.start();
    p2pclient.remotePort=17332;
    p2pserver.start();

    //upnp
    upnpengine.AddPortMappingPeriodicallyAsync("",17332,"TCP",17332,"",1,"AndamaRemoteDesktop",(12 * 60 * 60 /*12 wres*/),(10 * 60));//aitima gia anoigma its port 17332 gia 12 wres, kathe 10 lepta

    qDebug("-------------||||  GUI THREAD ||| Thread id inside MainWindow %i",QThread::currentThreadId());

    ui->lblDesktop->setMouseTracking((true));
    ui->lblDesktop->installEventFilter(this);
    ui->txtRemotePassword->installEventFilter(this);
    ui->txtRemotePCID->installEventFilter(this);

}


//kaleitai sto protocol thread
void MainWindow::slot_protocol_finished_or_terminated()
{
    protocol_supervisor.protocol.setConnectionState(connectionState::disconnected);
    while (protocol_supervisor.isRunning()) {
        std::chrono::milliseconds sleep_dura(10);
        std::this_thread::sleep_for(sleep_dura);
    }

    if (firstRun){
        firstRun=false;
        setDefaultGUI();
    }

    protocol_supervisor.start();
}

void MainWindow::slot_addPortMappingResponse(const AddPortMappingResponse &addPortMappingRes)
{
    ui->lblP2PPort->setVisible(addPortMappingRes.statusCode == 200);
    if (addPortMappingRes.statusCode == 200){
        QString s;
        if(addPortMappingRes.internalPort == addPortMappingRes.remotePort){
            s = tr("P2P port: %1").arg(QString::fromStdString(std::to_string(addPortMappingRes.internalPort)));
            ui->lblP2PPort->setText(s);
        }else{
            s = tr("P2P internal port: %1, external port: %2").arg(QString::fromStdString(std::to_string(addPortMappingRes.internalPort)),QString::fromStdString(std::to_string(addPortMappingRes.remotePort)));
            ui->lblP2PPort->setText(s);
        }
        tbllogmodel.addLogData(tr("UPnP Successful. %1 (%2)").arg(
                                   s,
                                   addPortMappingRes.deviceInfo.ServerTag.c_str())
                               );
    }else{
        tbllogmodel.addLogData(tr("UPnP returned error code: %1 (%2)").arg(
                                   QString::fromStdString(std::to_string(addPortMappingRes.statusCode)),
                                   addPortMappingRes.deviceInfo.ServerTag.c_str())
                               );
    }

}

void MainWindow::protocol_exception(QString ex)
{
    QMessageBox msgBox;
    msgBox.setText(ex);
    msgBox.exec();
}

void MainWindow::non_UI_thread_messageRecieved(const clientServerProtocol *client, const int msgType, const std::vector<char>& vdata)
{
    try {
        if (msgType == protocol_supervisor.protocol.MSG_MOUSE)
        {
            int _xpos       = bytesToInt(vdata,0,2);
            int _ypos       = bytesToInt(vdata,2,2);
            int _btn        = bytesToInt(vdata,4,1);
            int _mouseEvent = bytesToInt(vdata,5,1);
            int _wheel      = bytesToInt(vdata,6,2);
            int _wheelSign  = bytesToInt(vdata,8,1);

            if (_mouseEvent == 1){
                 Mouse::moveToPoint(_xpos,_ypos);
            }
             else if (_mouseEvent == 2){
                 Mouse::press((Qt::MouseButton)_btn);
             }
             else if (_mouseEvent == 3){
                 Mouse::release((Qt::MouseButton)_btn);
             }
             else if (_mouseEvent == 4){
                 Mouse::doubleClick((Qt::MouseButton)_btn);
             }
             else if (_mouseEvent == 5){
                int _sign = _wheelSign == 1 ? 1 : -1;
                Mouse::scroll(_sign * _wheel);
             }

        } // MSG_MOUSE

        else if (msgType == protocol_supervisor.protocol.MSG_KEYBOARD)
        {
            int _portableVKey       = bytesToInt(vdata,0,4);
            int _portableModifiers  = bytesToInt(vdata,4,1);
            int _keyEvent           = bytesToInt(vdata,5,1);
            int localVKey           = Keyboard::convertPortableKeyToLocal((portableVKey) _portableVKey);

            //ean to key pou stalthike den yparxei sto topiko mixanima den kanw tipota
            if (localVKey == -1 && _portableModifiers == 0) return;

            bool press = false,release = false;

             if (_portableVKey == portableVKey::PVK_CAPSLOCK){
#ifdef Q_OS_MAC
                if (protocol_supervisor.protocol.getRemoteComputerOS() == OS::Windows){
                    //ta windows gia to plikro caps stlenoun press kai release kathe fora
                    //opote edw tha prepei na steilw tin mia fora mono press
                    //kai tin epomeni fora mono release.
                    if (_keyEvent == 1){//agnow to release event pou tha stalthei
                        static bool caps;
                        press   = !caps;
                        release = !press;
                        caps    = !caps;

                        //static bool caps;
                        //press   =  (caps == false);
                        //release =  (caps == true);
                        //caps = press;
                    }

                    _keyEvent = -1;//to an tha ginei press h release to xeirizomai me tis pio panw metavlites press/release
                }
#elif defined Q_OS_WIN || defined Q_OS_LINUX
                if (protocol_supervisor.protocol.getRemoteComputerOS() == OS::MacOS){
                    //otan to trexon leitourgiko einai windows h LINUX kai to remote computer
                    //einai mac, simainei oti mas stelnei mia fora caps press
                    //kai argotera mia fora caps release.
                    //Epeidei ta windows kai linux theloun kathe fora press kai release
                    //metatrepw kathe press h release se press+release
                    press   = true;
                    release = true;
                }
#endif
            }//if PVK_CAPSLOCK

            if (_keyEvent == 1 || press){
                Keyboard::keyPress(localVKey,
                                  _portableModifiers);
            }
            if (_keyEvent == 2 || release) { // MI GINEI ELSEIF giati mporei na kleithoun kai ta dyo gia to caps se windows
                Keyboard::keyRelease(localVKey,
                                  _portableModifiers);
            }

        } // MSG_KEYBOARD

    } catch ( ... ){
       //TODO: oti kai na ginei edw den prepei na sykwthei exception
        qDebug("--> Exception in: void MainWindow::non_UI_thread_messageRecieved(int msgType, std::vector<char> cdata,std::vector<char> vdata)");
    }
}

/*
void MainWindow::setDisabledRemoteControlWidgets(bool flag)
{
    ui->lblRemoteIDError->setDisabled(flag);
    ui->lblRemotePasswordError->setDisabled(flag);
    ui->lblRemoteID->setDisabled(flag);
    ui->lblRemotePassword->setDisabled(flag);
    ui->txtRemotePCID->setDisabled(flag);
    ui->txtRemotePassword->setDisabled(flag);
    ui->btnConnectToRemoteClient->setDisabled(flag);

    if (flag == false){
        ui->txtRemotePCID->setFocus();
    }
}
*/

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (p2pclient.isClientConnected){
        p2pclient.sendDisconnectFromRemoteComputer();
        p2pclient.isClientConnected=false;
        p2pclient.quit();

        setDefaultGUI();

        ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
        ui->lblStatus->setText(tr("Disconnected from remote computer. Ready!"));
        //ui->btnConnectToRemoteClient->setEnabled(true);
        //ui->txtRemotePCID->setEnabled(true);
        //ui->txtRemotePassword->setEnabled(true);
        tbllogmodel.addLogData(tr("P2P Connection: Disconnected from remote computer"));

        event->ignore();
        return;
    }

    if (protocol_supervisor.protocol.getConnectionState() == connectionState::connectedWithOtherAsClient){
        protocol_supervisor.protocol.sendDisconnectFromRemoteComputer();
        setDefaultGUI();

        ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
        ui->lblStatus->setText(tr("Remote computer disconnected. Ready!"));

        event->ignore();
    }
    else if (protocol_supervisor.protocol.getConnectionState() == connectionState::connectedWithOtherAsServer){
        //preidopoiw ton xristi oti yparxei syndedemenos ypologistis kai  oti tha diakopei i syndesi
        QMessageBox msgBox;
        msgBox.setText("Andama");
        msgBox.setTextFormat(Qt::TextFormat::RichText);
        msgBox.setFixedWidth(300);
        msgBox.setInformativeText(tr("Another user is already connected to this computer.<br />Are you sure that you want to exit?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);

        if (msgBox.exec() == QMessageBox::Yes){
            event->accept();
            protocol_supervisor.protocol.sendDisconnectFromRemoteComputer();
        }else {
            event->ignore();
        }
    }

    // threads cleanup
    keepAlive.stopThread = true;
    keepAlive.wait();

    screenshotWrk.stopThread = true;
    screenshotWrk.wait();

    upnpengine.stopAddPortMappingAsyncThread = true;
    //upnpengine.waitForAllAddPortMappingPendingRequests(); //TODO: den termatizei to thread opws prepei, apla den emfanizetai error...Tha prepei na parw referense tou thread kai na perimenw mexri na stamatisei
}

void MainWindow::setDefaultGUI()
{
    this->showNormal();
    this->move(lastMainWindowPosition);

    ui->remoteConnectProgressBar->hide();

    ui->btnConnectToRemoteClient->show();
    ui->btnConnectToRemoteClient->setEnabled(true);

    this->setMaximumSize(728,418);
    this->setMinimumSize(728,418);
    this->resize(728,418);
    ui->mainWidget->setVisible(true);
    ui->lblDesktop->setVisible(false);
}

void MainWindow::mymessageRecieved(const clientServerProtocol *client, const int msgType,const std::vector<char>& vdata)
{
    try{

       if (msgType == protocol_supervisor.protocol.MSG_ID){
           //setDisabledRemoteControlWidgets(false);

            qDebug("Thread id inside mymessageRecieved %li", (long)QThread::currentThreadId());
            std::string strID(vdata.begin(),vdata.end());
            QString qs = QString::fromStdString(strID);
            ui->lblID->setText(qs);
            qDebug() << std::string(vdata.begin(),vdata.end()).c_str();

            ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
            ui->lblStatus->setText(tr("Ready!"));


            tbllogmodel.addLogData(tr("Application ID recieved from proxy. Ready to connect and accept connections!"));

        }
       else if (msgType == protocol_supervisor.protocol.MSG_LOCAL_PASSWORD_GENERATED){
            ui->txtLocalPassword->setText(QString::fromStdString(protocol_supervisor.protocol.password));
            p2pserver.password = protocol_supervisor.protocol.password;
       }
       else if (msgType == protocol_supervisor.protocol.MSG_CONNECTION_ACCEPTED){
           qDebug("O apomakrismenos ypologistis apodexthike ti syndesi!");

           //otan lavw minima apodoxis tis syndesis apo ton allo ypologisti
           //tou stelnw minima na mou steilei to prwto screenshot
            ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
            const QString str = tr("Remote computer accepted the connection. Requesting remote desktop image");
            ui->lblStatus->setText(str + "...");
            tbllogmodel.addLogData(str);

           if(p2pclient.isClientConnected == true){
                p2pclient.RequestScreenshot();
           }else{
                protocol_supervisor.protocol.RequestScreenshot();
           }
       }
       else if (msgType == protocol_supervisor.protocol.MSG_CONNECT_ID_NOT_FOUND){
           ui->lblRemoteIDError->setText(tr("Remote ID does not exist"));
           ui->btnConnectToRemoteClient->setVisible(true);
           ui->remoteConnectProgressBar->setHidden(true);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
           ui->lblStatus->setText(tr("Ready!"));
           tbllogmodel.addLogData(tr("The requested remote computer ID does not exist"));
           QApplication::beep();
       }
       else if (msgType == protocol_supervisor.protocol.MSG_CONNECT_PASSWORD_NOT_CORRECT){
           ui->lblRemotePasswordError->setText(tr("The password is not correct"));
           ui->btnConnectToRemoteClient->setVisible(true);
           ui->remoteConnectProgressBar->setHidden(true);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
           ui->lblStatus->setText(tr("Ready!"));
           tbllogmodel.addLogData(tr("The password you entered is not correct"));
           QApplication::beep();
       }
       else if (msgType == protocol_supervisor.protocol.MSG_BAN_IP_WRONG_PWD){
           ui->btnConnectToRemoteClient->setVisible(true);
           ui->lblRemotePasswordError->setVisible(true);
           ui->lblRemotePasswordError->setText(tr("IP banned"));
           ui->remoteConnectProgressBar->setHidden(true);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_red.png)");
           const QString str_rejected_conn = tr("The connection is rejected because of multiple wrong password attempts");
           ui->lblStatus->setText(str_rejected_conn);
           tbllogmodel.addLogData(str_rejected_conn);
           QApplication::beep();
       }
       else if (msgType == protocol_supervisor.protocol.MSG_WARNING_BAN_IP_WRONG_PWD){
           ui->lblRemotePasswordError->setText(tr("The password is not correct"));
           ui->txtRemotePassword->setFocus();
           ui->btnConnectToRemoteClient->setVisible(true);
           ui->remoteConnectProgressBar->setHidden(true);
           const QString str_wrong_password = tr("The password you entered is not correct");
           tbllogmodel.addLogData(str_wrong_password);

           const QString str1 = str_wrong_password + ". " + tr("Remaining tries: %1");
           const int remain = bytesToInt(vdata);
           const QString str2 = str1.arg(remain);
           tbllogmodel.addLogData(str2);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_orange.png)");
           ui->lblStatus->setText(str2);
           QApplication::beep();
       }
       else if (msgType == protocol_supervisor.protocol.MSG_ERROR_APP_VERSION_NOT_ACCEPTED){
           const QString str_app_ver_not_supported = tr("The current version of the application is no longer supported.");
           const QString str_please_upgrade = tr("Pease upgrade");
           tbllogmodel.addLogData(str_app_ver_not_supported + " " +  str_please_upgrade);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_red.png)");
           ui->lblStatus->setText(tr("Upgrade required"));

           QMessageBox msgBox;
           msgBox.setText("Andama");
           msgBox.setTextFormat(Qt::TextFormat::RichText);
           msgBox.setFixedWidth(300);
           const QString str_download_newer = tr("Do you want to download the newer version?");
           msgBox.setInformativeText(str_app_ver_not_supported + " " + str_download_newer);
           msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
           msgBox.setDefaultButton(QMessageBox::Yes);

           //efoson epelekse na kanei download to update
           //anoigw to update url ston browser
           if (msgBox.exec() == QMessageBox::Yes){
               std::string surl(vdata.begin(),vdata.end());
               QString qsurl(surl.data());
               QUrl url(qsurl);
               QDesktopServices::openUrl(url);
           }else {
           }
       }
       else if (msgType == protocol_supervisor.protocol.MSG_REMOTE_CLIENT_ACCEPTED){
           qDebug("Apodexthika ti syndesi tou remote pc.");
           //const QString
           QString str_remote_client = tr("Remote client accepted");
           tbllogmodel.addLogData(str_remote_client);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
           ui->lblStatus->setText(str_remote_client + ".");
           ui->btnConnectToRemoteClient->setEnabled(false);
           lastMainWindowPosition = this->pos();
           this->showMinimized();
       }
       else if(msgType == protocol_supervisor.protocol.MSG_NO_INTERNET_CONNECTION){
           //setDisabledRemoteControlWidgets(true);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_red.png)");
           QString str_error_connecting = tr("Error connecting. %1").arg(tr("Please make sure you have an internet connection and try again."));
           ui->lblStatus->setText(str_error_connecting);
           tbllogmodel.addLogData(str_error_connecting);
       }
       else if(msgType == protocol_supervisor.protocol.MSG_NO_PROXY_CONNECTION){
           //setDisabledRemoteControlWidgets(true);
           if (p2pclient.remotePort == 0){
                ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_red.png)");
                QString str_error_connecting_proxy = tr("Error connecting with proxy server. %1").arg(tr("Please make sure you have an internet connection and try again."));
                ui->lblStatus->setText(str_error_connecting_proxy);
                tbllogmodel.addLogData(str_error_connecting_proxy);
           }else{
               p2pclient.remotePort == 0;
           }
       }
       else if(msgType == protocol_supervisor.protocol.MSG_REMOTE_COMPUTER_DISCONNECTED){
           //AppNapController::EnableAppNap();
           setDefaultGUI();
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
           ui->lblStatus->setText(tr("Remote computer disconnected. Ready!"));
           tbllogmodel.addLogData(tr("The remote computer is disconnected"));
       }
       else if(msgType == protocol_supervisor.protocol.MSG_WARNING_BAN_IP_WRONG_ID){
           ui->btnConnectToRemoteClient->setVisible(true);
           ui->remoteConnectProgressBar->setHidden(true);

           const QString str1 = tr("Warning! Your IP will be banned because of multiple wrong ID attempts. Remaining tries: %1");
           const int remain = bytesToInt(vdata);
           const QString str2 = str1.arg(remain);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_orange.png)");
           ui->lblStatus->setText(str2);
           tbllogmodel.addLogData(str2);
           QApplication::beep();
       }
       else if(msgType == protocol_supervisor.protocol.MSG_BAN_IP_WRONG_ID){
           ui->btnConnectToRemoteClient->setVisible(true);
           ui->remoteConnectProgressBar->setHidden(true);

           const QString str1 = tr("Connection rejected because of multiple wrong ID attempts. Please try again after %1 sec.");
           const int remain = bytesToInt(vdata);
           const QString str2 = str1.arg(remain);
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_red.png)");
           ui->lblStatus->setText(str2);
           tbllogmodel.addLogData(str2);
           QApplication::beep();
       }
       else if(msgType == protocol_supervisor.protocol.MSG_ERROR_CANNOT_CONNECT_SAME_ID){
           ui->btnConnectToRemoteClient->setVisible(true);
           ui->remoteConnectProgressBar->setHidden(true);
           ui->lblStatus->setText(tr("Ready!"));
           ui->widgetStatus->setStyleSheet("background-image: url(:/images/images/status_green.png)");
           ui->lblRemoteIDError->setText(tr("Cannot connect to the same ID"));
           tbllogmodel.addLogData(tr("Hey! I cannot connect to myself"));
           QApplication::beep();
       }
       else if (msgType == protocol_supervisor.protocol.MSG_SCREENSHOT_DIFF_REQUEST || msgType == protocol_supervisor.protocol.MSG_SCREENSHOT_REQUEST){
           //std::cout << "MainWindow::mymessageRecieved > Lipsi aitimatos gia apostoli neou screenshot" << std::endl;
           //Bench bench("MSG_SCREENSHOT_DIFF_REQUEST and MSG_SCREENSHOT_DIFF_REQUEST");
#ifdef Q_OS_MAC
           //xreiazetai gia na anoigei to monitor se periptwsi
           //pou einai kleisto. Sta windows anoigei mono tou.
           //Sto linux den exw dokimasei (todo)
           //Kanonika xreiazetai hook wste na vlepei kai sti synexeia
           //ean to monitor koimatai wste na to anoigei (todo)
           if (msgType == protocol_supervisor.protocol.MSG_SCREENSHOT_REQUEST){
                IOPMAssertionID assertionID;
                IOPMAssertionDeclareUserActivity(CFSTR(""),
                                                 kIOPMUserActiveLocal,
                                                 &assertionID);
           }

           objcappnap::disableAppNap();
#endif


           //qDebug("5. UI myMessageRecieved: Screensot request recieved. Etoimasia apostolis screenshot me grabwindow");
           //QImage qimg(QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId()).toImage());
           //QImage qimg(QGuiApplication::screens()[QGuiApplication::screens().count()-1]->grabWindow(QApplication::desktop()->winId()).toImage());
           //if (qimg.isNull())
           //{
           //    qDebug("---------> qimg is null!!!");
           //}
           //qDebug("6. Grabwindow succeded: image width: %i. Calling screenshotWrk.setScreenshot",qimg.width());

           //std::cout << "MainWindow::mymessageRecieved > Lifthike to screenshot kai tha tethei sto screenshotWrk" << std::endl;
           screenshotWrk.setScreenshot(QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId()).toImage(),msgType);
           //std::cout << "MainWindow::mymessageRecieved > To screenshot tethike sto screenshotWrk" << std::endl;
       }
       else if (msgType == protocol_supervisor.protocol.MSG_SCREENSHOT)
       {
          lastMainWindowPosition = this->pos();

          qDebug("Screenshot recieved. Setting image in control! Total bytes: %lu", vdata.size());
          QPixmap qpix;
          //QByteArray qbytes;

          //qbytes.insert(0, vdata.data(), vdata.size());

          //QByteArray image_bytes_uncompressed=qUncompress(qbytes);
          QByteArray image_bytes_uncompressed=qUncompress(reinterpret_cast<const unsigned char*>(vdata.data()),vdata.size());

          //qpix.loadFromData(image_bytes_uncompressed,"WEBP");
          qpix.loadFromData(image_bytes_uncompressed,"JPG");

          //*** symantiko. to lastScreenshot crataei kathe fora to pio prosfato
          //screenshot prosthetontas tis diafores
          lastScreenshot = QImage(qpix.toImage());

          ui->mainWidget->hide();

          //resizing window
          int w_diff = this->frameGeometry().width()-this->geometry().width();
          int h_diff = this->frameGeometry().height()-this->geometry().height();
          int max_width  = QApplication::desktop()->availableGeometry().width() - w_diff;
          int max_height = QApplication::desktop()->availableGeometry().height() - h_diff;

          this->move(QApplication::desktop()->availableGeometry().x(),QApplication::desktop()->availableGeometry().y());
          //this->move(0,0);

          double scale = 1;
          if (max_width >= qpix.width() && max_height >= qpix.height()){

          } else {
                double h_scale = (double)max_height / qpix.height();
                double w_scale = (double)max_width / qpix.width();
                scale = std::min(h_scale, w_scale);
          }

          this->setMaximumSize(QSize(nearbyint(qpix.width()  * scale) ,
                                      nearbyint(qpix.height() * scale) ));
          this->setMinimumSize(QSize(nearbyint(qpix.width()  * scale) ,
                                      nearbyint(qpix.height() * scale)));

          this->resize(nearbyint(qpix.width()  * scale) ,
                       nearbyint(qpix.height() * scale)  );

          ui->lblDesktop->setVisible(true);
          ui->lblDesktop->move(0,0);
          ui->lblDesktop->setMaximumSize(this->geometry().width(),this->geometry().height());
          ui->lblDesktop->setMinimumSize(this->geometry().width(),this->geometry().height());
          ui->lblDesktop->resize(this->geometry().width(),this->geometry().height());
         // ui->lblDesktop->resize(this->frameGeometry().width(),this->geometry().height());


           qDebug("Seting last screenshot for first time. Height: %i", lastScreenshot.height());
           //w.lblDesktop->setPixmap(qpix.scaledToHeight(w.lblDesktop->height()));
           //ui->lblDesktop->setPixmap(qpix.scaledToHeight(ui->lblDesktop->height()));
           ui->lblDesktop->setPixmap(qpix.scaled(ui->lblDesktop->width(), ui->lblDesktop->height(),
                                                 Qt::AspectRatioMode::IgnoreAspectRatio,
                                                 Qt::TransformationMode::SmoothTransformation));

           std::cout << "IMAGE DIFF BYTES RECIEVED:" << vdata.size() << std::endl;
       }
       else if (msgType == protocol_supervisor.protocol.MSG_SCREENSHOT_DIFF)
       {
           if (vdata.size() == 8)
           {
               //qDebug("DS.6 UI - To screenshot diff einai keno. Epistrefw");
               return;
           }

           Bench("xronos emfanisis screenshot diff stin othoni");

           //qDebug("DS.2 UI - To screenshot diff lifthike. Stelnw screenshot diff request.");
           //protocol.RequestScreenshotDiff();

           //qDebug("DS.3 UI - Screenshot diff recieved. Setting image in control! Total bytes: %lu", vdata.size());
           QImage qpix;

           //lamvanw tin thesi x tis eikonas (prwta 2 bytes)
           std::vector<char> cx(vdata.begin(),vdata.begin()+2);
           int x = bytesToInt(cx);
           //qDebug("x: %i",x);

           //lamvanw to y tis eikonas (epomena 2 bytes)
           std::vector<char> cy(vdata.begin()+2,vdata.begin()+4);
           int y = bytesToInt(cy);
           //qDebug("y: %i",y);

           //qDebug("DS.4 desktop lbl height: %i, lastscreenshot height: %i",ui->lblDesktop->height(),lastScreenshot.height());

           QByteArray image_bytes_uncompressed=qUncompress(reinterpret_cast<const unsigned char*>(vdata.data()+4),vdata.size()-4);

           //qDebug("DS.5 diff image uncompressed recieved bytes: %i",image_bytes_uncompressed.size());
           //qDebug("DS.7 UI - Tha ginei decode twn bytes pou lifthikan se JPG");

          qpix.loadFromData(image_bytes_uncompressed,"JPG");

          //qDebug("DS.8 diff image loaded! Height: %i, width: %i",qpix.height(),qpix.width());

          qreal xx=x;
          qreal yy=y;
          qreal ww=qpix.width();
          qreal hh=qpix.height();

          QRectF tarRect(xx,yy,ww,hh);

          //qDebug("DS.9 Will paint lastScreenshot image with diff recieved. qrectf x: %f, width: %f",tarRect.x(),tarRect.width());
          QPainter qPainter(&lastScreenshot);
          qPainter.drawImage(tarRect,qpix,qpix.rect());
          QSize qs(ui->lblDesktop->geometry().width(),ui->lblDesktop->geometry().height());
          QImage qimg2 = lastScreenshot.scaled(qs, Qt::AspectRatioMode::IgnoreAspectRatio,Qt::TransformationMode::SmoothTransformation);
          //qDebug("DS.10 Diff img painted to lastscreenshot. lastscreenshot height: %i. Proetoimasia gia na fortwthei sto lblDesktop",lastScreenshot.height());

          QPixmap qp(QPixmap::fromImage(qimg2));

           ui->lblDesktop->setPixmap(qp);

           std::cout << "IMAGE DIFF BYTES RECIEVED:" << vdata.size() << std::endl;
           //qDebug("DS.11 Diff img fortwthike sto lblDesktop. Telos epeksergasias. Epistrofi apo to UI. lastScreenshot.height: %i.",lastScreenshot.height());
       }//MSG_SCREENSHOT_DIFF
    }
    catch (std::exception &ex)
    {
        qDebug("exception: %s",ex.what());

        QMessageBox msgBox;
        msgBox.setText(QString::fromUtf8(ex.what()));
        msgBox.exec();
    }
}

//click sto koumpi syndesis se apomakrismeno ypologisti
void MainWindow::on_btnConnectToRemoteClient_clicked()
{   
    //efoson exei symplirwthei to remote ID kai remote password
    //apostelw to aitima syndesis
    if(ui->txtRemotePCID->text().size() > 0 && ui->txtRemotePassword->text().size() > 0){
        //apokryptw to button, emfanizw marquee klp
        //wste na fainetai oti kati ginetai
        ui->lblStatus->setText(tr("Connecting to remote computer..."));
        ui->btnConnectToRemoteClient->hide();
        ui->remoteConnectProgressBar->setHidden(false);
        //ui->txtRemotePCID->setFocus();//xreiazetai giati sta windows meta apo to click sto button pairnei to focus to local password field
        ui->lblRemoteIDError->clear();
        ui->lblRemotePasswordError->clear();
        tbllogmodel.addLogData("Connecting to the remote computer...");

        //lamvanw ta remote id kai remote password
        //ta metatrepw se vector<char> kai ta pernaw sto protocolo
        //gia na ginai apostoli tou aitimatos syndesis ston apomakrismeno ypologist
        std::string strRemoteID=ui->txtRemotePCID->text().toStdString();
        std::vector<char> vectRemoteID = std::vector<char>(strRemoteID.begin(),strRemoteID.end());

        std::string strPassword = ui->txtRemotePassword->text().toStdString();
        std::vector<char> vectRemotePassword = std::vector<char>(strPassword.begin(), strPassword.end());

        if (ui->txtRemotePCID->text().contains(":")){
            tbllogmodel.addLogData(tr("Connecting to the remote computer directly..."));
            lastMainWindowPosition = this->pos();
            p2pclient.remotePort=ui->txtRemotePCID->text().split(":")[1].toInt();
            p2pclient.remoteIpAddress = ui->txtRemotePCID->text().split(":")[0].toStdString();
            p2pclient.setRemotePassword(ui->txtRemotePassword->text().toStdString());
            p2pclient.start();
        }else{
            protocol_supervisor.protocol.Connect(vectRemoteID, vectRemotePassword);
        }
    } else {
        //den exei kataxwrithei remote id h remote password
        //opote kanw beep kai dinw to focus sto pedio pou den exei symplirwthei
        QApplication::beep();
        if (ui->txtRemotePCID->text().size() == 0){
            ui->txtRemotePCID->setFocus();
        } else {
            ui->txtRemotePassword->setFocus();
        }
    }


} //on_btnConnectToRemoteClient_clicked

void MainWindow::on_connectWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    menu.addAction(ui->actionAbout);
    menu.exec(ui->connectWidget->mapToGlobal(pos));
}
void MainWindow::on_actionAbout_triggered()
{
    about.show();
    about.raise();//ean vrisketai pisw apo to trexon parathyro to fernei mprosta
}

MainWindow::~MainWindow()
{
    delete ui;
}

