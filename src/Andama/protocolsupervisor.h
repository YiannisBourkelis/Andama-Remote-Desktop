#ifndef PROTOCOLSUPERVISOR_H
#define PROTOCOLSUPERVISOR_H

#include "clientserverprotocol.h"
#include "clientsocket.h"
#include <QThread>

class protocolSupervisor : public QThread
{
    Q_OBJECT
public:
    protocolSupervisor();

    //public variables
    clientServerProtocol protocol;
    clientSocket clientsocket;

protected:
    void run(void);
};

#endif // PROTOCOLSUPERVISOR_H
