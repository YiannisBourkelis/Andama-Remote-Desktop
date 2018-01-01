#include "MouseCursorHook.h"
#include <map>

//https://msdn.microsoft.com/en-us/library/windows/desktop/ms648391%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396
//http://doc.qt.io/archives/qt-4.8/qt.html#CursorShape-enum

#ifdef _WIN32

static std::map<HCURSOR, Qt::CursorShape> _sysMouseCursorsMap;
static Qt::CursorShape currentCursorShape = Qt::ArrowCursor;

protocolSupervisor* MouseCursorHook::protocSupervisor;
P2PServer* MouseCursorHook::p2pServer;

MouseCursorHook::MouseCursorHook()
{
    if (_sysMouseCursorsMap.empty()){
        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::ArrowCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_UPARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::UpArrowCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_CROSS, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::CrossCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::ArrowCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_WAIT, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::WaitCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_IBEAM, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::IBeamCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_SIZENS, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::SizeVerCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_SIZEWE, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::SizeHorCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_SIZENESW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::SizeBDiagCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_SIZENWSE, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::SizeFDiagCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_SIZEALL, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::SizeAllCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_HAND, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::PointingHandCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_HELP, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::WhatsThisCursor;

        _sysMouseCursorsMap [(HCURSOR)LoadImage(0, IDC_NO, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED)]
                            = Qt::ForbiddenCursor;


    }
}

static LRESULT CALLBACK WinEventCallback2(_In_ int nCode,
                                          _In_ WPARAM wParam,
                                          _In_ LPARAM lParam)
{
    CURSORINFO curInfo = {};
    curInfo.cbSize = sizeof(curInfo);


    if (GetCursorInfo(&curInfo)){

        auto found_cursor = _sysMouseCursorsMap.find(curInfo.hCursor);

        if ( found_cursor != _sysMouseCursorsMap.end()) { // system cursor shape
            if (currentCursorShape != found_cursor->second){
                currentCursorShape =  found_cursor->second;
                std::cout << "Cursor shape:" << found_cursor->second << std::endl;
                connectionState connState = MouseCursorHook::protocSupervisor->protocol.getConnectionState();
                if (MouseCursorHook::p2pServer->hasConnectedClientThreadsRunning()) {
                    MouseCursorHook::p2pServer->sendMouseCursorType(found_cursor->second);
                } else if (connState == connectionState::connectedWithOtherAsServer) {
                    MouseCursorHook::protocSupervisor->protocol.sendMouseCursorType(found_cursor->second);
                }
            }
        }//system cursor shape
        else if (curInfo.flags == 0){ //hidden cursor
            //Qt::BlankCursor
            if (currentCursorShape != Qt::BlankCursor){
                currentCursorShape = Qt::BlankCursor;
                std::cout << "Hidden (blank) cursor" << std::endl;
            }
        }//hidden cursor
        else {
             std::cout << "Other cursor" << curInfo.hCursor << std::endl;
        }//Other cursor

    }
        return CallNextHookEx(0, nCode, wParam, lParam);
}



void MouseCursorHook::setMouseCursorHook()
{
    _mythreadid = GetCurrentThreadId();

    _mouseHook = SetWindowsHookEx(WH_MOUSE_LL,
                                               WinEventCallback2,
                                               GetModuleHandle(0),
                                               0

                    );
     std::cout << "Mouse Cursor hook set:" << _mouseHook << std::endl;

     MSG message;
     BOOL bRet;


     while ( (bRet = GetMessage(&message, NULL, 0, 0)) != 0){
     //while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE) ){

         std::cout << "bret:" << bRet << std::endl;


         //if (bRet == -1){
         //    break;
         //}

         //***TranslateMessage(&message);
         DispatchMessage(&message);
         //Sleep(1);
     }

    std::cout << "stoping mouse hook thread..." << std::endl;
}

void MouseCursorHook::unhookMouseCursorHook()
{
    if (_mouseHook)
        UnhookWindowsHookEx(_mouseHook);

    BOOL pres;
    pres = PostThreadMessage(_threadid, WM_QUIT, 0, 0);
    std::cout << "PostThreadMessage:" << pres << " LastError:" << GetLastError() << std::endl;

   // _stopThread = true;
}

void MouseCursorHook::run(void)
{
    setMouseCursorHook();
}


#endif

