#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "screenshotsworker.h"
#include "screenshotprovider.h"
#include "clientserver.h"
#include "keepalive.h"

#ifdef Q_OS_WIN
#include <QtWinExtras/QtWin>
#include <qt_windows.h>
#include <windows.h>
#endif

class Engine : public QObject
{
    Q_OBJECT
    Q_ENUMS(StatusErrorLevelEnum)
    Q_PROPERTY(QString id READ getId NOTIFY idChanged)
    Q_PROPERTY(QString password READ getPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString exceptionErrorText READ getExceptionErrorText NOTIFY exceptionErrorTextChanged)
    Q_PROPERTY(QString statusText READ getStatusText NOTIFY statusTextChanged)
    Q_PROPERTY(StatusErrorLevelEnum statusErrorLevel READ getStatusErrorLevel NOTIFY statusErrorLevelChanged)
    Q_PROPERTY(QString remoteErrorIdText READ getRemoteErrorIdText NOTIFY remoteErrorIdTextChanged)
    Q_PROPERTY(QString remoteErrorPasswordText READ getRemoteErrorPasswordText NOTIFY remoteErrorPasswordTextChanged)
    Q_PROPERTY(bool showRemote READ getShowRemote NOTIFY showRemoteChanged)


public:
    clientserver protocol;

    enum StatusErrorLevelEnum{
        STATUS_NOERROR,
        STATUS_WARNING,
        STATUS_ERROR
    };

    explicit Engine(QObject* parent);
#ifndef ANDAMA_SERVICE_MODE //a service/daemon does not use qml related code
    explicit Engine(ScreenshotProvider* provider, QObject* parent = 0);
#endif //ANDAMA_SERVICE_MODE


    QString getId(){ return id;}
    QString getPassword(){ return password;}
    QString getExceptionErrorText(){ return exceptionErrorText;}
    QString getStatusText(){ return statusText;}
    StatusErrorLevelEnum getStatusErrorLevel(){ return statusErrorLevel;}
    QString getRemoteErrorIdText(){ return remoteErrorIdText;}
    QString getRemoteErrorPasswordText(){ return remoteErrorPasswordText;}
    bool getShowRemote(){ return showRemote;}


    Q_INVOKABLE void connectToRemote(QString remoteID, QString remotePassword){
        std::string strRemoteID=remoteID.toStdString();
        std::vector<char> vectRemoteID = std::vector<char>(strRemoteID.begin(),strRemoteID.end());
        std::string strRemotePasw=remotePassword.toStdString();
        std::vector<char> vectRemotePass = std::vector<char>(strRemotePasw.begin(),strRemotePasw.end());
        statusText="Connecting to remote computer...";
        emit statusTextChanged();
        protocol.Connect(vectRemoteID,vectRemotePass);

    }

    Q_INVOKABLE void sendMouse(int x, int y, int button, int mouseEvent, int wheelDelta, int wheelDeltaSign, int wheelOrientation){
        protocol.sendMouse(x, y, button, mouseEvent, wheelDelta, wheelDeltaSign, wheelOrientation);
    }

    Q_INVOKABLE void sendKeyboard(quint32 portableVKey, int key, bool autoRepeat, int count, int portableModifiers, int keyEvent){
        Q_UNUSED(key);Q_UNUSED(autoRepeat);Q_UNUSED(count);

        //FIXME handle portable key like  : //portableVKey natPKey = Keyboard::getPortableVKey(portableVKey,key);
        protocol.sendKeyboard(portableVKey, portableModifiers, keyEvent);
    }


    void start();


    enum Status{
        DISCONNECTED,
        CONNECTED_WITH_PROXY,
                  MSG_CONNECTION_ACCEPTED,
                  MSG_REMOTE_CLIENT_ACCEPTED,

                  MSG_NO_INTERNET_CONNECTION,
                  MSG_NO_PROXY_CONNECTION,
                  MSG_REMOTE_COMPUTER_DISCONNECTED,

                  MSG_BAN_IP_WRONG_ID,
                  MSG_WARNING_BAN_IP_WRONG_ID,
                  MSG_CONNECT_PASSWORD_NOT_CORRECT,
                  MSG_BAN_IP_WRONG_PWD,
                  MSG_WARNING_BAN_IP_WRONG_PWD,
                  MSG_ERROR_CANNOT_CONNECT_SAME_ID,
                  MSG_ERROR_APP_VERSION_NOT_ACCEPTED,
                  MSG_ERROR_PROTOCOL_VERSION_NOT_ACCEPTED,
                 };

signals:
    void idChanged();
    void passwordChanged();
    void exceptionErrorTextChanged();
    void statusTextChanged();
    void statusErrorLevelChanged();
    void remoteErrorIdTextChanged();
    void remoteErrorPasswordTextChanged();
    void showRemoteChanged();
    void newFrameAvailable(uint frameNumber);

public slots:
    void slot_protocol_finished_or_terminated();
    void protocol_exception(QString);
    void non_UI_thread_messageRecieved(const int msgType, const std::vector<char>& vdata);
    void mymessageReceived(const int msgType,const std::vector<char>& vdata);

private:
    void notifyNewFrame(){
        static uint frameNumber = 0;
        emit newFrameAvailable(frameNumber++);
    }

    QString id;
    QString password;
    //Status status; //TODO
    QString exceptionErrorText;
    QString statusText;
    StatusErrorLevelEnum statusErrorLevel;

    QString remoteErrorIdText;
    QString remoteErrorPasswordText;

    QImage lastScreenshot;
    screenshotsWorker screenshotWrk;
    keepalive keepAlive;


    bool showRemote = false; //Whether the remote screenshot is to be shown

#ifndef ANDAMA_SERVICE_MODE
    ScreenshotProvider* screenshotProvider;
#endif //ANDAMA_SERVICE_MODE

};

#endif // ENGINE_H
