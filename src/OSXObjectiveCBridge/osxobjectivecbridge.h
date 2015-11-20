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

#ifndef OSXOBJECTIVECBRIDGE_H
#define OSXOBJECTIVECBRIDGE_H

#include "osxobjectivecbridge_global.h"

OSXOBJECTIVECBRIDGESHARED_EXPORT void printsomething();

#if !defined(__cplusplus)
#define C_API extern
#else
#define C_API extern "C"
#endif

namespace objcappnap
{
    OSXOBJECTIVECBRIDGESHARED_EXPORT void printsomething();
    C_API OSXOBJECTIVECBRIDGESHARED_EXPORT void disableAppNap();
}

#endif // OSXOBJECTIVECBRIDGE_H
