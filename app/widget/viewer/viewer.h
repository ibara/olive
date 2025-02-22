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

#ifndef VIEWER_WIDGET_H
#define VIEWER_WIDGET_H

#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QWidget>

#include "common/rational.h"
#include "viewerglwidget.h"
#include "widget/playbackcontrols/playbackcontrols.h"
#include "widget/timeruler/timeruler.h"

/**
 * @brief An OpenGL-based viewer widget with playback controls (a PlaybackControls widget).
 */
class ViewerWidget : public QWidget
{
  Q_OBJECT
public:
  ViewerWidget(QWidget* parent);

  void SetPlaybackControlsEnabled(bool enabled);

  void SetTimeRulerEnabled(bool enabled);

  void SetTimebase(const rational& r);

public slots:
  /**
   * @brief Set the texture to draw and draw it
   *
   * Wrapper function for ViewerGLWidget::SetTexture().
   *
   * @param tex
   */
  void SetTexture(GLuint tex);

signals:
  void TimeChanged(const rational&);

protected:
  virtual void resizeEvent(QResizeEvent *event) override;

private:
  ViewerGLWidget* gl_widget_;

  PlaybackControls* controls_;

  TimeRuler* ruler_;

  QScrollBar* scrollbar_;

  rational time_base_;

private slots:
  void RulerTimeChange(int64_t i);
};

#endif // VIEWER_WIDGET_H
