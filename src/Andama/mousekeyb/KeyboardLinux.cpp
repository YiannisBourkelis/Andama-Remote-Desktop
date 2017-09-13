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
#define XK_LATIN1
#define XK_MISCELLANY
#include <X11/keysymdef.h>
#include <X11/XF86keysym.h>
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
    switch (qtkey)
    {
    case Qt::Key_Space:
        return portableVKey::PVK_SPACE;
    case Qt::Key_Alt:
        return portableVKey::PVK_ALT;
    case Qt::Key_Meta:
        return portableVKey::PVK_LWIN;
    case Qt::Key_Multi_key:
        return portableVKey::PVK_RWIN;
    case Qt::Key_Control:
        return portableVKey::PVK_CONTROL;
    case Qt::Key_Shift:
        return portableVKey::PVK_SHIFT;
    case Qt::Key_CapsLock:
        return portableVKey::PVK_CAPSLOCK;
    case Qt::Key_Tab:
        return portableVKey::PVK_TAB;
    case Qt::Key_Escape:
        return portableVKey::PVK_ESCAPE;
    case Qt::Key_Backspace:
        return portableVKey::PVK_BACK;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        return portableVKey::PVK_RETURN;

    case Qt::Key_Left:
        return portableVKey::PVK_LEFT;
    case Qt::Key_Right:
        return portableVKey::PVK_RIGHT;
    case Qt::Key_Up:
        return portableVKey::PVK_UP;
    case Qt::Key_Down:
        return portableVKey::PVK_DOWN;

    case Qt::Key_NumLock:
        return portableVKey::PVK_NUMLOCK;
    case Qt::Key_0:
        return portableVKey::PVK_NUMPAD0;
    case Qt::Key_1:
        return portableVKey::PVK_NUMPAD1;
    case Qt::Key_2:
        return portableVKey::PVK_NUMPAD2;
    case Qt::Key_3:
        return portableVKey::PVK_NUMPAD3;
    case Qt::Key_4:
        return portableVKey::PVK_NUMPAD4;
    case Qt::Key_5:
        return portableVKey::PVK_NUMPAD5;
    case Qt::Key_6:
        return portableVKey::PVK_NUMPAD6;
    case Qt::Key_7:
        return portableVKey::PVK_NUMPAD7;
    case Qt::Key_8:
        return portableVKey::PVK_NUMPAD8;
    case Qt::Key_9:
        return portableVKey::PVK_NUMPAD9;

    case Qt::Key_Equal:
        return portableVKey::PVK_EQUAL;
    case Qt::Key_Plus:
        return portableVKey::PVK_ADD;
    case Qt::Key_Minus:
        return portableVKey::PVK_MINUS;
    case Qt::Key_multiply:
    case Qt::Key_Asterisk:
        return portableVKey::PVK_MULTIPLY;
    case Qt::Key_division:
    case Qt::Key_Slash:
        return portableVKey::PVK_SLASH;
    case Qt::Key_BracketLeft:
        return portableVKey::PVK_LEFT_BRACKET;
    case Qt::Key_BracketRight:
        return portableVKey::PVK_RIGHT_BRACKET;
    case Qt::Key_Backslash:
        return portableVKey::PVK_BACKSLASH;

    case Qt::Key_Semicolon:
        return portableVKey::PVK_SEMICOLON;
    case Qt::Key_Comma:
        return portableVKey::PVK_COMMA;
    case Qt::Key_Period:
        return portableVKey::PVK_PERIOD;
    case Qt::Key_QuoteDbl:
        return portableVKey::PVK_QUOTE;

    case Qt::Key_Insert:
        return portableVKey::PVK_INSERT;
    case Qt::Key_Delete:
        return portableVKey::PVK_DELETE;
    case Qt::Key_Home:
        return portableVKey::PVK_HOME;
    case Qt::Key_End:
        return portableVKey::PVK_END;
    case Qt::Key_PageUp:
        return portableVKey::PVK_PAGE_UP;
    case Qt::Key_PageDown:
        return portableVKey::PVK_PAGE_DOWN;

    case Qt::Key_F1:
        return portableVKey::PVK_F1;
    case Qt::Key_F2:
        return portableVKey::PVK_F2;
    case Qt::Key_F3:
        return portableVKey::PVK_F3;
    case Qt::Key_F4:
        return portableVKey::PVK_F4;
    case Qt::Key_F5:
        return portableVKey::PVK_F5;
    case Qt::Key_F6:
        return portableVKey::PVK_F6;
    case Qt::Key_F7:
        return portableVKey::PVK_F7;
    case Qt::Key_F8:
        return portableVKey::PVK_F8;
    case Qt::Key_F9:
        return portableVKey::PVK_F9;
    case Qt::Key_F10:
        return portableVKey::PVK_F10;
    case Qt::Key_F11:
        return portableVKey::PVK_F11;
    case Qt::Key_F12:
        return portableVKey::PVK_F12;
    case Qt::Key_F13:
        return portableVKey::PVK_F13;
    case Qt::Key_F14:
        return portableVKey::PVK_F14;
    case Qt::Key_F15:
        return portableVKey::PVK_F15;
    case Qt::Key_F16:
        return portableVKey::PVK_F16;
    case Qt::Key_F17:
        return portableVKey::PVK_F17;
    case Qt::Key_F18:
        return portableVKey::PVK_F18;
    case Qt::Key_F19:
        return portableVKey::PVK_F19;
    case Qt::Key_F20:
        return portableVKey::PVK_F20;

    case Qt::Key_VolumeUp:
        return portableVKey::PVK_VOLUME_UP;
    case Qt::Key_VolumeDown:
        return portableVKey::PVK_VOLUME_DOWN;
    case Qt::Key_VolumeMute:
        return portableVKey::PVK_VOLUME_MUTE;

    case Qt::Key_Help:
        return portableVKey::PVK_HELP;
    case Qt::Key_QuoteLeft:
        return portableVKey::PVK_GRAVE;
    }

    //A-Z
    if (qtkey >= Qt::Key_A && qtkey <= Qt::Key_Z){
        return (portableVKey) qtkey;
    }

    return PVK_UNKNOWN; // den vrethike annalogo key
}

int Keyboard::convertPortableKeyToLocal(portableVKey portableKey)
{
    switch (portableKey)
    {
    case portableVKey::PVK_EQUAL:
        return XK_equal;
    case portableVKey::PVK_MINUS:
        return XK_minus;
    case portableVKey::PVK_RIGHT_BRACKET:
        return XK_bracketright;
    case portableVKey::PVK_LEFT_BRACKET:
        return XK_bracketleft;
    case portableVKey::PVK_QUOTE:
        return XK_quotedbl;
    case portableVKey::PVK_SHIFT:
        return XK_Shift_L;
    case portableVKey::PVK_CONTROL:
        return XK_Control_L;
    case portableVKey::PVK_ALT:
        return XK_Alt_L;
    case portableVKey::PVK_SEMICOLON:
        return XK_semicolon;
    case portableVKey::PVK_BACKSLASH:
        return XK_backslash;
    case portableVKey::PVK_COMMA:
        return XK_comma;
    case portableVKey::PVK_SLASH:
        return XK_slash;
    case portableVKey::PVK_PERIOD:
        return XK_period;
    case portableVKey::PVK_GRAVE:
        return XK_grave;
    case portableVKey::PVK_DECIMAL:
        return XK_KP_Decimal;
    case portableVKey::PVK_MULTIPLY:
        return XK_KP_Multiply;
    case portableVKey::PVK_ADD:
        return XK_KP_Add;
    case portableVKey::PVK_KEYPAD_DIVIDE:
        return XK_KP_Divide;
    case portableVKey::PVK_KEYPAD_MINUS:
        return XK_KP_Subtract;

    case portableVKey::PVK_NUMPAD0:
        return XK_KP_0;
    case portableVKey::PVK_NUMPAD1:
        return XK_KP_1;
    case portableVKey::PVK_NUMPAD2:
        return XK_KP_2;
    case portableVKey::PVK_NUMPAD3:
        return XK_KP_3;
    case portableVKey::PVK_NUMPAD4:
        return XK_KP_4;
    case portableVKey::PVK_NUMPAD5:
        return XK_KP_5;
    case portableVKey::PVK_NUMPAD6:
        return XK_KP_6;
    case portableVKey::PVK_NUMPAD7:
        return XK_KP_7;
    case portableVKey::PVK_NUMPAD8:
        return XK_KP_8;
    case portableVKey::PVK_NUMPAD9:
        return XK_KP_9;

    case portableVKey::PVK_RETURN:
        return XK_Return;
    case portableVKey::PVK_TAB:
        return XK_Tab;
    case portableVKey::PVK_SPACE:
        return XK_space;
    case portableVKey::PVK_BACK:
        return XK_BackSpace;
    case portableVKey::PVK_ESCAPE:
        return XK_Escape;
    case portableVKey::PVK_CAPSLOCK:
        return XK_Caps_Lock;

    case portableVKey::PVK_VOLUME_UP:
        return XF86XK_AudioRaiseVolume;
    case portableVKey::PVK_VOLUME_DOWN:
        return XF86XK_AudioLowerVolume;
    case portableVKey::PVK_VOLUME_MUTE:
        return XF86XK_AudioMute;

    case portableVKey::PVK_F1:
        return XK_F1;
    case portableVKey::PVK_F2:
        return XK_F2;
    case portableVKey::PVK_F3:
        return XK_F3;
    case portableVKey::PVK_F4:
        return XK_F4;
    case portableVKey::PVK_F5:
        return XK_F5;
    case portableVKey::PVK_F6:
        return XK_F6;
    case portableVKey::PVK_F7:
        return XK_F7;
    case portableVKey::PVK_F8:
        return XK_F8;
    case portableVKey::PVK_F9:
        return XK_F9;
    case portableVKey::PVK_F10:
        return XK_F10;
    case portableVKey::PVK_F11:
        return XK_F11;
    case portableVKey::PVK_F12:
        return XK_F12;
    case portableVKey::PVK_F13:
        return XK_F13;
    case portableVKey::PVK_F14:
        return XK_F14;
    case portableVKey::PVK_F15:
        return XK_F15;
    case portableVKey::PVK_F16:
        return XK_F16;
    case portableVKey::PVK_F17:
        return XK_F17;
    case portableVKey::PVK_F18:
        return XK_F18;
    case portableVKey::PVK_F19:
        return XK_F19;
    case portableVKey::PVK_F20:
        return XK_F20;
    case portableVKey::PVK_F21:
        return XK_F21;
    case portableVKey::PVK_F22:
        return XK_F22;
    case portableVKey::PVK_F23:
        return XK_F23;
    case portableVKey::PVK_F24:
        return XK_F24;

    case portableVKey::PVK_HELP:
        return XK_Help;
    case portableVKey::PVK_HOME:
        return XK_Home;
    case portableVKey::PVK_PAGE_UP:
        return XK_Page_Up;
    case portableVKey::PVK_DELETE:
        return XK_Delete;
    case portableVKey::PVK_END:
        return XK_End;
    case portableVKey::PVK_PAGE_DOWN:
        return XK_Page_Down;

    case portableVKey::PVK_LEFT:
        return XK_Left;
    case portableVKey::PVK_RIGHT:
        return XK_Right;
    case portableVKey::PVK_UP:
        return XK_Up;
    case portableVKey::PVK_DOWN:
        return XK_Down;
    case portableVKey::PVK_NUMLOCK:
        return XK_Num_Lock;
    case portableVKey::PVK_INSERT:
        return XK_Insert;
    case portableVKey::PVK_LWIN:
        return XK_Super_L;
    case portableVKey::PVK_RWIN:
        return XK_Multi_key;

    case portableVKey::PVK_A:
    case portableVKey::PVK_B:
    case portableVKey::PVK_C:
    case portableVKey::PVK_D:
    case portableVKey::PVK_E:
    case portableVKey::PVK_F:
    case portableVKey::PVK_G:
    case portableVKey::PVK_H:
    case portableVKey::PVK_I:
    case portableVKey::PVK_J:
    case portableVKey::PVK_K:
    case portableVKey::PVK_L:
    case portableVKey::PVK_M:
    case portableVKey::PVK_N:
    case portableVKey::PVK_O:
    case portableVKey::PVK_P:
    case portableVKey::PVK_Q:
    case portableVKey::PVK_R:
    case portableVKey::PVK_S:
    case portableVKey::PVK_T:
    case portableVKey::PVK_U:
    case portableVKey::PVK_V:
    case portableVKey::PVK_W:
    case portableVKey::PVK_X:
    case portableVKey::PVK_Y:
    case portableVKey::PVK_Z:
    case portableVKey::PVK_0:
    case portableVKey::PVK_1:
    case portableVKey::PVK_2:
    case portableVKey::PVK_3:
    case portableVKey::PVK_4:
    case portableVKey::PVK_5:
    case portableVKey::PVK_6:
    case portableVKey::PVK_7:
    case portableVKey::PVK_8:
    case portableVKey::PVK_9:
        return portableKey;

    case PVK_CLEAR:
    case PVK_SEPARATOR:
    case PVK_UNKNOWN:
        break;
    }

    return -1; // den vrethike annalogo key
}

#endif
