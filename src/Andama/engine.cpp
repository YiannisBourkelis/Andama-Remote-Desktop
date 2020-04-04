#include "engine.h"
#include "mousekeyb/Mouse.h"
#include "mousekeyb/Keyboard.h"
#include "../Shared/AndamaHeaders/byte_functions.h"
#include <QString>

#ifdef Q_OS_MAC
#include "osxobjectivecbridge.h"
#include <IOKit/pwr_mgt/IOPMLib.h> //gia na ksypnaei to monitor
#endif


typedef std::vector<char> MyArray; // required for SIGNAL(sig_messageReceived(const int, const std::vector<char>&))


Engine::Engine(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<MyArray>("MyArray");
    qRegisterMetaType<clientserver *>("myClientServer");
    this->connect(&protocol_supervisor.protocol,
                  SIGNAL(sig_messageReceived(const int, const std::vector<char>&)),
                  this,
                  SLOT(mymessageReceived(const int, const std::vector<char>&)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&protocol_supervisor.protocol,
                  SIGNAL(sig_messageReceived(const int, const std::vector<char>&)),
                  this,
                  SLOT(non_UI_thread_messageReceived(const int, const std::vector<char>&)),
                  Qt::ConnectionType::DirectConnection);

    this->connect(&protocol_supervisor.protocol,
                  SIGNAL(sig_exception(QString)),
                  this,
                  SLOT(protocol_exception(QString)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&protocol_supervisor.protocol,
                  SIGNAL(finished()),
                  this,
                  SLOT(slot_protocol_finished_or_terminated()),
                  Qt::ConnectionType::AutoConnection);
}

#ifndef ANDAMA_SERVICE_MODE //AndamaService service/daemon does not use qml related code
Engine::Engine(ScreenshotProvider* provider,QObject* parent) : Engine(parent)
{
    screenshotProvider = provider;
}
#endif //ANDAMA_SERVICE_MODE

void Engine::start(){
    screenshotWrk.protocol_supervisor = &protocol_supervisor;
    screenshotWrk.imageQuality=80;
    screenshotWrk.start();

    keepAlive.protocol_supervisor = &protocol_supervisor;
    keepAlive.start();

    protocol_supervisor.start();

}


void Engine::slot_protocol_finished_or_terminated()
{
    protocol_supervisor.protocol.setConnectionState(connectionState::disconnected);
    while (protocol_supervisor.isRunning()) {
        std::chrono::milliseconds sleep_dura(10);
        std::this_thread::sleep_for(sleep_dura);
    }
    protocol_supervisor.start();
}

void Engine::protocol_exception(QString ex)
{
    exceptionErrorText = ex;
    emit exceptionErrorTextChanged();
}

void Engine::non_UI_thread_messageReceived(const int msgType, const std::vector<char>& vdata)
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
            int _portableModifiers = bytesToInt(vdata,4,1);
            int _keyEvent = bytesToInt(vdata,5,1);
            int localVKey = Keyboard::convertPortableKeyToLocal((portableVKey) _portableVKey);

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
        qDebug("--> Exception in: void MainWindow::non_UI_thread_messageReceived(int msgType, std::vector<char> cdata,std::vector<char> vdata)");
    }
}


void Engine::mymessageReceived(const int msgType,const std::vector<char>& vdata)
{

    try{
        remoteErrorPasswordText = "";
        remoteErrorIdText = "";
        std::string strID;
        QImage qimg;
        QPixmap qpix;
        QByteArray qbytes;
        QByteArray image_bytes_uncompressed;
        switch(msgType){
        case MSG_ID:
            statusErrorLevel= STATUS_NOERROR;
            strID = std::string(vdata.begin(),vdata.end());
            id = QString::fromStdString(strID);

            statusText="Ready!";
            emit idChanged();
            break;
        case MSG_LOCAL_PASSWORD_GENERATED:
            password = QString::fromStdString(protocol_supervisor.protocol.getLocalPlainPassword());
            emit passwordChanged();
            qDebug() << "Password: " << password;
            break;
        case MSG_CONNECTION_ACCEPTED:
            statusErrorLevel = STATUS_NOERROR;
            statusText="Remote computer accepted the request. Requesting remote desktop image...";
            protocol_supervisor.protocol.RequestScreenshot();
            break;
        case MSG_CONNECT_ID_NOT_FOUND:
            statusErrorLevel = STATUS_NOERROR;
            remoteErrorIdText="Remote ID not found";
            statusText="Ready!";
            break;
        case MSG_CONNECT_PASSWORD_NOT_CORRECT:
            statusErrorLevel = STATUS_NOERROR;
            remoteErrorPasswordText="The password is not correct";
            statusText="Ready!";
            break;
        case MSG_BAN_IP_WRONG_PWD:
            statusErrorLevel = STATUS_ERROR;
            remoteErrorPasswordText="";
            statusText="Connection rejected because of multiple wrong password attempts";
            break;
        case MSG_WARNING_BAN_IP_WRONG_PWD:
            statusErrorLevel = STATUS_WARNING;
            remoteErrorPasswordText="The password is not correct";
            statusText="The password is not correct. Remaining tries: !";
            break;
        case MSG_ERROR_APP_VERSION_NOT_ACCEPTED:
            statusErrorLevel = STATUS_ERROR;
            remoteErrorPasswordText="";
            statusText="Upgrade required";
            break;
        case MSG_REMOTE_CLIENT_ACCEPTED:
            statusErrorLevel = STATUS_NOERROR;
            statusText="Remote client accepted.";
            //deactivate button
            break;
        case MSG_NO_INTERNET_CONNECTION:
            //deactivate remote panel
            statusErrorLevel = STATUS_ERROR;
            statusText="Error connecting. Please make sure you have an internet connection and try again.";
            break;
        case MSG_NO_PROXY_CONNECTION:
            statusErrorLevel = STATUS_ERROR;
            statusText="Error connecting with proxy server. Please make sure you have an internet connection and try again.";
            break;
        case MSG_REMOTE_COMPUTER_DISCONNECTED:
            statusErrorLevel = STATUS_NOERROR;
            statusText="Remote computer disconnected. Ready!";
            break;
        case MSG_WARNING_BAN_IP_WRONG_ID:
            statusErrorLevel = STATUS_WARNING;
            statusText="Warning! Your IP will be banned because of multiple wrong ID attempts. Remaining tries: ";
            break;
        case MSG_BAN_IP_WRONG_ID:
            statusErrorLevel = STATUS_ERROR;
            statusText="Connection rejected because of multiple wrong ID attempts. Please try again after ";
            break;
        case MSG_ERROR_CANNOT_CONNECT_SAME_ID:
            statusErrorLevel = STATUS_NOERROR;
            remoteErrorIdText="Cannot connect to the same ID";
            statusText= "Ready!";
            break;
        case MSG_SCREENSHOT_DIFF_REQUEST:
        case MSG_SCREENSHOT_REQUEST:
            statusErrorLevel = STATUS_NOERROR;
            statusText="Ready!";
            //screenshotWrk.setScreenshot(qimg,msgType);

            //std::cout << "MainWindow::mymessageReceived > Lipsi aitimatos gia apostoli neou screenshot" << std::endl;
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


            qDebug("5. UI myMessageReceived: Screensot request received. Etoimasia apostolis screenshot me grabwindow");
#ifdef Q_OS_WIN
{
            //HDESK old_desktop = GetThreadDesktop(GetCurrentThreadId());

            /*
            LPCWSTR desk = L"Winlogon";
            HDESK desktop = OpenDesktop(desk, 0, FALSE, DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW |
                      DESKTOP_ENUMERATE | DESKTOP_HOOKCONTROL |
                      DESKTOP_WRITEOBJECTS | DESKTOP_READOBJECTS |
                  DESKTOP_SWITCHDESKTOP | GENERIC_WRITE);
                */

            /*
            HDESK desktop = OpenInputDesktop(0, FALSE,
                  DESKTOP_CREATEMENU | DESKTOP_CREATEWINDOW |
                  DESKTOP_ENUMERATE | DESKTOP_HOOKCONTROL |
                  DESKTOP_WRITEOBJECTS | DESKTOP_READOBJECTS |
                  DESKTOP_SWITCHDESKTOP | GENERIC_WRITE);

                  */

            /*
            // Get a handle to the interactive window station.
            HWINSTA hwinsta = OpenWindowStation(_T("winsta0"),               // the interactive window station
                                        FALSE,                       // handle is not inheritable
                                        READ_CONTROL | WRITE_DAC);
                                        */

          /*
            if (!desktop) {
              qDebug("unable to OpenInputDesktop(2):%u", GetLastError());
             // return false;
            } else {
                qDebug() << "desktop created";
            }

            if (!SetThreadDesktop(desktop)) {
              qDebug("switchToDesktop failed:%u", GetLastError());
            } else {
                 qDebug() << "switchToDesktop OK!";
            }
            */

            /*
            //auto hwinsta = OpenWindowStation(L"WinSta0", false, WINSTA_ALL_ACCESS); // when call from windows service OpenWindowStation returns 0
             auto hwinsta = OpenWindowStation(L"WinSta0", true, GENERIC_ALL);
                 qDebug("OpenWindowStation lasterror =%u", GetLastError());
                 qDebug("OpenWindowStation hwinsta= %u", GetLastError());
             auto hwinsta2 = SetProcessWindowStation(hwinsta);
                 qDebug("SetProcessWindowStation lasterror =%u", GetLastError());
                 qDebug("SetProcessWindowStation hwinsta2= %u", GetLastError());
             auto desktop = OpenDesktop(L"default", 0, true, READ_CONTROL | WRITE_DAC | DESKTOP_WRITEOBJECTS | DESKTOP_READOBJECTS);
                 qDebug("OpenDesktop lasterror = %u", GetLastError());
                 qDebug("OpenDesktop hdesk= %u", GetLastError());
             bool Success = SetThreadDesktop(desktop);
                 qDebug("SetThreadDesktop lasterror =%u", GetLastError());
                 qDebug("SetThreadDesktop Success= %u", GetLastError());
                 */

            /*

            auto winsta = OpenWindowStation(L"WinSta0", true, GENERIC_ALL);
                qDebug("OpenWindowStation lasterror =%u", GetLastError());


                SECURITY_ATTRIBUTES attributes = {sizeof(SECURITY_ATTRIBUTES), 0, true};
                attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
                attributes.bInheritHandle = true;
                //hNewDesktop = CreateDesktop("NewDesktopName", NULL, NULL, 0 , GENERIC_ALL,  &stSecurityAttr);


            auto hwinsta2 = SetProcessWindowStation(winsta);
                qDebug("SetProcessWindowStation lasterror =%u", GetLastError());


            // Create the destkop.
            auto desktop = CreateDesktop(L"newdesktopa",
                                       NULL,
                                       NULL,
                                       0,
                                       GENERIC_ALL,
                                       &attributes);
            qDebug("CreateDesktop lasterror =%u", GetLastError());


            //SetThreadDesktop(desktop);
            SwitchDesktop(desktop);
            qDebug("SetThreadDesktop lasterror =%u", GetLastError());
            */


            //qDebug() << "SetThreadDesktop (bool): " << SetThreadDesktop(desktop);

            // get the device context of the screen
            HDC hScreenDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
            // and a device context to put it in
            HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

            int x = GetDeviceCaps(hScreenDC, HORZRES);
            int y = GetDeviceCaps(hScreenDC, VERTRES);

            // maybe worth checking these are positive values
            HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, x, y);

            // get a new bitmap
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

            BitBlt(hMemoryDC, 0, 0, x, y, hScreenDC, 0, 0, SRCCOPY);
            hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

            qimg = QtWin::fromHBITMAP(hBitmap, QtWin::HBitmapNoAlpha).toImage();

            // clean up
            DeleteDC(hMemoryDC);
            DeleteDC(hScreenDC);

            //CloseDesktop(desktop);
}

            //qimg = QImage(QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId()).toImage());
#else
            qimg = QImage(QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId()).toImage());
#endif
            //QImage qimg(QGuiApplication::screens()[QGuiApplication::screens().count()-1]->grabWindow(QApplication::desktop()->winId()).toImage());
            if (qimg.isNull())
            {
                qDebug("---------> qimg is null!!!");
            }
            qDebug("6. Grabwindow succeded: image width: %i. Calling screenshotWrk.setScreenshot",qimg.width());

            //std::cout << "MainWindow::mymessageReceived > Lifthike to screenshot kai tha tethei sto screenshotWrk" << std::endl;
            screenshotWrk.setScreenshot(qimg,msgType);
            //std::cout << "MainWindow::mymessageReceived > To screenshot tethike sto screenshotWrk" << std::endl;
            break;
        case MSG_SCREENSHOT:
            qDebug("Screenshot received. Setting image in control! Total bytes: %lu", vdata.size());
#ifndef ANDAMA_SERVICE_MODE //AndamaService service/daemon does not use qml related code
            screenshotProvider->setFrame(vdata);
#endif //ANDAMA_SERVICE_MODE

            showRemote = true;
            emit showRemoteChanged();
            notifyNewFrame();

            break;
        case MSG_SCREENSHOT_DIFF:
#ifndef ANDAMA_SERVICE_MODE //AndamaService service/daemon does not use qml related code
            if ( screenshotProvider->updateFrame(vdata))
                notifyNewFrame();
#endif //ANDAMA_SERVICE_MODE
            break;

        default:
            break;
        }
    }
    catch (std::exception &ex)
    {
        qDebug("exception: %s",ex.what());
    }
    emit statusTextChanged();
    emit statusErrorLevelChanged();
}
