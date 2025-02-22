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

#include "timecodefunctions.h"

#include <QtMath>

QString padded(int arg, int padding) {
  return QString("%1").arg(arg, padding, 10, QChar('0'));
}

QString olive::timestamp_to_timecode(const int64_t &timestamp,
                                     const rational& timebase,
                                     const TimecodeDisplay& display)
{
  double timestamp_dbl = (rational(timestamp) * timebase).ToDouble();

  switch (display) {
  case kTimecodeFrames:
  case kTimecodeSeconds:
  {
    int total_seconds = qFloor(timestamp_dbl);

    int hours = total_seconds / 3600;
    int mins = total_seconds / 60 - hours * 60;
    int secs = total_seconds - mins * 60;

    if (display == kTimecodeSeconds) {
      int fraction = qRound((timestamp_dbl - total_seconds) * 1000);

      return QString("%1:%2:%3.%4").arg(padded(hours, 2),
                                        padded(mins, 2),
                                        padded(secs, 2),
                                        padded(fraction, 3));
    } else {
      rational frame_rate = timebase.flipped();

      int frames = qRound((timestamp_dbl - total_seconds) * frame_rate.ToDouble());

      return QString("%1:%2:%3;%4").arg(padded(hours, 2),
                                        padded(mins, 2),
                                        padded(secs, 2),
                                        padded(frames, 2));
    }
  }
  case kFrames:
    return QString::number(timestamp);
  case kMilliseconds:
    return QString::number(qRound(timestamp_dbl * 1000));
  }

  return QString();
}
