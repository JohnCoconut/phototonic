/*
 *  Copyright (C) 2013-2014 Ofer Kashayov - oferkv@live.com
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

#include "CopyMoveToDialog.h"
#include "Settings.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>

void CopyMoveToDialog::selection(const QItemSelection &, const QItemSelection &)
{
    if (!pathsTable->selectionModel()->selectedRows().empty()) {
        destinationLabel->setText(
            tr("Destination:") + " "
            + pathsTableModel->item(pathsTable->selectionModel()->selectedRows().at(0).row())
                  ->text());
    }
}

void CopyMoveToDialog::pathDoubleClick(const QModelIndex &)
{
    copyOrMove();
}

void CopyMoveToDialog::savePaths()
{
    Settings::bookmarkPaths.clear();
    for (int i = 0; i < pathsTableModel->rowCount(); ++i) {
        Settings::bookmarkPaths.insert(
            pathsTableModel->itemFromIndex(pathsTableModel->index(i, 0))->text());
    }
}

void CopyMoveToDialog::copyOrMove()
{
    savePaths();

    if (QModelIndexList indexesList = pathsTable->selectionModel()->selectedIndexes();
        !indexesList.empty()) {
        selectedPath = pathsTableModel->itemFromIndex(indexesList.first())->text();
        accept();
    } else {
        reject();
    }
}

void CopyMoveToDialog::justClose()
{
    savePaths();
    reject();
}

void CopyMoveToDialog::add()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), currentPath,
                                                        QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
    if (dirName.isEmpty()) {
        return;
    }

    QStandardItem *item = new QStandardItem(QIcon(":/images/bookmarks.png"), dirName);
    pathsTableModel->appendRow(item);

    pathsTable->selectionModel()->clearSelection();
    pathsTable->selectionModel()->select(pathsTableModel->index(pathsTableModel->rowCount() - 1, 0),
                                         QItemSelectionModel::Select);
}

void CopyMoveToDialog::remove()
{
    if (QModelIndexList indexesList = pathsTable->selectionModel()->selectedIndexes();
        !indexesList.empty()) {
        pathsTableModel->removeRow(indexesList.first().row());
    }
}

CopyMoveToDialog::CopyMoveToDialog(QWidget *parent, const QString &thumbsPath, bool move)
    : QDialog(parent)
{
    copyOp = !move;
    if (move) {
        setWindowTitle(tr("Move to..."));
        setWindowIcon(QIcon::fromTheme(QStringLiteral("go-next")));
    } else {
        setWindowTitle(tr("Copy to..."));
        setWindowIcon(QIcon::fromTheme(QStringLiteral("edit-copy")));
    }

    resize(350, 250);
    currentPath = thumbsPath;

    pathsTable = new QTableView(this);
    pathsTable->setSelectionBehavior(QAbstractItemView::SelectItems);
    pathsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    pathsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pathsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pathsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    pathsTableModel = new QStandardItemModel(this);
    pathsTable->setModel(pathsTableModel);
    pathsTable->verticalHeader()->setVisible(false);
    pathsTable->horizontalHeader()->setVisible(false);
    pathsTable->verticalHeader()->setDefaultSectionSize(
        pathsTable->verticalHeader()->minimumSectionSize());
    pathsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pathsTable->setShowGrid(false);

    connect(pathsTable->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            &CopyMoveToDialog::selection);
    connect(pathsTable, &QTableView::doubleClicked, this, &CopyMoveToDialog::pathDoubleClick);

    QHBoxLayout *addRemoveHbox = new QHBoxLayout;
    QPushButton *addButton = new QPushButton(tr("Browse..."));
    connect(addButton, &QPushButton::clicked, this, &CopyMoveToDialog::add);
    QPushButton *removeButton = new QPushButton(tr("Delete Bookmark"));
    connect(removeButton, &QPushButton::clicked, this, &CopyMoveToDialog::remove);
    addRemoveHbox->addWidget(removeButton, 0, Qt::AlignLeft);
    addRemoveHbox->addStretch(1);
    addRemoveHbox->addWidget(addButton, 0, Qt::AlignRight);

    QHBoxLayout *buttonsHbox = new QHBoxLayout;
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(cancelButton, &QPushButton::clicked, this, &CopyMoveToDialog::justClose);

    QPushButton *okButton = new QPushButton(tr("OK"));
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    okButton->setDefault(true);

    connect(okButton, &QPushButton::clicked, this, &CopyMoveToDialog::copyOrMove);

    buttonsHbox->addStretch(1);
    buttonsHbox->addWidget(cancelButton, 0, Qt::AlignRight);
    buttonsHbox->addWidget(okButton, 0, Qt::AlignRight);

    destinationLabel = new QLabel(tr("Destination:"));
    QFrame *line = new QFrame(this);
    line->setObjectName(QStringLiteral("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QVBoxLayout *mainVbox = new QVBoxLayout;
    mainVbox->addWidget(pathsTable);
    mainVbox->addLayout(addRemoveHbox);
    mainVbox->addWidget(line);
    mainVbox->addWidget(destinationLabel);
    mainVbox->addLayout(buttonsHbox);
    setLayout(mainVbox);

    // Load paths list
    for (const auto &bookmarkPath : qAsConst(Settings::bookmarkPaths)) {
        QStandardItem *item = new QStandardItem(QIcon(":/images/bookmarks.png"), bookmarkPath);
        pathsTableModel->appendRow(item);
    }
    pathsTableModel->sort(0);
}
