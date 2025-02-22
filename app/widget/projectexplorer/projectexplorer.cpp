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

#include "projectexplorer.h"

#include <QDebug>
#include <QVBoxLayout>

#include "projectexplorerdefines.h"

ProjectExplorer::ProjectExplorer(QWidget *parent) :
  QWidget(parent),
  view_type_(olive::TreeView),
  model_(this)
{
  // Create layout
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setSpacing(0);
  layout->setMargin(0);

  // Set up navigation bar
  nav_bar_ = new ProjectExplorerNavigation(this);
  connect(nav_bar_, SIGNAL(SizeChanged(int)), this, SLOT(SizeChangedSlot(int)));
  connect(nav_bar_, SIGNAL(DirectoryUpClicked()), this, SLOT(DirUpSlot()));
  layout->addWidget(nav_bar_);

  // Set up stacked widget
  stacked_widget_ = new QStackedWidget(this);
  layout->addWidget(stacked_widget_);

  // Add tree view to stacked widget
  tree_view_ = new ProjectExplorerTreeView(stacked_widget_);
  AddView(tree_view_);

  // Add list view to stacked widget
  list_view_ = new ProjectExplorerListView(stacked_widget_);
  AddView(list_view_);

  // Add icon view to stacked widget
  icon_view_ = new ProjectExplorerIconView(stacked_widget_);
  AddView(icon_view_);

  // Set default view to tree view
  set_view_type(olive::TreeView);

  // Set default icon size
  SizeChangedSlot(olive::kProjectIconSizeDefault);

  // Set rename timer timeout
  rename_timer_.setInterval(500);
  connect(&rename_timer_, SIGNAL(timeout()), this, SLOT(RenameTimerSlot()));
}

const olive::ProjectViewType &ProjectExplorer::view_type()
{
  return view_type_;
}

void ProjectExplorer::set_view_type(olive::ProjectViewType type)
{
  view_type_ = type;

  // Set widget based on view type
  switch (view_type_) {
  case olive::TreeView:
    stacked_widget_->setCurrentWidget(tree_view_);
    nav_bar_->setVisible(false);
    break;
  case olive::ListView:
    stacked_widget_->setCurrentWidget(list_view_);
    nav_bar_->setVisible(true);
    break;
  case olive::IconView:
    stacked_widget_->setCurrentWidget(icon_view_);
    nav_bar_->setVisible(true);
    break;
  }
}

void ProjectExplorer::Edit(Item *item)
{
  CurrentView()->edit(model_.CreateIndexFromItem(item));
}

void ProjectExplorer::AddView(QAbstractItemView *view)
{
  view->setModel(&model_);
  view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(view, SIGNAL(DoubleClickedView(const QModelIndex&)), this, SLOT(DoubleClickViewSlot(const QModelIndex&)));
  connect(view, SIGNAL(clicked(const QModelIndex&)), this, SLOT(ItemClickedSlot(const QModelIndex&)));
  stacked_widget_->addWidget(view);
}

void ProjectExplorer::BrowseToFolder(const QModelIndex &index)
{
  // Make sure any rename timers are stopped
  rename_timer_.stop();

  // Set appropriate views to this index
  icon_view_->setRootIndex(index);
  list_view_->setRootIndex(index);

  // Set navbar text to folder's name
  if (index.isValid()) {
    Folder* f = static_cast<Folder*>(index.internalPointer());
    nav_bar_->set_text(f->name());
  } else {
    // Or set it to an empty string if the index is valid (which means we're browsing to the root directory)
    nav_bar_->set_text(QString());
  }

  // Set directory up enabled button based on whether we're in root or not
  nav_bar_->set_dir_up_enabled(index.isValid());
}

QAbstractItemView *ProjectExplorer::CurrentView()
{
  return static_cast<QAbstractItemView*>(stacked_widget_->currentWidget());
}

void ProjectExplorer::ItemClickedSlot(const QModelIndex &index)
{
  if (index.isValid()) {
    if (clicked_index_ == index) {
      // The item has been clicked more than once, start a timer for renaming
      rename_timer_.start();
    } else {
      // Cache this index for the next click
      clicked_index_ = index;

      // If the rename timer had started, stop it now
      rename_timer_.stop();
    }
  } else {
    // Stop the rename timer
    rename_timer_.stop();
  }
}

void ProjectExplorer::DoubleClickViewSlot(const QModelIndex &index)
{
  if (index.isValid()) {

    // Retrieve source item from index
    Item* i = static_cast<Item*>(index.internalPointer());

    // If the item is a folder, browse to it
    if (i->CanHaveChildren()
        && (view_type() == olive::ListView || view_type() == olive::IconView)) {

      BrowseToFolder(index);

    }

    // Emit a signal
    emit DoubleClickedItem(i);

  } else {

    // Emit nullptr since no item was actually clicked on
    emit DoubleClickedItem(nullptr);

  }
}

void ProjectExplorer::SizeChangedSlot(int s)
{
  icon_view_->setGridSize(QSize(s, s));

  list_view_->setIconSize(QSize(s, s));
}

void ProjectExplorer::DirUpSlot()
{
  QModelIndex current_root = icon_view_->rootIndex();

  if (current_root.isValid()) {
    QModelIndex parent = current_root.parent();

    BrowseToFolder(parent);
  }
}

void ProjectExplorer::RenameTimerSlot()
{
  // Start editing this index
  CurrentView()->edit(clicked_index_);

  // Reset clicked index state
  clicked_index_ = QModelIndex();

  // Stop rename timer
  rename_timer_.stop();
}

Project *ProjectExplorer::project()
{
  return model_.project();
}

void ProjectExplorer::set_project(Project *p)
{
  model_.set_project(p);
}

QList<Item *> ProjectExplorer::SelectedItems()
{
  // Determine which view is active and get its selected indexes
  QModelIndexList index_list = CurrentView()->selectionModel()->selectedRows();

  // Convert indexes to item objects
  QList<Item*> selected_items;

  for (int i=0;i<index_list.size();i++) {
    const QModelIndex& index = index_list.at(i);

    Item* item = static_cast<Item*>(index.internalPointer());

    selected_items.append(item);
  }

  return selected_items;
}

Folder *ProjectExplorer::GetSelectedFolder()
{
  if (project() == nullptr) {
    return nullptr;
  }

  Folder* folder = nullptr;

  // Get the selected items from the panel
  QList<Item*> selected_items = SelectedItems();

  // Heuristic for finding the selected folder:
  //
  // - If `folder` is nullptr, we set the first folder we find. Either the item itself if it's a folder, or the
  //   item's parent.
  // - Otherwise, if all folders found are the same, we'll use that to import into.
  // - If more than one folder is found, we play it safe and import into the root folder

  for (int i=0;i<selected_items.size();i++) {
    Item* sel_item = selected_items.at(i);

    // If this item is not a folder, presumably it's parent is
    if (!sel_item->CanHaveChildren()) {
      sel_item = sel_item->parent();

      Q_ASSERT(sel_item->CanHaveChildren());
    }

    if (folder == nullptr) {
      // If the folder is nullptr, cache it as this folder
      folder = static_cast<Folder*>(sel_item);
    } else if (folder != sel_item) {
      // If not, we've already cached a folder so we check if it's the same
      // If it isn't, we "play it safe" and use the root folder
      folder = nullptr;
      break;
    }
  }

  // If we didn't pick up a folder from the heuristic above for whatever reason, use root
  if (folder == nullptr) {
    folder = project()->root();
  }

  return folder;
}

ProjectViewModel *ProjectExplorer::model()
{
  return &model_;
}
