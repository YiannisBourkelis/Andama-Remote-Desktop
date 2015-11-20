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


portableKeyboardModifier Keyboard::getPortableModifier (int key)
{
    switch (key)
    {
        case Qt::Key_Shift:
        return portableKeyboardModifier::Shift;
                break;
    case Qt::Key_Control:
        return portableKeyboardModifier::Control;
                break;
    case Qt::Key_Alt:
        return portableKeyboardModifier::Alt;
                break;
    case Qt::Key_Meta:
        return portableKeyboardModifier::Meta;
                break;
    //case Qt::key_key:
    //    return portableKeyboardModifier::Keypad;
    //            break;
    //case Qt::GroupSwitchModifier:
    //    return portableKeyboardModifier::GroupSwitch;
    //            break;
        default:
        return portableKeyboardModifier::NoModifier;
            break;
    }
}
/*
portableKeyboardModifier Keyboard::getPortableModifier (int key)
{
    switch (key)
    {
        case Qt::KeyboardModifier::ShiftModifier:
        return portableKeyboardModifier::Shift;
                break;
        case Qt::KeyboardModifier::ControlModifier:
        return portableKeyboardModifier::Control;
                break;
        case Qt::KeyboardModifier::AltModifier:
        return portableKeyboardModifier::Alt;
                break;
        case Qt::KeyboardModifier::MetaModifier:
        return portableKeyboardModifier::Meta;
                break;
        case Qt::KeyboardModifier::KeypadModifier:
        return portableKeyboardModifier::Keypad;
                break;
        case Qt::KeyboardModifier::GroupSwitchModifier:
        return portableKeyboardModifier::GroupSwitch;
                break;
        default:
        return portableKeyboardModifier::NoModifier;
            break;
    }
}*/
