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

#include <QStandardItemModel>
#include <QTableView>
#include <QWidget>

class InfoView : public QWidget {
    Q_OBJECT

public:
    InfoView(QWidget *parent);

    void clear();

    void addEntry(const QString &key, const QString &value);

    void addTitleEntry(const QString &title);

signals:

    void updateInfo();

public slots:

    void showInfoViewMenu(const QPoint &pt);

    void copyEntry();

protected:
    void showEvent(QShowEvent *event) override;

private slots:

    void filterItems();

private:
    QTableView *infoViewerTable;
    QStandardItemModel *imageInfoModel;
    QModelIndex selectedEntry;
    QMenu *infoMenu;
    QLineEdit *filterLineEdit;
};
