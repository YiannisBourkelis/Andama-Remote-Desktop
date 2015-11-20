#include "cs_thread.h"

clientserver clsrv;

void messageRecieved(int msgType, char data[])
{
    qDebug("msg: %i",msgType);
    //emit sig_messageRecieved(msgType,data);
}

void cs_thread::run()
{
    //clsrv.setMessageRecievedCallback(messageRecieved);
    //clsrv.start();
}
