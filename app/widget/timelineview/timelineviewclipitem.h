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

#ifndef TIMELINEVIEWCLIPITEM_H
#define TIMELINEVIEWCLIPITEM_H

#include "timelineviewrect.h"
#include "node/block/clip/clip.h"
#include "timelineviewghostitem.h"

class TimelineViewClipItem : public TimelineViewRect
{
public:
  TimelineViewClipItem(QGraphicsItem* parent = nullptr);

  ClipBlock* clip();
  void SetClip(ClipBlock* clip);

protected:
  virtual void UpdateRect() override;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
  ClipBlock* clip_;
};

#endif // TIMELINEVIEWCLIPITEM_H
