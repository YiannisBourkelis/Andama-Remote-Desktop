#include "engine.h"
#include "mousekeyb/Mouse.h"
#include "mousekeyb/Keyboard.h"

#ifdef Q_OS_MAC
#include "osxobjectivecbridge.h"
#include <IOKit/pwr_mgt/IOPMLib.h> //gia na ksypnaei to monitor
#endif


typedef std::vector<char> MyArray; // required for SIGNAL(sig_messageRecieved(const int, const std::vector<char>&))

Engine::Engine(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<MyArray>("MyArray");
    this->connect(&protocol,
                  SIGNAL(sig_messageRecieved(const int, const std::vector<char>&)),
                  this,
                  SLOT(mymessageReceived(const int, const std::vector<char>&)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&protocol,
                  SIGNAL(sig_messageRecieved(const int, const std::vector<char>&)),
                  this,
                  SLOT(non_UI_thread_messageRecieved(const int, const std::vector<char>&)),
                  Qt::ConnectionType::DirectConnection);

    this->connect(&protocol,
                  SIGNAL(sig_exception(QString)),
                  this,
                  SLOT(protocol_exception(QString)),
                  Qt::ConnectionType::AutoConnection);

    this->connect(&protocol,
                  SIGNAL(finished()),
                  this,
                  SLOT(slot_protocol_finished_or_terminated()),
                  Qt::ConnectionType::AutoConnection);
}

Engine::Engine(ScreenshotProvider* provider,QObject* parent) : Engine(parent)
{
    screenshotProvider = provider;
}


void Engine::start(){
    screenshotWrk.protocol = &protocol;
    screenshotWrk.imageQuality=80;
    screenshotWrk.start();

    keepAlive.protocol = &protocol;
    keepAlive.start();

    protocol.start();

}


void Engine::slot_protocol_finished_or_terminated()
{
    protocol.setConnectionState(connectionState::disconnected);
    while (protocol.isRunning()) {
        std::chrono::milliseconds sleep_dura(10);
        std::this_thread::sleep_for(sleep_dura);
    }
    protocol.start();
}

void Engine::protocol_exception(QString ex)
{
    exceptionErrorText = ex;
    emit exceptionErrorTextChanged();
}

void Engine::non_UI_thread_messageRecieved(const int msgType, const std::vector<char>& vdata)
{
    try {
        if (msgType == protocol.MSG_MOUSE)
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

        else if (msgType == protocol.MSG_KEYBOARD)
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
                if (protocol.getRemoteComputerOS() == OS::Windows){
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
                if (protocol.getRemoteComputerOS() == OS::MacOS){
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
        case clientserver::MSG_ID:
            statusErrorLevel=NOERROR;
            strID = std::string(vdata.begin(),vdata.end());
            id = QString::fromStdString(strID);

            statusText="Ready!";
            emit idChanged();
            break;
        case clientserver::MSG_LOCAL_PASSWORD_GENERATED:
            password = QString::fromStdString(protocol.password);
            emit passwordChanged();
            break;
        case clientserver::MSG_CONNECTION_ACCEPTED:
            statusErrorLevel = NOERROR;
            statusText="Remote computer accepted the request. Requesting remote desktop image...";
            protocol.RequestScreenshot();
            break;
        case clientserver::MSG_CONNECT_ID_NOT_FOUND:
            statusErrorLevel = NOERROR;
            remoteErrorIdText="Remote ID not found";
            statusText="Ready!";
            break;
        case clientserver::MSG_CONNECT_PASSWORD_NOT_CORRECT:
            statusErrorLevel = NOERROR;
            remoteErrorPasswordText="The password is not correct";
            statusText="Ready!";
            break;
        case clientserver::MSG_BAN_IP_WRONG_PWD:
            statusErrorLevel = ERROR;
            remoteErrorPasswordText="";
            statusText="Connection rejected because of multiple wrong password attempts";
            break;
        case clientserver::MSG_WARNING_BAN_IP_WRONG_PWD:
            statusErrorLevel = WARNING;
            remoteErrorPasswordText="The password is not correct";
            statusText="The password is not correct. Remaining tries: !";
            break;
        case clientserver::MSG_ERROR_APP_VERSION_NOT_ACCEPTED:
            statusErrorLevel = ERROR;
            remoteErrorPasswordText="";
            statusText="Upgrade required";
            break;
        case clientserver::MSG_REMOTE_CLIENT_ACCEPTED:
            statusErrorLevel = NOERROR;
            statusText="Remote client accepted.";
            //deactivate button
            break;
        case clientserver::MSG_NO_INTERNET_CONNECTION:
            //deactivate remote panel
            statusErrorLevel = ERROR;
            statusText="Error connecting. Please make sure you have an internet connection and try again.";
            break;
        case clientserver::MSG_NO_PROXY_CONNECTION:
            statusErrorLevel = ERROR;
            statusText="Error connecting with proxy server. Please check your internet connection or try again later";
            break;
        case clientserver::MSG_REMOTE_COMPUTER_DISCONNECTED:
            statusErrorLevel = NOERROR;
            statusText="Remote computer disconnected. Ready!";
            break;
        case clientserver::MSG_WARNING_BAN_IP_WRONG_ID:
            statusErrorLevel = WARNING;
            statusText="Warning! Your IP will be banned because of multiple wrong ID attempts. Remaining tries: ";
            break;
        case clientserver::MSG_BAN_IP_WRONG_ID:
            statusErrorLevel = ERROR;
            statusText="Connection rejected because of multiple wrong ID attempts. Please try again after ";
            break;
        case clientserver::MSG_ERROR_CANNOT_CONNECT_SAME_ID:
            statusErrorLevel = NOERROR;
            remoteErrorIdText="Cannot connect to the same ID";
            statusText= "Ready!";
            break;
        case clientserver::MSG_SCREENSHOT_DIFF_REQUEST:
        case clientserver::MSG_SCREENSHOT_REQUEST:
            statusErrorLevel = NOERROR;
            statusText="Ready!";
            //screenshotWrk.setScreenshot(qimg,msgType);

            //std::cout << "MainWindow::mymessageRecieved > Lipsi aitimatos gia apostoli neou screenshot" << std::endl;
 #ifdef Q_OS_MAC
            //xreiazetai gia na anoigei to monitor se periptwsi
            //pou einai kleisto. Sta windows anoigei mono tou.
            //Sto linux den exw dokimasei (todo)
            //Kanonika xreiazetai hook wste na vlepei kai sti synexeia
            //ean to monitor koimatai wste na to anoigei (todo)
            if (msgType == protocol.MSG_SCREENSHOT_REQUEST){
                 IOPMAssertionID assertionID;
                 IOPMAssertionDeclareUserActivity(CFSTR(""),
                                                  kIOPMUserActiveLocal,
                                                  &assertionID);
            }

            objcappnap::disableAppNap();
 #endif


            //qDebug("5. UI myMessageRecieved: Screensot request recieved. Etoimasia apostolis screenshot me grabwindow");
            qimg = QImage(QGuiApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId()).toImage());
            //QImage qimg(QGuiApplication::screens()[QGuiApplication::screens().count()-1]->grabWindow(QApplication::desktop()->winId()).toImage());
            if (qimg.isNull())
            {
                qDebug("---------> qimg is null!!!");
            }
            //qDebug("6. Grabwindow succeded: image width: %i. Calling screenshotWrk.setScreenshot",qimg.width());

            //std::cout << "MainWindow::mymessageRecieved > Lifthike to screenshot kai tha tethei sto screenshotWrk" << std::endl;
            screenshotWrk.setScreenshot(qimg,msgType);
            //std::cout << "MainWindow::mymessageRecieved > To screenshot tethike sto screenshotWrk" << std::endl;
            break;
        case clientserver::MSG_SCREENSHOT:
            qDebug("Screenshot recieved. Setting image in control! Total bytes: %lu", vdata.size());
            screenshotProvider->setFrame(vdata);

            showRemote = true;
            emit showRemoteChanged();
            notifyNewFrame();

            break;
        case clientserver::MSG_SCREENSHOT_DIFF:
            if ( screenshotProvider->updateFrame(vdata))
                notifyNewFrame();

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
