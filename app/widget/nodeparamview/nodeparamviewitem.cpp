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

#include "nodeparamviewitem.h"

#include <QCheckBox>
#include <QDebug>
#include <QFontComboBox>
#include <QLineEdit>
#include <QPainter>

#include "project/item/sequence/sequence.h"
#include "ui/icons/icons.h"
#include "widget/footagecombobox/footagecombobox.h"

// FIXME: Test code only
#include "panel/panelmanager.h"
#include "panel/project/project.h"
// End test code

NodeParamViewItem::NodeParamViewItem(QWidget *parent) :
  QWidget(parent)
{
  QVBoxLayout* main_layout = new QVBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setMargin(0);

  // Create title bar widget
  title_bar_ = new NodeParamViewItemTitleBar(this);
  title_bar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

  QHBoxLayout* title_bar_layout = new QHBoxLayout(title_bar_);

  title_bar_collapse_btn_ = new QPushButton();
  title_bar_collapse_btn_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
  title_bar_collapse_btn_->setStyleSheet("border: none; background: none;");
  title_bar_collapse_btn_->setCheckable(true);
  title_bar_collapse_btn_->setChecked(true);

  // FIXME: Revise icon sizing algorithm (share with NodeViewItem)
  title_bar_collapse_btn_->setIconSize(QSize(fontMetrics().height()/2, fontMetrics().height()/2));

  connect(title_bar_collapse_btn_, SIGNAL(clicked(bool)), this, SLOT(SetExpanded(bool)));
  title_bar_layout->addWidget(title_bar_collapse_btn_);

  title_bar_lbl_ = new QLabel(title_bar_);
  title_bar_layout->addWidget(title_bar_lbl_);

  // Add title bar to widget
  main_layout->addWidget(title_bar_);

  // Create and add contents widget
  contents_ = new QWidget(this);
  content_layout_ = new QGridLayout(contents_);
  main_layout->addWidget(contents_);

  // Set correct widget state
  SetExpanded(title_bar_collapse_btn_->isChecked());
}

void NodeParamViewItem::AttachNode(Node *n)
{
  // Make sure we can attach this node (CanAddNode() should be run by the caller to make sure this node is valid)
  Q_ASSERT(CanAddNode(n));

  // Add node to the list
  nodes_.append(n);

  // If the added node was the first node, set up the UI
  if (nodes_.size() == 1) {
    SetupUI();
  }
}

bool NodeParamViewItem::CanAddNode(Node* n)
{
  // Ensures that all Nodes have the same ID
  return (nodes_.isEmpty() || nodes_.first()->id() == n->id());
}

void NodeParamViewItem::changeEvent(QEvent *e)
{
  // FIXME: Retranslate all UI elements

  QWidget::changeEvent(e);
}

void NodeParamViewItem::SetupUI()
{
  Q_ASSERT(!nodes_.isEmpty());

  Node* first_node = nodes_.first();

  title_bar_lbl_->setText(first_node->Name());

  int row_count = 0;

  for (int i=0;i<first_node->ParameterCount();i++) {
    NodeParam* param = first_node->ParamAt(i);

    // This widget only needs to show input parameters
    if (param->type() == NodeParam::kInput) {

      // Add descriptor label
      QLabel* param_label = new QLabel(tr("%1:").arg(param->name()));

      content_layout_->addWidget(param_label, row_count, 0);

      // Add widgets for this parameter
      QList<QWidget*> widgets_for_param = CreateWidget(static_cast<NodeInput*>(param));
      for (int i=0;i<widgets_for_param.size();i++) {
        content_layout_->addWidget(widgets_for_param.at(i), row_count, i + 1);
      }

      row_count++;
    }
  }
}

QList<QWidget*> NodeParamViewItem::CreateWidget(NodeInput* input)
{
  QList<QWidget*> widgets;

  if (input->inputs().isEmpty()) {
    return widgets;
  }

  switch (input->inputs().first()) {
  case NodeParam::kNone:
  case NodeParam::kAny:
  case NodeParam::kBlock:
  case NodeParam::kTexture:
  case NodeParam::kMatrix:
    break;
  case NodeParam::kInt:
    // FIXME: LabelSlider in INTEGER mode
    break;
  case NodeParam::kFloat:
    // FIXME: LabelSlider in FLOAT mode
    break;
  case NodeParam::kFile:
    // FIXME: File selector
    break;
  case NodeParam::kColor:
    // FIXME: Color selector
    break;
  case NodeParam::kString:
  {
    QLineEdit* line_edit = new QLineEdit();
    widgets.append(line_edit);
  }
    break;
  case NodeParam::kBoolean:
  {
    QCheckBox* check_box = new QCheckBox();
    widgets.append(check_box);
  }
    break;
  case NodeParam::kFont:
  {
    QFontComboBox* font_combobox = new QFontComboBox();
    widgets.append(font_combobox);
  }
    break;
  case NodeParam::kFootage:
  {
    FootageComboBox* footage_combobox = new FootageComboBox();

    // FIXME: Test code
    ProjectPanel* pp = olive::panel_focus_manager->MostRecentlyFocused<ProjectPanel>();
    footage_combobox->SetRoot(pp->project()->root());
    // End test code

    // Pretty hacky way of getting the root folder for this node's sequence
    //const Folder* root_folder = static_cast<const Folder*>(static_cast<Sequence*>(input->parent()->parent())->root());
    //footage_combobox->SetRoot(root_folder);

    widgets.append(footage_combobox);
  }
    break;
  }

  return widgets;
}

void NodeParamViewItem::SetExpanded(bool e)
{
  expanded_ = e;

  contents_->setVisible(e);

  if (expanded_) {
    title_bar_collapse_btn_->setIcon(olive::icon::TriDown);
  } else {
    title_bar_collapse_btn_->setIcon(olive::icon::TriRight);
  }
}

NodeParamViewItemTitleBar::NodeParamViewItemTitleBar(QWidget *parent) :
  QWidget(parent)
{
}

void NodeParamViewItemTitleBar::paintEvent(QPaintEvent *event)
{
  QWidget::paintEvent(event);

  QPainter p(this);

  // Draw bottom border using text color
  p.setPen(palette().text().color());
  p.drawLine(0, height() - 1, width(), height() - 1);
}
