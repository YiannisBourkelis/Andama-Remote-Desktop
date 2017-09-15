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

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QCursor> //gia na dexetai to Q_OS_MAC klp...
#include <unordered_map>
#include <QKeyEvent>

#ifdef _WIN32
#include <windows.h>
#define VIRTUAL_TO_SCAN_CODE 0
#endif

enum portableKeyboardModifier
{
    NoModifier = 0,
    Shift = 1,
    Control = 2,
    Alt = 4,
    Meta = 8,
    Keypad = 16,
    GroupSwitch = 32
};

enum portableVKey
{
    PVK_UNKNOWN = -0x01,

    PVK_A = 0x041,
    PVK_B = 0x042,
    PVK_C = 0x043,
    PVK_D = 0x044,
    PVK_E = 0x045,
    PVK_F = 0x046,
    PVK_G = 0x047,
    PVK_H = 0x048,
    PVK_I = 0x049,
    PVK_J = 0x04A,
    PVK_K = 0x04B,
    PVK_L = 0x04C,
    PVK_M = 0x04D,
    PVK_N = 0x04E,
    PVK_O = 0x04F,
    PVK_P = 0x050,
    PVK_Q = 0x051,
    PVK_R = 0x052,
    PVK_S = 0x053,
    PVK_T = 0x054,
    PVK_U = 0x055,
    PVK_V = 0x056,
    PVK_W = 0x057,
    PVK_X = 0x058,
    PVK_Y = 0x059,
    PVK_Z = 0x05A,

    PVK_0 = 0x30,
    PVK_1 = 0x31,
    PVK_2 = 0x32,
    PVK_3 = 0x33,
    PVK_4 = 0x34,
    PVK_5 = 0x35,
    PVK_6 = 0x36,
    PVK_7 = 0x37,
    PVK_8 = 0x38,
    PVK_9 = 0x39,

    PVK_TAB          = 0x09,
    PVK_CLEAR        = 0x0c,
    PVK_RETURN       = 0x0d,
    //PVK_PAUSE        = 0x13,
    PVK_ESCAPE       = 0x1b,
    PVK_SPACE        = 0x20,
    PVK_CAPSLOCK     = 0x14,

    PVK_PAGE_UP      = 0x21, // PRIOR
    PVK_PAGE_DOWN    = 0x22, // NEXT
    PVK_END          = 0x23,
    PVK_HOME         = 0x24,
    PVK_LEFT         = 0x25,
    PVK_UP           = 0x26,
    PVK_RIGHT        = 0x27,
    PVK_DOWN         = 0x28,

    //PVK_PRINT        = 0x2a,
    //PVK_EXECUTE      = 0x2b,
    //PVK_SNAPSHOT     = 0x2c,  // Printscreen key
    PVK_INSERT       = 0x2d,
    PVK_DELETE       = 0x2e,
    PVK_HELP         = 0x2f,

    PVK_NUMPAD0      = 0x60,
    PVK_NUMPAD1      = 0x61,
    PVK_NUMPAD2      = 0x62,
    PVK_NUMPAD3      = 0x63,
    PVK_NUMPAD4      = 0x64,
    PVK_NUMPAD5      = 0x65,
    PVK_NUMPAD6      = 0x66,
    PVK_NUMPAD7      = 0x67,
    PVK_NUMPAD8      = 0x68,
    PVK_NUMPAD9      = 0x69,
    PVK_MULTIPLY     = 0x6A,
    PVK_ADD          = 0x6B,
    PVK_SEPARATOR    = 0x6C,
    PVK_KEYPAD_MINUS = 0x6D,
    PVK_DECIMAL      = 0x6E,
    PVK_KEYPAD_DIVIDE= 0x6F,

    PVK_F1           = 0x70,
    PVK_F2           = 0x71,
    PVK_F3           = 0x72,
    PVK_F4           = 0x73,
    PVK_F5           = 0x74,
    PVK_F6           = 0x75,
    PVK_F7           = 0x76,
    PVK_F8           = 0x77,
    PVK_F9           = 0x78,
    PVK_F10          = 0x79,
    PVK_F11          = 0x7a,
    PVK_F12          = 0x7b,
    PVK_F13          = 0x7c,
    PVK_F14          = 0x7d,
    PVK_F15          = 0x7e,
    PVK_F16          = 0x7f,
    PVK_F17          = 0x80,
    PVK_F18          = 0x81,
    PVK_F19          = 0x82,
    PVK_F20          = 0x83,
    PVK_F21          = 0x84,
    PVK_F22          = 0x85,
    PVK_F23          = 0x86,
    PVK_F24          = 0x87,

    PVK_NUMLOCK      = 0x90,

    PVK_VOLUME_MUTE  = 0xAD,
    PVK_VOLUME_DOWN  = 0xAE,
    PVK_VOLUME_UP    = 0xAF,

    PVK_QUOTE        = 0xDE, //VK_OEM_7
    PVK_BACKSLASH    = 0xDC, //0xE2, // VK_OEM_102
    PVK_COMMA        = 0xBC, //VK_OEM_COMMA
    PVK_PERIOD       = 0xBE, //VK_OEM_PERIOD
    PVK_BACK         = 0x08,

    PVK_SHIFT        = 0x10,
    PVK_CONTROL      = 0x11,
    PVK_ALT          = 0x12,

    PVK_LEFT_BRACKET = 0xDB,
    PVK_RIGHT_BRACKET = 0xDD,
    PVK_EQUAL        = 0xBB,
    PVK_SEMICOLON    = 0xBA,
    PVK_MINUS        = 0xBD,
    PVK_SLASH        = 0xBF,
    PVK_GRAVE        = 0xC0,
    PVK_LWIN         = 0x5B, //mono apo windows, map se control ston mac
    PVK_RWIN         = 0x5C, //mono apo windows, map se control ston mac

    PVK_TILDE        = 0xFF00,
    PVK_UNDERSCORE   = 0xFF01,
    PVK_BAR          = 0xFF02,
    PVK_AMPERSAND    = 0xFF03,
    PVK_EXCLAMATION  = 0xFF04,
    PVK_PERCENT      = 0xFF05,
    PVK_APOSTROPHE   = 0xFF06,
    PVK_AT           = 0xFF07,
    PVK_COLON        = 0xFF08,
    PVK_DOLLAR       = 0xFF09,
    PVK_QUESTION     = 0xFF0A,
    PVK_LEFT_BRACE   = 0xFF0B,
    PVK_RIGHT_BRACE  = 0xFF0C,
    PVK_LEFT_PAREN   = 0xFF0D,
    PVK_RIGHT_PAREN  = 0xFF0E,
    PVK_LESS_THAN    = 0xFF0F,
    PVK_GREATER_THAN = 0xFF10,
    PVK_NUMBER       = 0xFF11
};

class Keyboard {
public:
#ifdef Q_OS_WIN
    static INPUT getKeyEvent(int key, bool keyUp = false);
#endif

    static void keyPress(int key, int modifiers);
    static void keyRelease(int key, int modifiers);
    static portableKeyboardModifier getPortableModifier(int key);
    static portableVKey getPortableVKey(int key, int qtkey);
    static int convertPortableKeyToLocal(portableVKey portableKey);

private:
    std::unordered_map<int,int> Keys;
    //static bool Caps;
};

#endif // KEYBOARD_H
