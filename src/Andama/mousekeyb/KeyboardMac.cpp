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

#ifdef Q_OS_MAC
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>

void Keyboard::keyPress(int key, int modifiers)
{
    CGEventRef event = CGEventCreateKeyboardEvent(NULL, key, true);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
}

void Keyboard::keyRelease(int key, int modifiers)
{
    CGEventRef event = CGEventCreateKeyboardEvent(NULL, key, false);
    CGEventPost(kCGHIDEventTap, event);
    CFRelease(event);
}


//den exw ylopoiisei keys pou den yparxoun ston mac: pause/break, insert , scroll lock ,num lock

portableVKey Keyboard::getPortableVKey(int key, int qtkey)
{
    if (key == 0){
        switch (qtkey){
        case Qt::Key_Shift:
            return portableVKey::PVK_SHIFT;
            break;
        case Qt::Key_Control:
            return portableVKey::PVK_CONTROL;
            break;
        case Qt::Key_Alt:
            return portableVKey::PVK_ALT;
            break;
        case Qt::Key_CapsLock:
            return portableVKey::PVK_CAPSLOCK;
        case Qt::Key_Meta://ston mac to Ctrl aposteletai ws aristero Windows Key
            return portableVKey::PVK_LWIN;
        }
    }

    switch (key)
    {
    case kVK_ANSI_A:
        return portableVKey::PVK_A;
        break;
    case kVK_ANSI_S:
        return portableVKey::PVK_S;
        break;
    case kVK_ANSI_D:
        return portableVKey::PVK_D;
        break;
    case kVK_ANSI_F:
        return portableVKey::PVK_F;
        break;
            case kVK_ANSI_H:
        return portableVKey::PVK_H;
        break;
    case kVK_ANSI_G:
        return portableVKey::PVK_G;
        break;
    case kVK_ANSI_Z:
        return portableVKey::PVK_Z;
        break;
    case kVK_ANSI_X:
    return portableVKey::PVK_X;
    break;
        case kVK_ANSI_C:
    return portableVKey::PVK_C;
    break;
        case kVK_ANSI_V:
    return portableVKey::PVK_V;
    break;
        case kVK_ANSI_B:
    return portableVKey::PVK_B;
    break;
        case kVK_ANSI_Q:
    return portableVKey::PVK_Q;
    break;
        case kVK_ANSI_W:
    return portableVKey::PVK_W;
    break;
        case kVK_ANSI_E:
    return portableVKey::PVK_E;
    break;
        case kVK_ANSI_R:
    return portableVKey::PVK_R;
    break;
        case kVK_ANSI_Y:
    return portableVKey::PVK_Y;
    break;
        case kVK_ANSI_T:
    return portableVKey::PVK_T;
    break;
        case kVK_ANSI_1:
    return portableVKey::PVK_1;
    break;
        case kVK_ANSI_2:
    return portableVKey::PVK_2;
    break;
        case kVK_ANSI_3:
    return portableVKey::PVK_3;
    break;
        case kVK_ANSI_4:
    return portableVKey::PVK_4;
    break;
        case kVK_ANSI_6:
    return portableVKey::PVK_6;
    break;
        case kVK_ANSI_5:
    return portableVKey::PVK_5;
    break;
    case kVK_ANSI_Equal:
    return portableVKey::PVK_EQUAL;
    break;
        case kVK_ANSI_9:
    return portableVKey::PVK_9;
    break;
        case kVK_ANSI_7:
    return portableVKey::PVK_7;
    break;
    case kVK_ANSI_Minus:
    return portableVKey::PVK_MINUS;
    break;
        case kVK_ANSI_8:
    return portableVKey::PVK_8;
    break;
        case kVK_ANSI_0:
    return portableVKey::PVK_0;
    break;
    case kVK_ANSI_RightBracket:
    return portableVKey::PVK_RIGHT_BRACKET;
    break;
        case kVK_ANSI_O:
    return portableVKey::PVK_O;
    break;
        case kVK_ANSI_U:
    return portableVKey::PVK_U;
    break;
        case kVK_ANSI_LeftBracket:
    return portableVKey::PVK_LEFT_BRACKET;
    break;
        case kVK_ANSI_I:
    return portableVKey::PVK_I;
    break;
        case kVK_ANSI_P:
    return portableVKey::PVK_P;
    break;
        case kVK_ANSI_L:
    return portableVKey::PVK_L;
    break;
        case kVK_ANSI_J:
    return portableVKey::PVK_J;
    break;
        case kVK_ANSI_Quote:
    return portableVKey::PVK_QUOTE;
    break;
        case kVK_ANSI_K:
    return portableVKey::PVK_K;
    break;
    case kVK_Shift:
        return portableVKey::PVK_SHIFT;
        break;
    case kVK_Command:
        return portableVKey::PVK_CONTROL;
        break;
    case kVK_Option:
        return portableVKey::PVK_ALT;
        break;
    case kVK_ANSI_Semicolon:
    return portableVKey::PVK_SEMICOLON;
    break;
        case kVK_ANSI_Backslash:
    return portableVKey::PVK_BACKSLASH;
    break;
        case kVK_ANSI_Comma:
    return portableVKey::PVK_COMMA;
    break;
        case kVK_ANSI_Slash:
    return portableVKey::PVK_SLASH;
    break;
        case kVK_ANSI_N:
    return portableVKey::PVK_N;
    break;
        case kVK_ANSI_M:
    return portableVKey::PVK_M;
    break;
    case kVK_ANSI_Period:
        return portableVKey::PVK_PERIOD;
        break;
    case kVK_ANSI_Grave:
        return portableVKey::PVK_GRAVE;
        break;

    case kVK_ANSI_KeypadDecimal:
    return portableVKey::PVK_DECIMAL;
    break;
    case kVK_ANSI_KeypadMultiply:
    return portableVKey::PVK_MULTIPLY;
    break;
    case kVK_ANSI_KeypadPlus:
    return portableVKey::PVK_ADD;
    break;
    //case kVK_ANSI_KeypadClear:
    //return portableVKey::PVK_KEYPAD_CLEAR;
    //break;
    case kVK_ANSI_KeypadDivide:
    return portableVKey::PVK_KEYPAD_DIVIDE;
    break;
    //case kVK_ANSI_KeypadEnter:
    //return portableVKey::PVK_KEYPAD_ENTER;
    //break;
    case kVK_ANSI_KeypadMinus:
    return portableVKey::PVK_KEYPAD_MINUS;
    break;

    case kVK_ANSI_Keypad0:
    return portableVKey::PVK_NUMPAD0;
    break;
    case kVK_ANSI_Keypad1:
    return portableVKey::PVK_NUMPAD1;
    break;
    case kVK_ANSI_Keypad2:
    return portableVKey::PVK_NUMPAD2;
    break;
    case kVK_ANSI_Keypad3:
    return portableVKey::PVK_NUMPAD3;
    break;
    case kVK_ANSI_Keypad4:
    return portableVKey::PVK_NUMPAD4;
    break;
    case kVK_ANSI_Keypad5:
    return portableVKey::PVK_NUMPAD5;
    break;
    case kVK_ANSI_Keypad6:
    return portableVKey::PVK_NUMPAD6;
    break;
    case kVK_ANSI_Keypad7:
    return portableVKey::PVK_NUMPAD7;
    break;
    case kVK_ANSI_Keypad8:
    return portableVKey::PVK_NUMPAD8;
    break;
    case kVK_ANSI_Keypad9:
    return portableVKey::PVK_NUMPAD9;
    break;

    case kVK_Return:
    return portableVKey::PVK_RETURN;
    break;
    case kVK_Tab:
    return portableVKey::PVK_TAB;
    break;
    case kVK_Space:
    return portableVKey::PVK_SPACE;
    break;
    case kVK_Delete:
    return portableVKey::PVK_BACK;
    break;
    case kVK_Escape:
    return portableVKey::PVK_ESCAPE;
    break;
    case kVK_CapsLock:
    return portableVKey::PVK_CAPSLOCK;
    break;
    case kVK_F17:
    return portableVKey::PVK_F17;
    break;
    case kVK_VolumeUp:
    return portableVKey::PVK_VOLUME_UP;
    break;
    case kVK_VolumeDown:
    return portableVKey::PVK_VOLUME_DOWN;
    break;
    case kVK_Mute:
    return portableVKey::PVK_VOLUME_MUTE;
    break;
    case kVK_F18:
    return portableVKey::PVK_F18;
    break;
    case kVK_F19:
    return portableVKey::PVK_F19;
    break;
    case kVK_F20:
    return portableVKey::PVK_F20;
    break;
    case kVK_F5:
    return portableVKey::PVK_F5;
    break;
    case kVK_F6:
    return portableVKey::PVK_F6;
    break;
    case kVK_F7:
    return portableVKey::PVK_F7;
    break;
    case kVK_F3:
    return portableVKey::PVK_F3;
    break;
    case kVK_F8:
    return portableVKey::PVK_F8;
    break;
    case kVK_F9:
    return portableVKey::PVK_F9;
    break;
    case kVK_F11:
    return portableVKey::PVK_F11;
    break;
    case kVK_F13:
    return portableVKey::PVK_F13;
    break;
    case kVK_F16:
    return portableVKey::PVK_F16;
    break;
    case kVK_F14:
    return portableVKey::PVK_F14;
    break;
    case kVK_F10:
    return portableVKey::PVK_F10;
    break;
    case kVK_F12:
    return portableVKey::PVK_F12;
    break;
    case kVK_F15:
    return portableVKey::PVK_F15;
    break;
    case kVK_Help:
    return portableVKey::PVK_HELP;
    break;
    case kVK_Home:
    return portableVKey::PVK_HOME;
    break;
    case kVK_PageUp:
    return portableVKey::PVK_PAGE_UP;
    break;
    case kVK_ForwardDelete:
    return portableVKey::PVK_DELETE; // ?
    break;
    case kVK_F4:
    return portableVKey::PVK_F4;
    break;
    case kVK_End:
    return portableVKey::PVK_END;
    break;
    case kVK_F2:
    return portableVKey::PVK_F2;
    break;
    case kVK_PageDown:
    return portableVKey::PVK_PAGE_DOWN;
    break;
    case kVK_F1:
    return portableVKey::PVK_F1;
    break;
    case kVK_LeftArrow:
    return portableVKey::PVK_LEFT;
    break;
    case kVK_RightArrow:
    return portableVKey::PVK_RIGHT;
    break;
    case kVK_DownArrow:
    return portableVKey::PVK_DOWN;
    break;
    case kVK_UpArrow:
    return portableVKey::PVK_UP;
    break;


        /*


#define VK_0	0x30
#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define VK_4	0x34
#define VK_5	0x35
#define VK_6	0x36
#define VK_7	0x37
#define VK_8	0x38
#define VK_9	0x39
///////
#define VK_A	0x041
#define VK_B	0x042
#define VK_C	0x043
#define VK_D	0x044
#define VK_E	0x045
#define VK_F	0x046
#define VK_G	0x047
#define VK_H	0x048
#define VK_I	0x049
#define VK_J	0x04A
#define VK_K	0x04B
#define VK_L	0x04C
#define VK_M	0x04D
#define VK_N	0x04E
#define VK_O	0x04F
#define VK_P	0x050
#define VK_Q	0x051
#define VK_R    0x052
#define VK_S	0x053
#define VK_T	0x054
#define VK_U	0x055
#define VK_V	0x056
#define VK_W	0x057
#define VK_X	0x058
#define VK_Y	0x059
#define VK_Z	0x05A
*/
    }

    return PVK_UNKNOWN; // den vrethike annalogo key
}

int Keyboard::convertPortableKeyToLocal(portableVKey portableKey)
{
        switch (portableKey)
        {
        case portableVKey::PVK_A:
            return kVK_ANSI_A;
            break;
        case portableVKey::PVK_S:
            return kVK_ANSI_S;
            break;
        case portableVKey::PVK_D:
            return kVK_ANSI_D;
            break;
        case portableVKey::PVK_F:
            return kVK_ANSI_F;
            break;
        case portableVKey::PVK_H:
            return kVK_ANSI_H;
            break;
        case portableVKey::PVK_G:
            return kVK_ANSI_G;
            break;
        case portableVKey::PVK_Z:
            return kVK_ANSI_Z;
            break;
        case portableVKey::PVK_X:
            return kVK_ANSI_X;
            break;
        case portableVKey::PVK_C:
            return kVK_ANSI_C;
            break;
        case portableVKey::PVK_V:
            return kVK_ANSI_V;
            break;
        case portableVKey::PVK_B:
            return kVK_ANSI_B;
            break;
        case portableVKey::PVK_Q:
            return kVK_ANSI_Q;
            break;
        case portableVKey::PVK_W:
            return kVK_ANSI_W;
            break;
        case portableVKey::PVK_E:
            return kVK_ANSI_E;
            break;
        case portableVKey::PVK_R:
            return kVK_ANSI_R;
            break;
        case portableVKey::PVK_Y:
            return kVK_ANSI_Y;
            break;
        case portableVKey::PVK_T:
            return kVK_ANSI_T;
            break;
        case portableVKey::PVK_1:
            return kVK_ANSI_1;
            break;
        case portableVKey::PVK_2:
            return kVK_ANSI_2;
            break;
        case portableVKey::PVK_3:
            return kVK_ANSI_3;
            break;
        case portableVKey::PVK_4:
            return kVK_ANSI_4;
            break;
        case portableVKey::PVK_6:
            return kVK_ANSI_6;
            break;
        case portableVKey::PVK_5:
            return kVK_ANSI_5;
            break;
        case portableVKey::PVK_EQUAL:
    return kVK_ANSI_Equal;
    break;
        case portableVKey::PVK_9:
            return kVK_ANSI_9;
            break;
        case portableVKey::PVK_7:
            return kVK_ANSI_7;
            break;
        case portableVKey::PVK_MINUS:
            return kVK_ANSI_Minus;
            break;
        case portableVKey::PVK_8:
            return kVK_ANSI_8;
            break;
        case portableVKey::PVK_0:
            return kVK_ANSI_0;
            break;
        case portableVKey::PVK_RIGHT_BRACKET:
    return kVK_ANSI_RightBracket;
    break;
        case portableVKey::PVK_O:
            return kVK_ANSI_O;
            break;
        case portableVKey::PVK_U:
            return kVK_ANSI_U;
            break;
        case portableVKey::PVK_LEFT_BRACKET:
    return kVK_ANSI_LeftBracket;
    break;
        case portableVKey::PVK_I:
            return kVK_ANSI_I;
            break;
        case portableVKey::PVK_P:
            return kVK_ANSI_P;
            break;
        case portableVKey::PVK_L:
            return kVK_ANSI_L;
            break;
        case portableVKey::PVK_J:
            return kVK_ANSI_J;
            break;
        case portableVKey::PVK_QUOTE:
            return kVK_ANSI_Quote;
            break;
        case portableVKey::PVK_K:
            return kVK_ANSI_K;
            break;
        case portableVKey::PVK_SHIFT:
            return kVK_Shift;
            break;
        case portableVKey::PVK_CONTROL:
            return kVK_Command;
            break;
        case portableVKey::PVK_ALT:
            return kVK_Option;
            break;
        case portableVKey::PVK_SEMICOLON:
    return kVK_ANSI_Semicolon;
    break;
        case portableVKey::PVK_BACKSLASH:
            return kVK_ANSI_Backslash;
            break;
        case portableVKey::PVK_COMMA:
            return kVK_ANSI_Comma;
            break;
        case portableVKey::PVK_SLASH:
            return kVK_ANSI_Slash;
            break;
        case portableVKey::PVK_N:
            return kVK_ANSI_N;
            break;
        case portableVKey::PVK_M:
            return kVK_ANSI_M;
            break;
        case portableVKey::PVK_PERIOD:
            return kVK_ANSI_Period;
            break;
        case portableVKey::PVK_GRAVE:
            return kVK_ANSI_Grave;
            break;

        case portableVKey::PVK_DECIMAL:
        return kVK_ANSI_KeypadDecimal;
        break;
        case portableVKey::PVK_MULTIPLY:
        return kVK_ANSI_KeypadMultiply;
        break;
        case portableVKey::PVK_ADD:
        return kVK_ANSI_KeypadPlus;
        break;
        //case kVK_ANSI_KeypadClear:
        //return portableVKey::PVK_KEYPAD_CLEAR;
        //break;
        case portableVKey::PVK_KEYPAD_DIVIDE:
        return kVK_ANSI_KeypadDivide;
        break;
        //case kVK_ANSI_KeypadEnter:
        //return portableVKey::PVK_KEYPAD_ENTER;
        //break;
        case portableVKey::PVK_KEYPAD_MINUS:
        return kVK_ANSI_KeypadMinus;
        break;
        //case kVK_ANSI_KeypadEquals:
        //return portableVKey::PVK_KEYPAD_EQUALS;
        //break;

        case portableVKey::PVK_NUMPAD0:
            return kVK_ANSI_Keypad0;
            break;
        case portableVKey::PVK_NUMPAD1:
            return kVK_ANSI_Keypad1;
            break;
        case portableVKey::PVK_NUMPAD2:
            return kVK_ANSI_Keypad2;
            break;
        case portableVKey::PVK_NUMPAD3:
            return kVK_ANSI_Keypad3;
            break;
        case portableVKey::PVK_NUMPAD4:
            return kVK_ANSI_Keypad4;
            break;
        case portableVKey::PVK_NUMPAD5:
            return kVK_ANSI_Keypad5;
            break;
        case portableVKey::PVK_NUMPAD6:
            return kVK_ANSI_Keypad6;
            break;
        case portableVKey::PVK_NUMPAD7:
            return kVK_ANSI_Keypad7;
            break;
        case portableVKey::PVK_NUMPAD8:
            return kVK_ANSI_Keypad8;
            break;
        case portableVKey::PVK_NUMPAD9:
            return kVK_ANSI_Keypad9;
            break;

        case portableVKey::PVK_RETURN:
            return kVK_Return;
            break;
        case portableVKey::PVK_TAB:
            return kVK_Tab;
            break;
        case portableVKey::PVK_SPACE:
            return kVK_Space;
            break;
        case portableVKey::PVK_BACK:
            return kVK_Delete;
            break;
        case portableVKey::PVK_ESCAPE:
            return kVK_Escape;
            break;
        case portableVKey::PVK_CAPSLOCK:
            return kVK_CapsLock;
            break;
        case portableVKey::PVK_F17:
            return kVK_F17;
            break;
        case portableVKey::PVK_VOLUME_UP:
            return kVK_VolumeUp;
            break;
        case portableVKey::PVK_VOLUME_DOWN:
            return kVK_VolumeDown;
            break;
        case portableVKey::PVK_VOLUME_MUTE:
            return kVK_Mute;
            break;
        case portableVKey::PVK_F18:
            return kVK_F18;
            break;
        case portableVKey::PVK_F19:
            return kVK_F19;
            break;
        case portableVKey::PVK_F20:
            return kVK_F20;
            break;
        case portableVKey::PVK_F5:
            return kVK_F5;
            break;
        case portableVKey::PVK_F6:
            return kVK_F6;
            break;
        case portableVKey::PVK_F7:
            return kVK_F7;
            break;
        case portableVKey::PVK_F3:
            return kVK_F3;
            break;
        case portableVKey::PVK_F8:
            return kVK_F8;
            break;
        case portableVKey::PVK_F9:
            return kVK_F9;
            break;
        case portableVKey::PVK_F11:
            return kVK_F11;
            break;
        case portableVKey::PVK_F13:
            return kVK_F13;
            break;
        case portableVKey::PVK_F16:
            return kVK_F16;
            break;
        case portableVKey::PVK_F14:
            return kVK_F14;
            break;
        case portableVKey::PVK_F10:
            return kVK_F10;
            break;
        case portableVKey::PVK_F12:
            return kVK_F12;
            break;
        case portableVKey::PVK_F15:
            return kVK_F15;
            break;
        case portableVKey::PVK_HELP:
            return kVK_Help;
            break;
        case portableVKey::PVK_HOME:
            return kVK_Home;
            break;
        case portableVKey::PVK_PAGE_UP:
            return kVK_PageUp;
            break;
        case portableVKey::PVK_DELETE:
            return kVK_ForwardDelete; // ?
            break;
        case portableVKey::PVK_F4:
            return kVK_F4;
            break;
        case portableVKey::PVK_END:
            return kVK_End;
            break;
        case portableVKey::PVK_F2:
            return kVK_F2;
            break;
        case portableVKey::PVK_PAGE_DOWN:
            return kVK_PageDown;
            break;
        case  portableVKey::PVK_F1:
            return kVK_F1;
            break;
        case portableVKey::PVK_LEFT:
            return kVK_LeftArrow;
            break;
        case portableVKey::PVK_RIGHT:
            return kVK_RightArrow;
            break;
        case portableVKey::PVK_DOWN:
            return kVK_DownArrow;
            break;
        case portableVKey::PVK_UP:
            return kVK_UpArrow;
            break;
        case portableVKey::PVK_LWIN:
        case portableVKey::PVK_RWIN:
            return kVK_Control;
            break;
        }

        return -1; // den vrethike annalogo key
}

//https://qt.gitorious.org/qt/qt/source/61570cd378e1b5a257ad90efcdac4098cb85262c:src/gui/kernel/qkeymapper_win.cpp
// Key translation ---------------------------------------------------------------------[ start ] --
// Meaning of values:
//             0 = Character output key, needs keyboard driver mapping
//   Key_unknown = Unknown Virtual Key, no translation possible, ignore
static const uint KeyTbl[] = { // Keyboard mapping table
                        // Dec |  Hex | Windows Virtual key
    Qt::Key_unknown,    //   0   0x00
    Qt::Key_unknown,    //   1   0x01   VK_LBUTTON          | Left mouse button
    Qt::Key_unknown,    //   2   0x02   VK_RBUTTON          | Right mouse button
    Qt::Key_Cancel,     //   3   0x03   VK_CANCEL           | Control-Break processing
    Qt::Key_unknown,    //   4   0x04   VK_MBUTTON          | Middle mouse button
    Qt::Key_unknown,    //   5   0x05   VK_XBUTTON1         | X1 mouse button
    Qt::Key_unknown,    //   6   0x06   VK_XBUTTON2         | X2 mouse button
    Qt::Key_unknown,    //   7   0x07   -- unassigned --
    Qt::Key_Backspace,  //   8   0x08   VK_BACK             | BackSpace key
    Qt::Key_Tab,        //   9   0x09   VK_TAB              | Tab key
    Qt::Key_unknown,    //  10   0x0A   -- reserved --
    Qt::Key_unknown,    //  11   0x0B   -- reserved --
    Qt::Key_Clear,      //  12   0x0C   VK_CLEAR            | Clear key
    Qt::Key_Return,     //  13   0x0D   VK_RETURN           | Enter key
    Qt::Key_unknown,    //  14   0x0E   -- unassigned --
    Qt::Key_unknown,    //  15   0x0F   -- unassigned --
    Qt::Key_Shift,      //  16   0x10   VK_SHIFT            | Shift key
    Qt::Key_Control,    //  17   0x11   VK_CONTROL          | Ctrl key
    Qt::Key_Alt,        //  18   0x12   VK_MENU             | Alt key
    Qt::Key_Pause,      //  19   0x13   VK_PAUSE            | Pause key
    Qt::Key_CapsLock,   //  20   0x14   VK_CAPITAL          | Caps-Lock
    Qt::Key_unknown,    //  21   0x15   VK_KANA / VK_HANGUL | IME Kana or Hangul mode
    Qt::Key_unknown,    //  22   0x16   -- unassigned --
    Qt::Key_unknown,    //  23   0x17   VK_JUNJA            | IME Junja mode
    Qt::Key_unknown,    //  24   0x18   VK_FINAL            | IME final mode
    Qt::Key_unknown,    //  25   0x19   VK_HANJA / VK_KANJI | IME Hanja or Kanji mode
    Qt::Key_unknown,    //  26   0x1A   -- unassigned --
    Qt::Key_Escape,     //  27   0x1B   VK_ESCAPE           | Esc key
    Qt::Key_unknown,    //  28   0x1C   VK_CONVERT          | IME convert
    Qt::Key_unknown,    //  29   0x1D   VK_NONCONVERT       | IME non-convert
    Qt::Key_unknown,    //  30   0x1E   VK_ACCEPT           | IME accept
    Qt::Key_Mode_switch,//  31   0x1F   VK_MODECHANGE       | IME mode change request
    Qt::Key_Space,      //  32   0x20   VK_SPACE            | Spacebar
    Qt::Key_PageUp,     //  33   0x21   VK_PRIOR            | Page Up key
    Qt::Key_PageDown,   //  34   0x22   VK_NEXT             | Page Down key
    Qt::Key_End,        //  35   0x23   VK_END              | End key
    Qt::Key_Home,       //  36   0x24   VK_HOME             | Home key
    Qt::Key_Left,       //  37   0x25   VK_LEFT             | Left arrow key
    Qt::Key_Up,         //  38   0x26   VK_UP               | Up arrow key
    Qt::Key_Right,      //  39   0x27   VK_RIGHT            | Right arrow key
    Qt::Key_Down,       //  40   0x28   VK_DOWN             | Down arrow key
    Qt::Key_Select,     //  41   0x29   VK_SELECT           | Select key
    Qt::Key_Printer,    //  42   0x2A   VK_PRINT            | Print key
    Qt::Key_Execute,    //  43   0x2B   VK_EXECUTE          | Execute key
    Qt::Key_Print,      //  44   0x2C   VK_SNAPSHOT         | Print Screen key
    Qt::Key_Insert,     //  45   0x2D   VK_INSERT           | Ins key
    Qt::Key_Delete,     //  46   0x2E   VK_DELETE           | Del key
    Qt::Key_Help,       //  47   0x2F   VK_HELP             | Help key
    0,                  //  48   0x30   (VK_0)              | 0 key
    0,                  //  49   0x31   (VK_1)              | 1 key
    0,                  //  50   0x32   (VK_2)              | 2 key
    0,                  //  51   0x33   (VK_3)              | 3 key
    0,                  //  52   0x34   (VK_4)              | 4 key
    0,                  //  53   0x35   (VK_5)              | 5 key
    0,                  //  54   0x36   (VK_6)              | 6 key
    0,                  //  55   0x37   (VK_7)              | 7 key
    0,                  //  56   0x38   (VK_8)              | 8 key
    0,                  //  57   0x39   (VK_9)              | 9 key
    Qt::Key_unknown,    //  58   0x3A   -- unassigned --
    Qt::Key_unknown,    //  59   0x3B   -- unassigned --
    Qt::Key_unknown,    //  60   0x3C   -- unassigned --
    Qt::Key_unknown,    //  61   0x3D   -- unassigned --
    Qt::Key_unknown,    //  62   0x3E   -- unassigned --
    Qt::Key_unknown,    //  63   0x3F   -- unassigned --
    Qt::Key_unknown,    //  64   0x40   -- unassigned --
    0,                  //  65   0x41   (VK_A)              | A key
    0,                  //  66   0x42   (VK_B)              | B key
    0,                  //  67   0x43   (VK_C)              | C key
    0,                  //  68   0x44   (VK_D)              | D key
    0,                  //  69   0x45   (VK_E)              | E key
    0,                  //  70   0x46   (VK_F)              | F key
    0,                  //  71   0x47   (VK_G)              | G key
    0,                  //  72   0x48   (VK_H)              | H key
    0,                  //  73   0x49   (VK_I)              | I key
    0,                  //  74   0x4A   (VK_J)              | J key
    0,                  //  75   0x4B   (VK_K)              | K key
    0,                  //  76   0x4C   (VK_L)              | L key
    0,                  //  77   0x4D   (VK_M)              | M key
    0,                  //  78   0x4E   (VK_N)              | N key
    0,                  //  79   0x4F   (VK_O)              | O key
    0,                  //  80   0x50   (VK_P)              | P key
    0,                  //  81   0x51   (VK_Q)              | Q key
    0,                  //  82   0x52   (VK_R)              | R key
    0,                  //  83   0x53   (VK_S)              | S key
    0,                  //  84   0x54   (VK_T)              | T key
    0,                  //  85   0x55   (VK_U)              | U key
    0,                  //  86   0x56   (VK_V)              | V key
    0,                  //  87   0x57   (VK_W)              | W key
    0,                  //  88   0x58   (VK_X)              | X key
    0,                  //  89   0x59   (VK_Y)              | Y key
    0,                  //  90   0x5A   (VK_Z)              | Z key
    Qt::Key_Meta,       //  91   0x5B   VK_LWIN             | Left Windows  - MS Natural kbd
    Qt::Key_Meta,       //  92   0x5C   VK_RWIN             | Right Windows - MS Natural kbd
    Qt::Key_Menu,       //  93   0x5D   VK_APPS             | Application key-MS Natural kbd
    Qt::Key_unknown,    //  94   0x5E   -- reserved --
    Qt::Key_Sleep,      //  95   0x5F   VK_SLEEP
    Qt::Key_0,          //  96   0x60   VK_NUMPAD0          | Numeric keypad 0 key
    Qt::Key_1,          //  97   0x61   VK_NUMPAD1          | Numeric keypad 1 key
    Qt::Key_2,          //  98   0x62   VK_NUMPAD2          | Numeric keypad 2 key
    Qt::Key_3,          //  99   0x63   VK_NUMPAD3          | Numeric keypad 3 key
    Qt::Key_4,          // 100   0x64   VK_NUMPAD4          | Numeric keypad 4 key
    Qt::Key_5,          // 101   0x65   VK_NUMPAD5          | Numeric keypad 5 key
    Qt::Key_6,          // 102   0x66   VK_NUMPAD6          | Numeric keypad 6 key
    Qt::Key_7,          // 103   0x67   VK_NUMPAD7          | Numeric keypad 7 key
    Qt::Key_8,          // 104   0x68   VK_NUMPAD8          | Numeric keypad 8 key
    Qt::Key_9,          // 105   0x69   VK_NUMPAD9          | Numeric keypad 9 key
    Qt::Key_Asterisk,   // 106   0x6A   VK_MULTIPLY         | Multiply key
    Qt::Key_Plus,       // 107   0x6B   VK_ADD              | Add key
    Qt::Key_Comma,      // 108   0x6C   VK_SEPARATOR        | Separator key
    Qt::Key_Minus,      // 109   0x6D   VK_SUBTRACT         | Subtract key
    Qt::Key_Period,     // 110   0x6E   VK_DECIMAL          | Decimal key
    Qt::Key_Slash,      // 111   0x6F   VK_DIVIDE           | Divide key
    Qt::Key_F1,         // 112   0x70   VK_F1               | F1 key
    Qt::Key_F2,         // 113   0x71   VK_F2               | F2 key
    Qt::Key_F3,         // 114   0x72   VK_F3               | F3 key
    Qt::Key_F4,         // 115   0x73   VK_F4               | F4 key
    Qt::Key_F5,         // 116   0x74   VK_F5               | F5 key
    Qt::Key_F6,         // 117   0x75   VK_F6               | F6 key
    Qt::Key_F7,         // 118   0x76   VK_F7               | F7 key
    Qt::Key_F8,         // 119   0x77   VK_F8               | F8 key
    Qt::Key_F9,         // 120   0x78   VK_F9               | F9 key
    Qt::Key_F10,        // 121   0x79   VK_F10              | F10 key
    Qt::Key_F11,        // 122   0x7A   VK_F11              | F11 key
    Qt::Key_F12,        // 123   0x7B   VK_F12              | F12 key
    Qt::Key_F13,        // 124   0x7C   VK_F13              | F13 key
    Qt::Key_F14,        // 125   0x7D   VK_F14              | F14 key
    Qt::Key_F15,        // 126   0x7E   VK_F15              | F15 key
    Qt::Key_F16,        // 127   0x7F   VK_F16              | F16 key
    Qt::Key_F17,        // 128   0x80   VK_F17              | F17 key
    Qt::Key_F18,        // 129   0x81   VK_F18              | F18 key
    Qt::Key_F19,        // 130   0x82   VK_F19              | F19 key
    Qt::Key_F20,        // 131   0x83   VK_F20              | F20 key
    Qt::Key_F21,        // 132   0x84   VK_F21              | F21 key
    Qt::Key_F22,        // 133   0x85   VK_F22              | F22 key
    Qt::Key_F23,        // 134   0x86   VK_F23              | F23 key
    Qt::Key_F24,        // 135   0x87   VK_F24              | F24 key
    Qt::Key_unknown,    // 136   0x88   -- unassigned --
    Qt::Key_unknown,    // 137   0x89   -- unassigned --
    Qt::Key_unknown,    // 138   0x8A   -- unassigned --
    Qt::Key_unknown,    // 139   0x8B   -- unassigned --
    Qt::Key_unknown,    // 140   0x8C   -- unassigned --
    Qt::Key_unknown,    // 141   0x8D   -- unassigned --
    Qt::Key_unknown,    // 142   0x8E   -- unassigned --
    Qt::Key_unknown,    // 143   0x8F   -- unassigned --
    Qt::Key_NumLock,    // 144   0x90   VK_NUMLOCK          | Num Lock key
    Qt::Key_ScrollLock, // 145   0x91   VK_SCROLL           | Scroll Lock key
                        // Fujitsu/OASYS kbd --------------------
    0, //Qt::Key_Jisho, // 146   0x92   VK_OEM_FJ_JISHO     | 'Dictionary' key /
                        //              VK_OEM_NEC_EQUAL  = key on numpad on NEC PC-9800 kbd
    Qt::Key_Massyo,     // 147   0x93   VK_OEM_FJ_MASSHOU   | 'Unregister word' key
    Qt::Key_Touroku,    // 148   0x94   VK_OEM_FJ_TOUROKU   | 'Register word' key
    0, //Qt::Key_Oyayubi_Left,//149   0x95  VK_OEM_FJ_LOYA  | 'Left OYAYUBI' key
    0, //Qt::Key_Oyayubi_Right,//150  0x96  VK_OEM_FJ_ROYA  | 'Right OYAYUBI' key
    Qt::Key_unknown,    // 151   0x97   -- unassigned --
    Qt::Key_unknown,    // 152   0x98   -- unassigned --
    Qt::Key_unknown,    // 153   0x99   -- unassigned --
    Qt::Key_unknown,    // 154   0x9A   -- unassigned --
    Qt::Key_unknown,    // 155   0x9B   -- unassigned --
    Qt::Key_unknown,    // 156   0x9C   -- unassigned --
    Qt::Key_unknown,    // 157   0x9D   -- unassigned --
    Qt::Key_unknown,    // 158   0x9E   -- unassigned --
    Qt::Key_unknown,    // 159   0x9F   -- unassigned --
    Qt::Key_Shift,      // 160   0xA0   VK_LSHIFT           | Left Shift key
    Qt::Key_Shift,      // 161   0xA1   VK_RSHIFT           | Right Shift key
    Qt::Key_Control,    // 162   0xA2   VK_LCONTROL         | Left Ctrl key
    Qt::Key_Control,    // 163   0xA3   VK_RCONTROL         | Right Ctrl key
    Qt::Key_Alt,        // 164   0xA4   VK_LMENU            | Left Menu key
    Qt::Key_Alt,        // 165   0xA5   VK_RMENU            | Right Menu key
    Qt::Key_Back,       // 166   0xA6   VK_BROWSER_BACK     | Browser Back key
    Qt::Key_Forward,    // 167   0xA7   VK_BROWSER_FORWARD  | Browser Forward key
    Qt::Key_Refresh,    // 168   0xA8   VK_BROWSER_REFRESH  | Browser Refresh key
    Qt::Key_Stop,       // 169   0xA9   VK_BROWSER_STOP     | Browser Stop key
    Qt::Key_Search,     // 170   0xAA   VK_BROWSER_SEARCH   | Browser Search key
    Qt::Key_Favorites,  // 171   0xAB   VK_BROWSER_FAVORITES| Browser Favorites key
    Qt::Key_HomePage,   // 172   0xAC   VK_BROWSER_HOME     | Browser Start and Home key
    Qt::Key_VolumeMute, // 173   0xAD   VK_VOLUME_MUTE      | Volume Mute key
    Qt::Key_VolumeDown, // 174   0xAE   VK_VOLUME_DOWN      | Volume Down key
    Qt::Key_VolumeUp,   // 175   0xAF   VK_VOLUME_UP        | Volume Up key
    Qt::Key_MediaNext,  // 176   0xB0   VK_MEDIA_NEXT_TRACK | Next Track key
    Qt::Key_MediaPrevious, //177 0xB1   VK_MEDIA_PREV_TRACK | Previous Track key
    Qt::Key_MediaStop,  // 178   0xB2   VK_MEDIA_STOP       | Stop Media key
    Qt::Key_MediaPlay,  // 179   0xB3   VK_MEDIA_PLAY_PAUSE | Play/Pause Media key
    Qt::Key_LaunchMail, // 180   0xB4   VK_LAUNCH_MAIL      | Start Mail key
    Qt::Key_LaunchMedia,// 181   0xB5   VK_LAUNCH_MEDIA_SELECT Select Media key
    Qt::Key_Launch0,    // 182   0xB6   VK_LAUNCH_APP1      | Start Application 1 key
    Qt::Key_Launch1,    // 183   0xB7   VK_LAUNCH_APP2      | Start Application 2 key
    Qt::Key_unknown,    // 184   0xB8   -- reserved --
    Qt::Key_unknown,    // 185   0xB9   -- reserved --
    0,                  // 186   0xBA   VK_OEM_1            | ';:' for US
    0,                  // 187   0xBB   VK_OEM_PLUS         | '+' any country
    0,                  // 188   0xBC   VK_OEM_COMMA        | ',' any country
    0,                  // 189   0xBD   VK_OEM_MINUS        | '-' any country
    0,                  // 190   0xBE   VK_OEM_PERIOD       | '.' any country
    0,                  // 191   0xBF   VK_OEM_2            | '/?' for US
    0,                  // 192   0xC0   VK_OEM_3            | '`~' for US
    Qt::Key_unknown,    // 193   0xC1   -- reserved --
    Qt::Key_unknown,    // 194   0xC2   -- reserved --
    Qt::Key_unknown,    // 195   0xC3   -- reserved --
    Qt::Key_unknown,    // 196   0xC4   -- reserved --
    Qt::Key_unknown,    // 197   0xC5   -- reserved --
    Qt::Key_unknown,    // 198   0xC6   -- reserved --
    Qt::Key_unknown,    // 199   0xC7   -- reserved --
    Qt::Key_unknown,    // 200   0xC8   -- reserved --
    Qt::Key_unknown,    // 201   0xC9   -- reserved --
    Qt::Key_unknown,    // 202   0xCA   -- reserved --
    Qt::Key_unknown,    // 203   0xCB   -- reserved --
    Qt::Key_unknown,    // 204   0xCC   -- reserved --
    Qt::Key_unknown,    // 205   0xCD   -- reserved --
    Qt::Key_unknown,    // 206   0xCE   -- reserved --
    Qt::Key_unknown,    // 207   0xCF   -- reserved --
    Qt::Key_unknown,    // 208   0xD0   -- reserved --
    Qt::Key_unknown,    // 209   0xD1   -- reserved --
    Qt::Key_unknown,    // 210   0xD2   -- reserved --
    Qt::Key_unknown,    // 211   0xD3   -- reserved --
    Qt::Key_unknown,    // 212   0xD4   -- reserved --
    Qt::Key_unknown,    // 213   0xD5   -- reserved --
    Qt::Key_unknown,    // 214   0xD6   -- reserved --
    Qt::Key_unknown,    // 215   0xD7   -- reserved --
    Qt::Key_unknown,    // 216   0xD8   -- unassigned --
    Qt::Key_unknown,    // 217   0xD9   -- unassigned --
    Qt::Key_unknown,    // 218   0xDA   -- unassigned --
    0,                  // 219   0xDB   VK_OEM_4            | '[{' for US
    0,                  // 220   0xDC   VK_OEM_5            | '\|' for US
    0,                  // 221   0xDD   VK_OEM_6            | ']}' for US
    0,                  // 222   0xDE   VK_OEM_7            | ''"' for US
    0,                  // 223   0xDF   VK_OEM_8
    Qt::Key_unknown,    // 224   0xE0   -- reserved --
    Qt::Key_unknown,    // 225   0xE1   VK_OEM_AX           | 'AX' key on Japanese AX kbd
    Qt::Key_unknown,    // 226   0xE2   VK_OEM_102          | "<>" or "\|" on RT 102-key kbd
    Qt::Key_unknown,    // 227   0xE3   VK_ICO_HELP         | Help key on ICO
    Qt::Key_unknown,    // 228   0xE4   VK_ICO_00           | 00 key on ICO
    Qt::Key_unknown,    // 229   0xE5   VK_PROCESSKEY       | IME Process key
    Qt::Key_unknown,    // 230   0xE6   VK_ICO_CLEAR        |
    Qt::Key_unknown,    // 231   0xE7   VK_PACKET           | Unicode char as keystrokes
    Qt::Key_unknown,    // 232   0xE8   -- unassigned --
                        // Nokia/Ericsson definitions ---------------
    Qt::Key_unknown,    // 233   0xE9   VK_OEM_RESET
    Qt::Key_unknown,    // 234   0xEA   VK_OEM_JUMP
    Qt::Key_unknown,    // 235   0xEB   VK_OEM_PA1
    Qt::Key_unknown,    // 236   0xEC   VK_OEM_PA2
    Qt::Key_unknown,    // 237   0xED   VK_OEM_PA3
    Qt::Key_unknown,    // 238   0xEE   VK_OEM_WSCTRL
    Qt::Key_unknown,    // 239   0xEF   VK_OEM_CUSEL
    Qt::Key_unknown,    // 240   0xF0   VK_OEM_ATTN
    Qt::Key_unknown,    // 241   0xF1   VK_OEM_FINISH
    Qt::Key_unknown,    // 242   0xF2   VK_OEM_COPY
    Qt::Key_unknown,    // 243   0xF3   VK_OEM_AUTO
    Qt::Key_unknown,    // 244   0xF4   VK_OEM_ENLW
    Qt::Key_unknown,    // 245   0xF5   VK_OEM_BACKTAB
    Qt::Key_unknown,    // 246   0xF6   VK_ATTN             | Attn key
    Qt::Key_unknown,    // 247   0xF7   VK_CRSEL            | CrSel key
    Qt::Key_unknown,    // 248   0xF8   VK_EXSEL            | ExSel key
    Qt::Key_unknown,    // 249   0xF9   VK_EREOF            | Erase EOF key
    Qt::Key_Play,       // 250   0xFA   VK_PLAY             | Play key
    Qt::Key_Zoom,       // 251   0xFB   VK_ZOOM             | Zoom key
    Qt::Key_unknown,    // 252   0xFC   VK_NONAME           | Reserved
    Qt::Key_unknown,    // 253   0xFD   VK_PA1              | PA1 key
    Qt::Key_Clear,      // 254   0xFE   VK_OEM_CLEAR        | Clear key
    0
};


#endif
