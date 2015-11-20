#ifndef CS_THREAD_H
#define CS_THREAD_H

#include "qthread.h"
#include "clientserver.h"
#include "qdebug.h"

class cs_thread : public QThread
{
    Q_OBJECT

public:
    clientserver clsrv;

protected:
    void run();

signals:
    void sig_messageRecieved(int msgType, char data[]);

};

#endif // CS_THREAD_H
