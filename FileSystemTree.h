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

#pragma once

#include <QTreeView>

class FileSystemModel;

class FileSystemTree : public QTreeView {
    Q_OBJECT

public:
    FileSystemTree(QWidget *parent);

    FileSystemModel *fileSystemModel;

    QModelIndex getCurrentIndex();

    void setModelFlags();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dragMoveEvent(QDragMoveEvent *event) override;

    void dropEvent(QDropEvent *event) override;

signals:

    void dropOp(Qt::KeyboardModifiers keyMods, bool dirOp, QString cpMvDirPath);

private:
    QModelIndex dndOrigSelection;

private slots:

    void resizeTreeColumn(const QModelIndex &);
};
