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

#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H

#include <QPushButton>

#include "tool/tool.h"

/**
 * @brief Simple derived class of QPushButton to contain an Tool ID. Used as the main widget through Toolbar.
 */
class ToolbarButton : public QPushButton
{
public:
  /**
   * @brief ToolbarButton Constructor
   *
   * @param parent
   *
   * QWidget parent. Almost always an instance of Toolbar.
   *
   * @param icon
   *
   * Icon to set this QWidget to.
   *
   * @param tool
   *
   * Tool object. Must be a member of enum olive::tool::Tool, including kNone if this button does not represent a tool.
   */
  ToolbarButton(QWidget* parent, const QIcon& icon, const olive::tool::Tool& tool);

  /**
   * @brief Retrieve tool ID that this button represents
   *
   * Set in the constructor and shouldn't change throughout its lifetime.
   */
  const olive::tool::Tool& tool();
private:
  /**
   * @brief Internal tool value
   */
  olive::tool::Tool tool_;
};

#endif // TOOLBARBUTTON_H
