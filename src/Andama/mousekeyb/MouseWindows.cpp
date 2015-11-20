/* ***********************************************************************
 * Andama
 * (C) 2014 by Yiannis Bourkelis (yiannis@grbytes.com)
 *
 * This file is part of Andama.
 *
 * Andama is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Andama is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Andama.  If not, see <http://www.gnu.org/licenses/>.
 * ***********************************************************************/

#include "Mouse.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#ifdef _WIN32

void Mouse::moveToPoint(const int x, const int y)
{
    // get screen resolution
    HDC hDCScreen = GetDC(NULL);
    int horres = GetDeviceCaps(hDCScreen, HORZRES);
    int vertres = GetDeviceCaps(hDCScreen, VERTRES);
    ReleaseDC(NULL, hDCScreen);

    const int points_in_line = 65535;
    const double points_in_x_pixel = points_in_line / static_cast<double>(horres);
    const double points_in_y_pixel = points_in_line / static_cast<double>(vertres);

    INPUT event;
    event.type = INPUT_MOUSE;
    event.mi.dx = x * points_in_x_pixel; //+ 0.5;
    event.mi.dy = y * points_in_y_pixel; //+ 0.5;
    event.mi.mouseData = 0;
    event.mi.dwFlags =  MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
    event.mi.time = 0;
    event.mi.dwExtraInfo = 0;

    SendInput(1, &event, sizeof(event));
}

void Mouse::press(Qt::MouseButton button)
{
    DWORD btn = button == Qt::LeftButton  ? MOUSEEVENTF_LEFTDOWN   :
                button == Qt::RightButton ? MOUSEEVENTF_RIGHTDOWN  :
                button == Qt::MidButton   ? MOUSEEVENTF_MIDDLEDOWN : 0;

    INPUT event;
    event.type = INPUT_MOUSE;
    event.mi.dx = 0;
    event.mi.dy = 0;
    event.mi.mouseData = 0;
    event.mi.dwFlags = btn;
    event.mi.time = 0;
    event.mi.dwExtraInfo = 0;

    SendInput(1, &event, sizeof(event));
}

void Mouse::release(Qt::MouseButton button)
{
    DWORD btn = button == Qt::LeftButton  ? MOUSEEVENTF_LEFTUP   :
                button == Qt::RightButton ? MOUSEEVENTF_RIGHTUP  :
                button == Qt::MidButton   ? MOUSEEVENTF_MIDDLEUP : 0;

    INPUT event;
    event.type = INPUT_MOUSE;
    event.mi.dx = 0;
    event.mi.dy = 0;
    event.mi.mouseData = 0;
    event.mi.dwFlags = btn;
    event.mi.time = 0;
    event.mi.dwExtraInfo = 0;

    SendInput(1, &event, sizeof(event));
}

void Mouse::scroll(int value)
{
    INPUT event;
    event.type = INPUT_MOUSE;
    event.mi.dx = 0;
    event.mi.dy = 0;
    event.mi.mouseData = value * WHEEL_DELTA;
    event.mi.dwFlags = MOUSEEVENTF_WHEEL;
    event.mi.time = 0;
    event.mi.dwExtraInfo = 0;

    SendInput(1, &event, sizeof(event));
}

void Mouse::doubleClick(Qt::MouseButton button)
{
    press(button);
    release(button);

    std::chrono::milliseconds dura(100);
    std::this_thread::sleep_for(dura);

    press(button);
    release(button);
}
#endif
