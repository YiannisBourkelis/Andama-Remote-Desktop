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

#include "Keyboard.h"

#ifdef _WIN32
void Keyboard::keyPress(int key, int modifiers) {
    /*
    if (modifiers) {
        INPUT input = getKeyEvent(modifiers);
        SendInput(1, &input, sizeof(input));
    }
    */

    INPUT input = getKeyEvent(key);
    SendInput(1, &input, sizeof(input));
}

void Keyboard::keyRelease(int key, int modifiers)
{
    /*
    if (modifiers) {
        INPUT input = getKeyEvent(modifiers, true);
        SendInput(1, &input, sizeof(input));
    }
    */

    INPUT input = getKeyEvent(key, true);
    SendInput(1, &input, sizeof(input));
}

INPUT Keyboard::getKeyEvent(int key, bool keyUp) {

    INPUT event;
    event.type = INPUT_KEYBOARD;
    event.ki.wVk = key;
    event.ki.wScan = MapVirtualKey(key, VIRTUAL_TO_SCAN_CODE);
    event.ki.dwFlags = keyUp ? KEYEVENTF_KEYUP : 0;
    event.ki.time = 0;
    event.ki.dwExtraInfo = 0;

    return event;
}

portableVKey Keyboard::getPortableVKey(int key, int qtkey)
{
    return (portableVKey)key;
}

int Keyboard::convertPortableKeyToLocal(portableVKey portableKey)
{
    return portableKey;
}

#endif
