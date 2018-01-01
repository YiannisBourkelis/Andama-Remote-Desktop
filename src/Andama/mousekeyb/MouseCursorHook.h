#ifndef MOUSECURSORHOOK_H
#define MOUSECURSORHOOK_H

#include <iostream>
#include <QThread>
#include <atomic>
#include "../../Andama/protocolsupervisor.h"
#include "../../Andama/p2pserver.h"

#ifdef _WIN32
    #include <windows.h>
#endif

class MouseCursorHook  : public QThread
{
    Q_OBJECT

public:
    //constructors
    MouseCursorHook();

    //public static variables
    static protocolSupervisor* protocSupervisor;
    P2PServer * p2pServer;

    //public methods
    void setMouseCursorHook();
    void unhookMouseCursorHook();

private:
    //private variables
#ifdef _WIN32
    HHOOK _mouseHook;
    DWORD _mythreadid;
#endif

    bool _stopThread = false;

protected:
    void run(void);
};

#endif // MOUSECURSORHOOK_H
