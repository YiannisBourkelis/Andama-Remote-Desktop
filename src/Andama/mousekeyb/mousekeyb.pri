# * ***********************************************************************
# * Andama
# * (C) 2014 by Yiannis Bourkelis (hello@andama.org)
# *
# * This file is part of Andama.
# *
# * Andama is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * Andama is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with Andama.  If not, see <http://www.gnu.org/licenses/>.
# * ***********************************************************************/

HEADERS += \
    $$PWD/Mouse.h \
    $$PWD/Keyboard.h \
    $$PWD/MouseCursorHook.h

SOURCES += \
    $$PWD/MouseWindows.cpp \
    $$PWD/MouseMac.cpp \
    $$PWD/KeyboardMac.cpp \
    $$PWD/KeyboardWindows.cpp \
    $$PWD/Keyboard.cpp \
    $$PWD/MouseLinux.cpp \
    $$PWD/KeyboardLinux.cpp \
    $$PWD/MouseCursorHookWindows.cpp
