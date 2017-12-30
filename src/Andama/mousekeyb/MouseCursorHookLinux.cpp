#include "MouseCursorHook.h"

#ifdef Q_OS_LINUX
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
