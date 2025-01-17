/*
 *  Copyright (C) 2013-2018 Ofer Kashayov <oferkv@live.com>
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

#include "RenameDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

RenameDialog::RenameDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Rename Image"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    QPushButton *okButton = new QPushButton(tr("Rename"));
    connect(okButton, &QPushButton::clicked, this, &RenameDialog::ok);
    okButton->setDefault(true);

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, &QPushButton::clicked, this, &RenameDialog::abort);
    buttonsLayout->addWidget(cancelButton, 1, Qt::AlignRight);
    buttonsLayout->addWidget(okButton, 0, Qt::AlignRight);

    QHBoxLayout *renameLayout = new QHBoxLayout;
    QLabel *label = new QLabel(tr("New name: "));
    fileNameLineEdit = new QLineEdit();
    fileNameLineEdit->setMinimumWidth(200);
    renameLayout->addWidget(label);
    renameLayout->addWidget(fileNameLineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(renameLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
    setWindowIcon(QIcon(":/images/phototonic.png"));
    setMinimumWidth(480);
}

void RenameDialog::ok()
{
    accept();
}

void RenameDialog::abort()
{
    reject();
}

void RenameDialog::setFileName(const QString &name)
{
    fileNameLineEdit->setText(name);
    fileNameLineEdit->setSelection(0, name.lastIndexOf('.'));
}
