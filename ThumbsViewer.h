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

#pragma once

#include "MetadataCache.h"

#include <QBitArray>
#include <QDir>
#include <QFileInfoList>
#include <QListView>
#include <QTimer>

#include <cmath>
#include <exiv2/exiv2.hpp>

class ImagePreview;
class ImageViewer;
class Phototonic;
class QStandardItem;
class QStandardItemModel;

#define BAD_IMAGE_SIZE 64
#define WINDOW_ICON_SIZE 48

class ImageTags;
class InfoView;

struct DuplicateImage
{
    QString filePath;
    int duplicates;
    int id = 0;
};

struct Histogram
{
    float red[256]{};
    float green[256]{};
    float blue[256]{};

    inline float compareChannel(const float hist1[256], const float hist2[256])
    {
        float len1 = 0.F, len2 = 0.F, corr = 0.F;

        for (uint16_t i = 0; i < 256; i++) {
            len1 += hist1[i];
            len2 += hist2[i];
            corr += std::sqrt(hist1[i] * hist2[i]);
        }

        const float part1 = 1.F / std::sqrt(len1 * len2);

        return std::sqrt(1.F - part1 * corr);
    }

    inline float compare(const Histogram &other)
    {
        return compareChannel(red, other.red) + compareChannel(green, other.green)
            + compareChannel(blue, other.blue);
    }
};
Q_DECLARE_METATYPE(Histogram);

class ThumbsViewer : public QListView {
    Q_OBJECT

public:
    enum UserRoles
    {
        FileNameRole = Qt::UserRole + 1,
        SortRole,
        LoadedRole,
        BrightnessRole,
        TypeRole,
        SizeRole,
        TimeRole,
        HistogramRole
    };
    enum ThumbnailLayouts
    {
        Classic,
        Squares,
        Compact
    };

    ThumbsViewer(QWidget *parent, const std::shared_ptr<MetadataCache> &metadataCache);

    void loadPrepare();

    void applyFilter();

    void reLoad();

    void loadDuplicates();

    void loadFileList();

    void loadSubDirectories();

    void setThumbColors();

    bool setCurrentIndexByName(const QString &fileName);

    bool setCurrentIndexByRow(int row);

    void setCurrentRow(int row);

    void setImageViewerWindowTitle();

    void setNeedToScroll(bool needToScroll);

    void selectCurrentIndex();

    QStandardItem *addThumb(const QString &imageFullPath);

    void abort(bool permanent = false);

    void selectThumbByRow(int row);

    void selectByBrightness(qreal min, qreal max);

    [[nodiscard]] int getNextRow() const;

    [[nodiscard]] int getPrevRow() const;

    [[nodiscard]] int getLastRow() const;

    [[nodiscard]] int getRandomRow() const;

    [[nodiscard]] int getCurrentRow() const;

    QStringList getSelectedThumbsList();

    QString getSingleSelectionFilename();

    void setImageViewer(ImageViewer *imageViewer);

    void sortBySimilarity();

    InfoView *infoView;
    ImagePreview *imagePreview;
    ImageTags *imageTags;
    QDir thumbsDir;
    QStringList fileFilters;
    QStandardItemModel *thumbsViewerModel;
    QDir::SortFlags thumbsSortFlags;
    int thumbSize;
    QString filterString;
    bool isBusy;

protected:
    void startDrag(Qt::DropActions) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void initThumbs();

    bool loadThumb(int row);

    void findDupes(bool resetCounters);

    int getFirstVisibleThumb();

    int getLastVisibleThumb();

    void updateThumbsCount();

    void updateFoundDupesState(int duplicates, int filesScanned, int originalImages);

    void updateImageInfoViewer(int row);

    [[nodiscard]] QSize itemSizeHint() const;

    [[nodiscard]] QString thumbnailFileName(const QString &path) const;
    [[nodiscard]] QString locateThumbnail(const QString &path) const;
    void storeThumbnail(const QString &originalPath, QImage thumbnail,
                        const QSize &originalSize) const;

    QFileInfo thumbFileInfo;
    QFileInfoList thumbFileInfoList;
    QList<Histogram> histograms;
    QList<QString> histFiles;
    QPixmap emptyImg;
    QModelIndex currentIndex;
    Phototonic *phototonic;
    std::shared_ptr<MetadataCache> metadataCache;
    ImageViewer *imageViewer;
    QHash<QBitArray, DuplicateImage> dupImageHashes;
    bool isAbortThumbsLoading = false;
    bool isClosing = false;
    bool isNeedToScroll = false;
    int currentRow = 0;
    bool scrolledForward = false;
    int thumbsRangeFirst;
    int thumbsRangeLast;

    QTimer m_selectionChangedTimer;
    QTimer m_loadThumbTimer;

public slots:

    void loadVisibleThumbs(int scrollBarValue = 0);

    void onSelectionChanged();

    void invertSelection();

private slots:

    void loadThumbsRange();

    void loadAllThumbs();
};
