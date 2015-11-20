/*
#include "appnapcontroller.h"

static std::atomic<bool> appNapThreadIsRunning;
static std::atomic<bool> enableAppNapPending;
static std::thread appNapThread;

AppNapController::AppNapController()
{
    enableAppNapPending = false;
    appNapThreadIsRunning = false;
}

void AppNapController::DisableAppNap(){
    if (!appNapThreadIsRunning){
        appNapThread = std::thread(DisableAppNapInNewThread);
        appNapThread.detach();
    }
}

void AppNapController::EnableAppNap(){
    enableAppNapPending = true;
}

void AppNapController::DisableAppNapInNewThread(){
    std::cout << "AppNapController::DisableAppNapInNewThread() started" << std::endl;
    appNapThreadIsRunning = true;
    while(true){
        //printsomething();
        //disableAppNap();
        //std::this_thread::sleep_for(std::chrono::seconds(5));
        auto t = std::thread(DisableAppNapInNewThreadChild);
        t.join();

        if (enableAppNapPending){
            enableAppNapPending = false;
            break;
        }
    }

    std::cout << "AppNapController::DisableAppNapInNewThread() ending" << std::endl;
    appNapThreadIsRunning = false;
}

void AppNapController::DisableAppNapInNewThreadChild(){
    printsomething();
    disableAppNap();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}
*/
