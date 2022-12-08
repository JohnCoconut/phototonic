/*
 *  Copyright (C) 2013-2014 Ofer Kashayov <oferkv@live.com>
 *  This file is part of Phototonic Image Viewer.
 *
 *  Phototonic is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Phototonic is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Phototonic.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FileSystemTree.h"
#include "FileSystemModel.h"
#include "Settings.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

FileSystemTree::FileSystemTree(QWidget *parent)
    : QTreeView(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    fileSystemModel = new FileSystemModel();
    fileSystemModel->setRootPath(QLatin1String(""));
    setModelFlags();
    setModel(fileSystemModel);

    for (int i = 1; i <= 3; ++i) {
        hideColumn(i);
    }
    setHeaderHidden(true);

    connect(
        fileSystemModel, &QFileSystemModel::layoutChanged, this,
        [this]() { scrollTo(currentIndex()); }, Qt::QueuedConnection);

    connect(this, &FileSystemTree::expanded, this, &FileSystemTree::resizeTreeColumn);
    connect(this, &FileSystemTree::collapsed, this, &FileSystemTree::resizeTreeColumn);
}

QModelIndex FileSystemTree::getCurrentIndex()
{
    return selectedIndexes().first();
}

void FileSystemTree::resizeTreeColumn(const QModelIndex &index)
{
    resizeColumnToContents(0);
    scrollTo(index);
}

void FileSystemTree::dragEnterEvent(QDragEnterEvent *event)
{
    QModelIndexList selectedDirs = selectionModel()->selectedRows();
    if (!selectedDirs.empty()) {
        dndOrigSelection = selectedDirs[0];
        event->acceptProposedAction();
    }
}

void FileSystemTree::dragMoveEvent(QDragMoveEvent *event)
{
    setCurrentIndex(indexAt(event->pos()));
}

void FileSystemTree::dropEvent(QDropEvent *event)
{
    if (event->source() != nullptr) {
        QString fileSystemTreeStr = QStringLiteral("FileSystemTree");
        bool dirOp = (event->source()->metaObject()->className() == fileSystemTreeStr);
        emit dropOp(event->keyboardModifiers(), dirOp,
                    event->mimeData()->urls().at(0).toLocalFile());
        setCurrentIndex(dndOrigSelection);
    }
}

void FileSystemTree::setModelFlags()
{
    fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    if (Settings::showHiddenFiles) {
        fileSystemModel->setFilter(fileSystemModel->filter() | QDir::Hidden);
    }
}
