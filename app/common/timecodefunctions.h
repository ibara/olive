/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2019 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#ifndef TIMECODEFUNCTIONS_H
#define TIMECODEFUNCTIONS_H

#include <QString>

#include "common/rational.h"

namespace olive {

enum TimecodeDisplay {
  kTimecodeFrames,
  kTimecodeSeconds,
  kFrames,
  kMilliseconds
};

/**
 * @brief Convert a timestamp (according to a rational timebase) to a user-friendly string representation
 */
QString timestamp_to_timecode(const int64_t &timestamp, const rational& timebase, const TimecodeDisplay& display);

}

#endif // TIMECODEFUNCTIONS_H
