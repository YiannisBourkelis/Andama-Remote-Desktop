#include "MouseCursorHook.h"

#ifdef Q_OS_MAC
protocolSupervisor* MouseCursorHook::protocSupervisor;

MouseCursorHook::MouseCursorHook()
{
}

void MouseCursorHook::setMouseCursorHook()
{
}

void MouseCursorHook::unhookMouseCursorHook()
{
}

void MouseCursorHook::run(void)
{
    setMouseCursorHook();
}

#endif
