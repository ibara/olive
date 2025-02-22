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

#include "viewerglwidget.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

#include "render/gl/functions.h"
#include "render/gl/shadergenerators.h"

ViewerGLWidget::ViewerGLWidget(QWidget *parent) :
  QOpenGLWidget(parent),
  texture_(0)
{
}

void ViewerGLWidget::SetTexture(GLuint tex)
{
  // Update the texture
  texture_ = tex;

  // Paint the texture
  update();
}

void ViewerGLWidget::initializeGL()
{
  // Re-retrieve pipeline pertaining to this context
  pipeline_ = olive::gl::GetDefaultPipeline();
}

void ViewerGLWidget::paintGL()
{
  // Get functions attached to this context (they will already be initialized)
  QOpenGLFunctions* f = context()->functions();

  // Clear background to empty
  f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  f->glClear(GL_COLOR_BUFFER_BIT);

  // Check if we have a texture to draw
  if (texture_ > 0) {
    // Bind retrieved texture
    f->glBindTexture(GL_TEXTURE_2D, texture_);

    // Blit using the pipeline retrieved in initializeGL()
    olive::gl::Blit(pipeline_, true);

    // Release retrieved texture
    f->glBindTexture(GL_TEXTURE_2D, 0);
  }
}
