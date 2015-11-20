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

#include "osxobjectivecbridge.h"
#include "iostream"
#include <Foundation/Foundation.h>


namespace objcappnap
{

    OSXOBJECTIVECBRIDGESHARED_EXPORT void printsomething(){
        std::cout  << "printsomething17" << std::endl;
    }

    static id activity;

    OSXOBJECTIVECBRIDGESHARED_EXPORT void disableAppNap() {
        activity = [[NSProcessInfo processInfo] beginActivityWithOptions:NSActivityBackground | NSActivityIdleDisplaySleepDisabled |
                                                                             NSActivityLatencyCritical | NSActivityUserInitiated
                                                                 reason:@"Disable App Nap"];

    }
}
