#include "protocolsupervisor.h"

protocolSupervisor::protocolSupervisor()
{
    clientsocket.protocol = &protocol;
}

void protocolSupervisor::run(void)
{
    clientsocket.connectToServer();
}
