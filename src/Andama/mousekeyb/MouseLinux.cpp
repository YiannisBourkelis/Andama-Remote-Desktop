/* ***********************************************************************
 * Andama
 * (C) 2014 by Yiannis Bourkelis (hello@andama.org)
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
#include <QPoint>
#include <QCursor>

#ifdef Q_OS_LINUX

#include <X11/extensions/XTest.h>

Qt::MouseButtons Mouse::bp = Qt::NoButton;



void Mouse::moveToPoint(const int x, const int y)
{
    QByteArray display_name = qgetenv("DISPLAY");

    Display *display = XOpenDisplay(display_name.constData());

    int horres = XDisplayWidth(display, 0);
    int vertres = XDisplayHeight(display, 0);

    QRect screen(0, 0, horres-1, vertres-1);

    const int delay = 0;// msec
    XTestFakeMotionEvent(display, -1, x, y, delay);
    XFlush(display);
    XCloseDisplay(display);
}

void Mouse::selectArea(const int x, const int y)
{

}

void Mouse::press(Qt::MouseButton button)
{
    QByteArray display_name = qgetenv("DISPLAY");

    Display *display = XOpenDisplay(display_name.constData());

    //1 = Left, 2 = Middle, 3 = Right
    unsigned int btn = button == Qt::LeftButton ? 1 :
                       button == Qt::RightButton ? 3 :
                       button == Qt::MidButton ? 2 : 0;

    XTestFakeButtonEvent(display, btn, True, 0);
    XFlush(display);

    XCloseDisplay(display);
}

void Mouse::release(Qt::MouseButton button)
{
    QByteArray display_name = qgetenv("DISPLAY");

    Display *display = XOpenDisplay(display_name.constData());

    unsigned int btn = button == Qt::LeftButton ? 1 :
                       button == Qt::RightButton ? 3 :
                       button == Qt::MidButton ? 2 : 0;

    XTestFakeButtonEvent(display, btn, False, 0);
    XFlush(display);

    XCloseDisplay(display);
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

void Mouse::scroll(int value)
{
    QByteArray display_name = qgetenv("DISPLAY");

    Display *display = XOpenDisplay(display_name.constData());

    unsigned button =  value > 0 ? Button4 : Button5; //Button4 - scroll up, Button5 - scroll down
    value = value > 0 ? value : -value;

    for (int i = 0; i < value; i++) {
        XTestFakeButtonEvent(display, button, True, 0);
        XTestFakeButtonEvent(display, button, False, 0);
    }

    XFlush(display);
    XCloseDisplay(display);
}
#endif



