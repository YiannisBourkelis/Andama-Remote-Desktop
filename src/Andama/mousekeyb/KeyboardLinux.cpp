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

#ifdef Q_OS_LINUX
#define XK_LATIN1;
#define XK_MISCELLANY;
#include <X11/keysymdef.h>
#include <X11/extensions/XTest.h>

void Keyboard::keyPress(int key, int modifiers)
{
    //XK_BackSpace
    QByteArray display_name = qgetenv("DISPLAY");
    Display *display = XOpenDisplay(display_name.constData());
    XTestFakeKeyEvent(display, XKeysymToKeycode(display, key), 1, 0);
    XFlush(display);
    XCloseDisplay(display);
}

void Keyboard::keyRelease(int key, int modifiers)
{
    QByteArray display_name = qgetenv("DISPLAY");
    Display *display = XOpenDisplay(display_name.constData());
    XTestFakeKeyEvent(display, XKeysymToKeycode(display, key), 0, 0);
    XFlush(display);
    XCloseDisplay(display);
}

portableVKey Keyboard::getPortableVKey(int key, int qtkey)
{
    switch (key)
    {
    case XK_equal:
return portableVKey::PVK_EQUAL;
break;
    case XK_minus:
        return portableVKey::PVK_MINUS;
        break;

    case XK_bracketright:
return portableVKey::PVK_RIGHT_BRACKET;
break;
    case XK_bracketleft:
return portableVKey::PVK_LEFT_BRACKET;
break;
    case XK_quotedbl:
        return portableVKey::PVK_QUOTE;
        break;
    case XK_Shift_L:
        return portableVKey::PVK_SHIFT;
        break;
    case XK_Control_L:
        return portableVKey::PVK_CONTROL;
        break;
    case XK_Alt_L:
        return  portableVKey::PVK_ALT;
        break;
    case XK_semicolon:
return portableVKey::PVK_SEMICOLON;
break;
    case XK_backslash:
        return portableVKey::PVK_BACKSLASH;
        break;
    case XK_comma:
        return portableVKey::PVK_COMMA;
        break;
    case XK_slash:
        return portableVKey::PVK_SLASH;
        break;
    case XK_period:
        return portableVKey::PVK_PERIOD;
        break;
    case XK_grave:
        return portableVKey::PVK_GRAVE;
        break;

    case XK_KP_Decimal:
    return portableVKey::PVK_DECIMAL;
    break;
    case XK_KP_Multiply:
    return portableVKey::PVK_MULTIPLY;
    break;
    case XK_KP_Add:
    return portableVKey::PVK_ADD;
    break;
    //case kVK_ANSI_KeypadClear:
    //return portableVKey::PVK_KEYPAD_CLEAR;
    //break;
    case XK_KP_Divide:
    return portableVKey::PVK_KEYPAD_DIVIDE;
    break;
    //case kVK_ANSI_KeypadEnter:
    //return portableVKey::PVK_KEYPAD_ENTER;
    //break;
    case XK_KP_Subtract:
    return portableVKey::PVK_KEYPAD_MINUS;
    break;
    //case kVK_ANSI_KeypadEquals:
    //return portableVKey::PVK_KEYPAD_EQUALS;
    //break;

    case XK_KP_0:
        return portableVKey::PVK_NUMPAD0;
        break;
    case XK_KP_1:
        return portableVKey::PVK_NUMPAD1;
        break;
    case XK_KP_2:
        return portableVKey::PVK_NUMPAD2;
        break;
    case XK_KP_3:
        return portableVKey::PVK_NUMPAD3;
        break;
    case XK_KP_4:
        return portableVKey::PVK_NUMPAD4;
        break;
    case XK_KP_5:
        return portableVKey::PVK_NUMPAD5;
        break;
    case XK_KP_6:
        return portableVKey::PVK_NUMPAD6;
        break;
    case XK_KP_7:
        return portableVKey::PVK_NUMPAD7;
        break;
    case XK_KP_8:
        return portableVKey::PVK_NUMPAD8;
        break;
    case XK_KP_9:
        return portableVKey::PVK_NUMPAD9;
        break;

    case XK_Return:
        return portableVKey::PVK_RETURN;
        break;
    case XK_Tab:
        return portableVKey::PVK_TAB;
        break;
    case XK_Linefeed
        return portableVKey::PVK_SPACE;
        break;
    case XK_BackSpace:
        return portableVKey::PVK_BACK;
        break;
    case XK_Escape:
        return portableVKey::PVK_ESCAPE;
        break;
    case XK_Caps_Lock:
        return portableVKey::PVK_CAPSLOCK;
        break;
    case XK_F17:
        return portableVKey::PVK_F17;
        break;
    //case portableVKey::PVK_VOLUME_UP:
    //    return kVK_VolumeUp;
    //    break;
    //case portableVKey::PVK_VOLUME_DOWN:
    //    return kVK_VolumeDown;
    //    break;
    //case portableVKey::PVK_VOLUME_MUTE:
    //    return kVK_Mute;
    //    break;
    case XK_F18:
        return portableVKey::PVK_F18;
        break;
    case XK_F19:
        return portableVKey::PVK_F19;
        break;
    case XK_F20:
        return portableVKey::PVK_F20;
        break;
    case XK_F5:
        return portableVKey::PVK_F5;
        break;
    case XK_F6:
        return portableVKey::PVK_F6;
        break;
    case XK_F7:
         return portableVKey::PVK_F7;
        break;
    case XK_F3:
        return portableVKey::PVK_F3;
        break;
    case XK_F8:
        return portableVKey::PVK_F8;
        break;
    case XK_F9:
        return portableVKey::PVK_F9;
        break;
    case XK_F11:
        return portableVKey::PVK_F11;
        break;
    case XK_F13:
        return portableVKey::PVK_F13;
        break;
    case XK_F16:
        return portableVKey::PVK_F16;
        break;
    case XK_F14:
        return portableVKey::PVK_F14;
        break;
    case XK_F10:
        return portableVKey::PVK_F10;
        break;
    case XK_F12:
        return portableVKey::PVK_F12;
        break;
    case XK_F15:
        return portableVKey::PVK_F15;
        break;
    case XK_Help:
        return portableVKey::PVK_HELP;
        break;
    case XK_Home:
        return portableVKey::PVK_HOME;
        break;
    case XK_Page_Up:
        return portableVKey::PVK_PAGE_UP;
        break;
    case XK_Delete:
        return portableVKey::PVK_DELETE; // ?
        break;
    case XK_F4:
        return portableVKey::PVK_F4;
        break;
    case XK_End:
        return portableVKey::PVK_END;
        break;
    case XK_F2:
        return portableVKey::PVK_F2;
        break;
    case XK_Page_Down:
        return portableVKey::PVK_PAGE_DOWN;
        break;
    case  XK_F1:
        return portableVKey::PVK_F1;
        break;
    case XK_Left:
        return portableVKey::PVK_LEFT;
        break;
    case XK_Right:
        return portableVKey::PVK_RIGHT;
        break;
    case XK_Down:
        return portableVKey::PVK_DOWN;
        break;
    case XK_Up:
        return  portableVKey::PVK_UP;
        break;
    case XK_Num_Lock:
    return portableVKey::PVK_NUMLOCK; //TODO: den paizei
    break;
    case XK_Insert:
        return portableVKey::PVK_INSERT;
        break;
    //case portableVKey::PVK_LWIN:
    //case portableVKey::PVK_RWIN:
    //    return kVK_Control;
    //    break;
    }

    //0-9, A-Z
    if (key >= 0x30 && key <= 0x5A){
        return (portableVKey) key;
    }

    return PVK_UNKNOWN; // den vrethike annalogo key

}

int Keyboard::convertPortableKeyToLocal(portableVKey portableKey)
{
        switch (portableKey)
        {
        case portableVKey::PVK_EQUAL:
    return XK_equal;
    break;
        case portableVKey::PVK_MINUS:
            return XK_minus;
            break;

        case portableVKey::PVK_RIGHT_BRACKET:
    return XK_bracketright;
    break;
        case portableVKey::PVK_LEFT_BRACKET:
    return XK_bracketleft;
    break;
        case portableVKey::PVK_QUOTE:
            return XK_quotedbl;
            break;
        case portableVKey::PVK_SHIFT:
            return XK_Shift_L;
            break;
        case portableVKey::PVK_CONTROL:
            return XK_Control_L;
            break;
        case portableVKey::PVK_ALT:
            return XK_Alt_L;
            break;
        case portableVKey::PVK_SEMICOLON:
    return XK_semicolon;
    break;
        case portableVKey::PVK_BACKSLASH:
            return XK_backslash;
            break;
        case portableVKey::PVK_COMMA:
            return XK_comma;
            break;
        case portableVKey::PVK_SLASH:
            return XK_slash;
            break;
        case portableVKey::PVK_PERIOD:
            return XK_period;
            break;
        case portableVKey::PVK_GRAVE:
            return XK_grave;
            break;

        case portableVKey::PVK_DECIMAL:
        return XK_KP_Decimal;
        break;
        case portableVKey::PVK_MULTIPLY:
        return XK_KP_Multiply;
        break;
        case portableVKey::PVK_ADD:
        return XK_KP_Add;
        break;
        //case kVK_ANSI_KeypadClear:
        //return portableVKey::PVK_KEYPAD_CLEAR;
        //break;
        case portableVKey::PVK_KEYPAD_DIVIDE:
        return XK_KP_Divide;
        break;
        //case kVK_ANSI_KeypadEnter:
        //return portableVKey::PVK_KEYPAD_ENTER;
        //break;
        case portableVKey::PVK_KEYPAD_MINUS:
        return XK_KP_Subtract;
        break;
        //case kVK_ANSI_KeypadEquals:
        //return portableVKey::PVK_KEYPAD_EQUALS;
        //break;

        case portableVKey::PVK_NUMPAD0:
            return XK_KP_0;
            break;
        case portableVKey::PVK_NUMPAD1:
            return XK_KP_1;
            break;
        case portableVKey::PVK_NUMPAD2:
            return XK_KP_2;
            break;
        case portableVKey::PVK_NUMPAD3:
            return XK_KP_3;
            break;
        case portableVKey::PVK_NUMPAD4:
            return XK_KP_4;
            break;
        case portableVKey::PVK_NUMPAD5:
            return XK_KP_5;
            break;
        case portableVKey::PVK_NUMPAD6:
            return XK_KP_6;
            break;
        case portableVKey::PVK_NUMPAD7:
            return XK_KP_7;
            break;
        case portableVKey::PVK_NUMPAD8:
            return XK_KP_8;
            break;
        case portableVKey::PVK_NUMPAD9:
            return XK_KP_9;
            break;

        case portableVKey::PVK_RETURN:
            return XK_Return;
            break;
        case portableVKey::PVK_TAB:
            return XK_Tab;
            break;
        case portableVKey::PVK_SPACE
            return XK_Linefeed;
            break;
        case portableVKey::PVK_BACK:
            return XK_BackSpace;
            break;
        case portableVKey::PVK_ESCAPE:
            return XK_Escape;
            break;
        case portableVKey::PVK_CAPSLOCK:
            return XK_Caps_Lock;
            break;
        case portableVKey::PVK_F17:
            return XK_F17;
            break;
        //case portableVKey::PVK_VOLUME_UP:
        //    return kVK_VolumeUp;
        //    break;
        //case portableVKey::PVK_VOLUME_DOWN:
        //    return kVK_VolumeDown;
        //    break;
        //case portableVKey::PVK_VOLUME_MUTE:
        //    return kVK_Mute;
        //    break;
        case portableVKey::PVK_F18:
            return XK_F18;
            break;
        case portableVKey::PVK_F19:
            return XK_F19;
            break;
        case portableVKey::PVK_F20:
            return XK_F20;
            break;
        case portableVKey::PVK_F5:
            return XK_F5;
            break;
        case portableVKey::PVK_F6:
            return XK_F6;
            break;
        case portableVKey::PVK_F7:
            return XK_F7;
            break;
        case portableVKey::PVK_F3:
            return XK_F3;
            break;
        case portableVKey::PVK_F8:
            return XK_F8;
            break;
        case portableVKey::PVK_F9:
            return XK_F9;
            break;
        case portableVKey::PVK_F11:
            return XK_F11;
            break;
        case portableVKey::PVK_F13:
            return XK_F13;
            break;
        case portableVKey::PVK_F16:
            return XK_F16;
            break;
        case portableVKey::PVK_F14:
            return XK_F14;
            break;
        case portableVKey::PVK_F10:
            return XK_F10;
            break;
        case portableVKey::PVK_F12:
            return XK_F12;
            break;
        case portableVKey::PVK_F15:
            return XK_F15;
            break;
        case portableVKey::PVK_HELP:
            return XK_Help;
            break;
        case portableVKey::PVK_HOME:
            return XK_Home;
            break;
        case portableVKey::PVK_PAGE_UP:
            return XK_Page_Up;
            break;
        case portableVKey::PVK_DELETE:
            return XK_Delete; // ?
            break;
        case portableVKey::PVK_F4:
            return XK_F4;
            break;
        case portableVKey::PVK_END:
            return XK_End;
            break;
        case portableVKey::PVK_F2:
            return XK_F2;
            break;
        case portableVKey::PVK_PAGE_DOWN:
            return XK_Page_Down;
            break;
        case  portableVKey::PVK_F1:
            return XK_F1;
            break;
        case portableVKey::PVK_LEFT:
            return XK_Left;
            break;
        case portableVKey::PVK_RIGHT:
            return XK_Right;
            break;
        case portableVKey::PVK_DOWN:
            return XK_Down;
            break;
        case portableVKey::PVK_UP:
            return XK_Up;
            break;
        case portableVKey::PVK_NUMLOCK:
        return XK_Num_Lock; //TODO: den paizei
        break;
        case portableVKey::PVK_INSERT:
            return XK_Insert;
            break;
        //case portableVKey::PVK_LWIN:
        //case portableVKey::PVK_RWIN:
        //    return kVK_Control;
        //    break;
        }

        //0-9, A-Z
        if (portableKey >= 0x30 && portableKey <= 0x5A){
            return portableKey;
        }

        return -1; // den vrethike annalogo key
}

#endif

