/*
 *  Copyright (C) 2013 Ofer Kashayov <oferkv@live.com>
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

#ifndef IMAGE_VIEWER_H
#define IMAGE_VIEWER_H

#include "MetadataCache.h"

#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMenu>
#include <QPointer>
#include <QScrollArea>

#include <exiv2/exiv2.hpp>

class CropRubberBand;
class ImageWidget;
class Phototonic;

class ImageViewer : public QWidget {
Q_OBJECT

public:
    bool tempDisableResize;
    bool batchMode = false;
    int mirrorLayout;
    QString viewerImageFullPath;
    QMenu *ImagePopUpMenu;
    QScrollArea *scrollArea;
    QLabel *imageInfoLabel;
    CropRubberBand *cropRubberBand;

    enum ZoomMethods {
        Disable = 0,
        WidthAndHeight,
        Width,
        Height,
        Disprop
    };

    enum MirrorLayouts {
        LayNone = 0,
        LayDual,
        LayTriple,
        LayQuad,
        LayVDual
    };

    enum Movement {
        MoveUp = 0,
        MoveDown,
        MoveLeft,
        MoveRight
    };

    ImageViewer(QWidget *parent, const std::shared_ptr<MetadataCache> &metadataCache);

    void loadImage(const QString& imageFileName);

    void clearImage();

    void resizeImage();

    void setCursorHiding(bool hide);

    void refresh();

    void reload();

    int getImageWidthPreCropped() const { return origImage.width(); }

    int getImageHeightPreCropped() const { return origImage.height(); }

    bool isNewImage() const { return newImage; }

    void keyMoveEvent(int direction);

    void rotateByExifRotation(QImage &image, const QString &imageFullPath);

    void setInfo(const QString& infoString);

    void setFeedback(const QString& feedbackString, bool timeLimited = true);

    void setBackgroundColor();

    QPoint getContextMenuPosition() {
        return contextMenuPosition;
    }

signals:
    void toolsUpdated();

public slots:

    void monitorCursorState();

    void saveImage();

    void saveImageAs();

    void copyImage();

    void pasteImage();

    void applyCropAndRotation();

private slots:

    void unsetFeedback();

    void updateRubberBandFeedback(QRect geom);

protected:
    void resizeEvent(QResizeEvent *event);

    void showEvent(QShowEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void contextMenuEvent(QContextMenuEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

private:
    Phototonic *phototonic;
    QLabel *movieWidget = nullptr;
    ImageWidget *imageWidget = nullptr;
    QImage origImage;
    QImage viewerImage;
    QImage mirrorImage;
    QTimer *mouseMovementTimer;
    QPointer<QMovie> animation;
    bool newImage;
    bool cursorIsHidden;
    bool moveImageLocked;
    qreal initialRotation = 0;
    int mouseX;
    int mouseY;
    int layoutX;
    int layoutY;
    QLabel *feedbackLabel;
    QPoint cropOrigin;
    QPoint contextMenuPosition;
    std::shared_ptr<MetadataCache> metadataCache;

    void setMouseMoveData(bool lockMove, int lMouseX, int lMouseY);

    void centerImage(const QSize &imgSize);

    void transform();

    void mirror();

    void colorize();

    void setImage(const QImage &image);
};

#endif // IMAGE_VIEWER_H
