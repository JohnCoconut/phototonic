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

#include <QDialog>
#include <QLabel>

class ThumbsViewer;

class CopyMoveDialog : public QDialog {
    Q_OBJECT

public slots:

    void abort();

public:
    CopyMoveDialog(QWidget *parent);

    static bool copyOrMoveFile(bool isCopy, const QString &srcFile, const QString &srcPath,
                               QString &dstPath, const QString &dstDir);

    void execute(ThumbsViewer *thumbView, const QString &destDir, bool pasteInCurrDir);

    int nFiles;
    int latestRow;

private:
    QLabel *opLabel;
    QPushButton *cancelButton;
    bool abortOp;
};
