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

#ifndef NODEOUTPUT_H
#define NODEOUTPUT_H

#include "param.h"

/**
 * @brief A node parameter designed to serve data to the input of another node
 */
class NodeOutput : public NodeParam
{
public:
  /**
   * @brief NodeOutput Constructor
   */
  NodeOutput();

  /**
   * @brief Returns kOutput
   */
  virtual Type type() override;

  /**
   * @brief The data type this parameter outputs
   *
   * This can be used in conjunction with NodeInput::can_accept_type() to determine whether this parameter can be
   * connected to it.
   */
  const DataType& data_type();
  void set_data_type(const DataType& type);

  /**
   * @brief Get the value of this output at a given tie
   *
   * This function is intended to primarily be called by any connected NodeInputs.
   *
   * The first thing this function does is request the parent Node object to Process() at this time. The Node should
   * then perform whatever actions necessary (usually taking data from inputs and creating output data) to set the
   * correct value that this output should have at this time (the Node should use set_value() for this). This function
   * will then return the value that was set after Process() returned.
   *
   * In many cases for efficiency, the Node can also ignore this request if it knows the output data will not change
   * (i.e. if the time has not changed from the last Process()).
   */
  virtual const QVariant& get_value(const rational &time);

  /**
   * @brief Set the current value of this output
   *
   * Intended to only be set by parent Node objects in their Node::Process() function. Whatever result data is intended
   * for use later in the pipeline should be set here (\see get_value()).
   */
  virtual void set_value(const QVariant& value);

private:
  DataType data_type_;

  QVariant value_;
};

#endif // NODEOUTPUT_H
