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

#include "Bookmarks.h"
#include "Settings.h"

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QModelIndex>

BookMarks::BookMarks(QWidget *parent)
    : QTreeWidget(parent)
{
    setDragDropMode(QAbstractItemView::DropOnly);

    connect(this, &BookMarks::expanded, this, &BookMarks::resizeTreeColumn);
    connect(this, &BookMarks::collapsed, this, &BookMarks::resizeTreeColumn);

    setColumnCount(1);
    setHeaderHidden(true);
    reloadBookmarks();
}

void BookMarks::reloadBookmarks()
{
    clear();
    for (const auto &itemPath : qAsConst(Settings::bookmarkPaths)) {
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setText(0, QFileInfo(itemPath).fileName());
        item->setIcon(0, QIcon(":/images/bookmarks.png"));
        item->setToolTip(0, itemPath);
        insertTopLevelItem(0, item);
    }
}

void BookMarks::resizeTreeColumn(const QModelIndex &)
{
    resizeColumnToContents(0);
}

void BookMarks::removeBookmark()
{
    if (selectedItems().size() == 1) {
        Settings::bookmarkPaths.remove(selectedItems().at(0)->toolTip(0));
        reloadBookmarks();
    }
}

void BookMarks::dragEnterEvent(QDragEnterEvent *event)
{
    QModelIndexList selectedDirs = selectionModel()->selectedRows();

    if (!selectedDirs.empty()) {
        dndOrigSelection = selectedDirs[0];
    }
    event->acceptProposedAction();
}

void BookMarks::dragMoveEvent(QDragMoveEvent *event)
{
    setCurrentIndex(indexAt(event->pos()));
}

void BookMarks::dropEvent(QDropEvent *event)
{
    if (event->source() != nullptr) {
        QString fileSystemTreeStr(QStringLiteral("FileSystemTree"));
        bool dirOp = (event->source()->metaObject()->className() == fileSystemTreeStr);
        emit dropOp(event->keyboardModifiers(), dirOp,
                    event->mimeData()->urls().at(0).toLocalFile());
    }
}
