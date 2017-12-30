#include "MouseCursorHook.h"

#ifdef _WIN32
MouseCursorHook::MouseCursorHook()
{

}

static HCURSOR c_IDC_SIZEWE,c_IDC_ARROW,c_IDC_SIZENS;

static LRESULT CALLBACK WinEventCallback2(_In_ int nCode,
                                          _In_ WPARAM wParam,
                                          _In_ LPARAM lParam)
{
    CURSORINFO curInfo = {};
    curInfo.cbSize = sizeof(curInfo);


    if (GetCursorInfo(&curInfo)){
        if (curInfo.hCursor == c_IDC_SIZEWE ||
            curInfo.hCursor == c_IDC_SIZENS
               ){
            std::cout << "resize cursor" << std::endl;

        }
    }


        //LPMSLLHOOKSTRUCT lpmh = reinterpret_cast<LPMSLLHOOKSTRUCT>(lParam);

    //if (wParam != 512){
        //std::cout << "Cusros change. mouseData:" << curInfo-> << " wParam:" << wParam << " lParam:" << lParam << std::endl;
//}
        return CallNextHookEx(0, nCode, wParam, lParam);
//return 0L;
}



void MouseCursorHook::setMouseCursorHook()
{
    _mythreadid = GetCurrentThreadId();

    _mouseHook = SetWindowsHookEx(WH_MOUSE_LL,
                                               WinEventCallback2,
                                               GetModuleHandle(0),
                                               0

                    );

    //c_IDC_SIZEWE = LoadCursor(NULL, IDC_SIZEWE);
    //c_IDC_SIZENS = LoadCursor(NULL, IDC_SIZENS);
   // c_IDC_ARROW =  LoadCursor(NULL, IDC_ARROW);
    c_IDC_SIZEWE = (HCURSOR)LoadImage(0, IDC_SIZEWE, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

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

