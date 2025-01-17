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

#include "ShortcutsTable.h"
#include "MessageBox.h"
#include "Settings.h"

#include <QHeaderView>
#include <QKeyEvent>
#include <QMenu>

ShortcutsTable::ShortcutsTable()
{
    keysModel = new QStandardItemModel(this);
    setModel(keysModel);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    verticalHeader()->hide();
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(verticalHeader()->minimumSectionSize());
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    horizontalHeader()->setHighlightSections(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setColumnHidden(2, true);

    shortcutsMenu = new QMenu(QLatin1String(""));
    clearAction = new QAction(tr("Delete shortcut"), this);
    connect(clearAction, &QAction::triggered, this, &ShortcutsTable::clearSelectedShortcut);
    shortcutsMenu->addAction(clearAction);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &ShortcutsTable::customContextMenuRequested, this,
            &ShortcutsTable::showShortcutPopupMenu);
    shortcutsFilter.clear();
}

void ShortcutsTable::addRow(const QString &action, const QString &description,
                            const QString &shortcut)
{
    keysModel->appendRow(QList<QStandardItem *>()
                         << new QStandardItem(description) << new QStandardItem(shortcut)
                         << new QStandardItem(action));
}

void ShortcutsTable::keyPressEvent(QKeyEvent *keyEvent)
{
    if (!this->selectedIndexes().count()) {
        return;
    }
    QString keySequenceText;
    QString keyText(QLatin1String(""));
    QString modifierText(QLatin1String(""));

    if (keyEvent->modifiers() & Qt::ShiftModifier)
        modifierText += QLatin1String("Shift+");
    if (keyEvent->modifiers() & Qt::ControlModifier)
        modifierText += QLatin1String("Ctrl+");
    if (keyEvent->modifiers() & Qt::AltModifier)
        modifierText += QLatin1String("Alt+");

    if ((keyEvent->key() >= Qt::Key_Shift && keyEvent->key() <= Qt::Key_ScrollLock)
        || (keyEvent->key() >= Qt::Key_Super_L && keyEvent->key() <= Qt::Key_Direction_R)
        || keyEvent->key() == Qt::Key_AltGr || keyEvent->key() < 0) {
        return;
    }

    keyText = QKeySequence(keyEvent->key()).toString();
    keySequenceText = modifierText + keyText;

    if ((keyEvent->modifiers() & Qt::AltModifier)
        && (keyEvent->key() > Qt::Key_0 && keyEvent->key() <= Qt::Key_Colon)) {
        MessageBox msgBox(this);
        msgBox.warning(
            tr("Set shortcut"),
            tr("%1 is reserved for launching external applications.").arg(keySequenceText));
        return;
    }

    QMapIterator<QString, QAction *> keysIterator(Settings::actionKeys);
    bool needToRefreshShortCuts = false;
    while (keysIterator.hasNext()) {
        keysIterator.next();

        QAction tmpAction(this);
        tmpAction.setShortcut(keySequenceText);
        if (keysIterator.value()->shortcut().toString() == tmpAction.shortcut().toString()) {
            if (!confirmOverwriteShortcut(keysIterator.value()->text(), keySequenceText)) {
                return;
            }
            Settings::actionKeys.value(keysIterator.key())->setShortcut(QKeySequence());
            needToRefreshShortCuts = true;
        }
    }

    int row = selectedIndexes().first().row();
    keysModel->item(row, 1)->setText(keySequenceText);
    Settings::actionKeys.value(keysModel->item(row, 2)->text())
        ->setShortcut(QKeySequence(keySequenceText));
    if (needToRefreshShortCuts) {
        refreshShortcuts();
    }
}

bool ShortcutsTable::confirmOverwriteShortcut(const QString &action, const QString &shortcut)
{
    MessageBox msgBox(this);
    msgBox.setText(tr("%1 is already assigned to %2, reassign shortcut?").arg(shortcut, action));
    msgBox.setWindowTitle(tr("Overwrite Shortcut"));
    msgBox.setIcon(MessageBox::Warning);
    msgBox.setStandardButtons(MessageBox::Yes | MessageBox::Cancel);
    msgBox.setDefaultButton(MessageBox::Cancel);
    msgBox.setButtonText(MessageBox::Yes, tr("Yes"));
    msgBox.setButtonText(MessageBox::Cancel, tr("Cancel"));

    return (msgBox.exec() == MessageBox::Yes);
}

void ShortcutsTable::clearSelectedShortcut()
{
    if (selectedEntry.isValid()) {
        QStandardItemModel *itemsModel = (QStandardItemModel *)model();
        itemsModel->item(selectedEntry.row(), 1)->setText(QLatin1String(""));
        Settings::actionKeys.value(itemsModel->item(selectedEntry.row(), 2)->text())
            ->setShortcut(QKeySequence(QLatin1String("")));
    }
}

void ShortcutsTable::showShortcutPopupMenu(const QPoint &point)
{
    selectedEntry = indexAt(point);
    if (selectedEntry.isValid())
        shortcutsMenu->popup(viewport()->mapToGlobal(point));
}

void ShortcutsTable::setFilter(const QString &filter)
{
    this->shortcutsFilter = filter;
    refreshShortcuts();
}

void ShortcutsTable::refreshShortcuts()
{
    keysModel->clear();
    keysModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Action")));
    keysModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Shortcut")));
    QMapIterator<QString, QAction *> it(Settings::actionKeys);
    while (it.hasNext()) {
        it.next();
        if (!shortcutsFilter.isEmpty()
            && !Settings::actionKeys.value(it.key())->text().toLower().contains(
                shortcutsFilter.toLower())) {
            continue;
        }
        addRow(it.key(), Settings::actionKeys.value(it.key())->text(),
               Settings::actionKeys.value(it.key())->shortcut().toString());
    }
    setColumnHidden(2, true);
}
