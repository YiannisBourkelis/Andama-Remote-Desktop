#ifndef MOUSECURSORHOOK_H
#define MOUSECURSORHOOK_H

#include <iostream>
#include <QThread>
#include <atomic>

#ifdef _WIN32
    #include <windows.h>
#endif


class MouseCursorHook  : public QThread
{
    Q_OBJECT

public:
    //constructors
    MouseCursorHook();

    //public methods
    void setMouseCursorHook();
    void unhookMouseCursorHook();

private:
    //private variables
    HHOOK _mouseHook;
    bool _stopThread = false;
    DWORD _mythreadid;

protected:
    void run(void);
};

#endif // MOUSECURSORHOOK_H
