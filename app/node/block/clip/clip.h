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

#ifndef CLIPBLOCK_H
#define CLIPBLOCK_H

#include "node/block/block.h"

/**
 * @brief Node that represents a block of Media
 */
class ClipBlock : public Block
{
  Q_OBJECT
public:
  ClipBlock();

  virtual QString Name() override;
  virtual QString id() override;
  virtual QString Description() override;

  virtual rational length() override;
  virtual void set_length(const rational &length) override;

  NodeInput* texture_input();

public slots:
  virtual void Process(const rational &time) override;

private:
  NodeInput* texture_input_;

  rational media_in_;

  rational length_;
};

#endif // TIMELINEBLOCK_H
