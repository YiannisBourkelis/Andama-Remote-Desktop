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

#ifndef MOUSE_H
#define MOUSE_H

#include <thread>
#include <QtGlobal>
#include <QtEvents>
#include <QDebug>

class Mouse {
public:
    // fails if given coordinates are not in the screen's rect
    // Linux: fails if there is an opening X display error
    static void moveToPoint(const int x, const int y);

    static void doubleClick(Qt::MouseButton button);

    // fails if the given button is not Qt::LeftButton, Qt::RightButton or Qt::MiddleButton
    // Linux: fails if there is an opening X display error
    static void press(Qt::MouseButton button);
    static void release(Qt::MouseButton button);

    // Linux: fails if there is an opening X display error
    static void scroll(int value); //positive values for scrolling up, negative for scrolling down

private:
    static Qt::MouseButtons bp;
    static void selectArea(const int x, const int y);
};
#endif // MOUSE_H
