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

#ifdef Q_OS_MAC
#include <ApplicationServices/ApplicationServices.h>
#endif

#ifdef Q_OS_MAC
Qt::MouseButtons Mouse::bp = Qt::NoButton;



void Mouse::moveToPoint(const int x, const int y)
{
    if(bp.testFlag(Qt::LeftButton)){
            //qDebug("--> MOUSE ::: moveToPoint.selectArea x:%i, y:%i",x,y);
        selectArea(x,y);
        return;
    }

    //qDebug("--> MOUSE ::: moveToPoint x:%i, y:%i",x,y);

    CGEventRef event = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, CGPointMake(x, y), kCGMouseButtonLeft /*ignored*/);

    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
}

void Mouse::selectArea(const int x, const int y)
{
    CGEventRef event = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDragged, CGPointMake(x, y), kCGMouseButtonLeft /*ignored*/);

    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
    //sleep(100); //!! kolaei ean ginetai sleep edw
}

void Mouse::press(Qt::MouseButton button)
{
    //qDebug("--> MOUSE ::: press button: %i",button);
    bp |= button;
    QPoint mousePos = QCursor::pos();
    CGEventType eventType =     button == Qt::LeftButton  ? kCGEventLeftMouseDown  :
                                button == Qt::RightButton ? kCGEventRightMouseDown :
                                button == Qt::MidButton   ? kCGEventOtherMouseDown : kCGEventNull;

    CGEventRef event = CGEventCreateMouseEvent(NULL, eventType, CGPointMake(mousePos.x(), mousePos.y()), kCGMouseButtonLeft /*ignored*/);

    CGEventPost(kCGHIDEventTap, event);
    sleep(0); // don't touch, it's Mac's magic
    CFRelease(event);
}

void Mouse::release(Qt::MouseButton button)
{
    //qDebug("--> MOUSE ::: release button: %i",button);

    bp ^= button;
    QPoint mousePos = QCursor::pos();
    CGEventType eventType =     button == Qt::LeftButton  ? kCGEventLeftMouseUp  :
                                button == Qt::RightButton ? kCGEventRightMouseUp :
                                button == Qt::MidButton   ? kCGEventOtherMouseUp : kCGEventNull;
    CGEventRef event = CGEventCreateMouseEvent(NULL, eventType, CGPointMake(mousePos.x(), mousePos.y()), kCGMouseButtonLeft /*ignored*/);

    CGEventPost(kCGHIDEventTap, event);
    sleep(0); // don't touch, it's Mac's magic
    CFRelease(event);
}

void Mouse::doubleClick(Qt::MouseButton button)
{
    //qDebug("--> MOUSE ::: doubleClick button: %i",button);

    //**********TODO: find out why click(os) is needed here*********
                               press(button);
                               release(button);
    //**************************************************************
    QPoint mousePos = QCursor::pos();
    CGEventType eventTypeMouseDown = kCGEventLeftMouseDown ;
    CGEventRef eventPress = CGEventCreateMouseEvent(NULL, eventTypeMouseDown, CGPointMake(mousePos.x(), mousePos.y()), kCGMouseButtonLeft /*ignored*/);

    CGEventType eventTypeMouseUp = kCGEventLeftMouseUp ;
    CGEventRef eventRelease = CGEventCreateMouseEvent(NULL, eventTypeMouseUp, CGPointMake(mousePos.x(), mousePos.y()), kCGMouseButtonLeft /*ignored*/);

    CGEventSetDoubleValueField(eventPress, kCGMouseEventClickState, 2);
    CGEventSetDoubleValueField(eventRelease, kCGMouseEventClickState, 2);

    CGEventPost(kCGHIDEventTap, eventPress);
    sleep(0); // don't touch, it's Mac's magic
    CGEventPost(kCGHIDEventTap, eventRelease);
    sleep(0);

    CFRelease(eventPress);
    CFRelease(eventRelease);
}

void Mouse::scroll(int value)
{
    //CGEventRef event = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 1, value > 0 ? 10 : -10);
    int32_t xScroll = -1; // Negative for right
    int32_t yScroll = -2; // Negative for down
    CGEventRef event = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitLine, value,value < 0 ? -1 * yScroll : yScroll, xScroll);


    //  Scrolling movement is generally represented by small signed integer values, typically in a range from -10 to +10.
    //  Large values may have unexpected results, depending on the application that processes the event.
    value = value > 0 ? value : -value;
    for (int i = 0; i < value; i += 10) {
        CGEventPost(kCGHIDEventTap, event);
        sleep(0); // don't touch, it's Mac's magic
    }

    CFRelease(event);
}

#else

/*
void Mouse::moveToPoint(const int x, const int y)
{
}

void Mouse::selectArea(const int x, const int y)
{
}

void Mouse::press(Qt::MouseButton button)
{
}

void Mouse::release(Qt::MouseButton button)
{
}

void Mouse::doubleClick(Qt::MouseButton button)
{
}

void Mouse::scroll(int value)
{
}
*/
#endif

