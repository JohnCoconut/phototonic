/*
 *  Copyright (C) 2018 Ofer Kashayov <oferkv@live.com>
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

#include <QStandardItemModel>
#include <QTableView>

class ShortcutsTable : public QTableView {
    Q_OBJECT

public:
    ShortcutsTable();

    void addRow(const QString &action, const QString &description, const QString &shortcut);

    void refreshShortcuts();

public slots:
    void setFilter(const QString &filter);

    void showShortcutPopupMenu(const QPoint &point);

    void clearSelectedShortcut();

protected:
    void keyPressEvent(QKeyEvent *keyEvent) override;

private:
    bool confirmOverwriteShortcut(const QString &action, const QString &shortcut);

    QStandardItemModel *keysModel;
    QModelIndex selectedEntry;
    QMenu *shortcutsMenu;
    QAction *clearAction;
    QString shortcutsFilter;
};
