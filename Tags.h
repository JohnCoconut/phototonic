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

#include <QWidget>

#include "MetadataCache.h"

#include <exiv2/exiv2.hpp>

class ThumbsViewer;
class QMenu;
class QTabBar;
class QTreeWidget;
class QTreeWidgetItem;

enum TagsDisplayMode
{
    DirectoryTagsDisplay,
    SelectionTagsDisplay
};

enum TagIcons
{
    TagIconDisabled,
    TagIconEnabled,
    TagIconMultiple,
    TagIconFilterDisabled,
    TagIconFilterEnabled,
    TagIconFilterNegate
};

class ImageTags : public QWidget {
    Q_OBJECT

public:
    ImageTags(QWidget *parent, ThumbsViewer *thumbsViewer,
              const std::shared_ptr<MetadataCache> &metadataCache);

    void addTag(const QString &tagName, bool tagChecked);

    void showTagsFilter();

    void showSelectedImagesTags();

    void resetTagsState();

    bool isImageFilteredOut(const QString &imagePath);

    void removeTag();

    void populateTagsTree();

    QMenu *tagsMenu;
    QTreeWidget *tagsTree;
    bool dirFilteringActive;
    QAction *removeTagAction;
    TagsDisplayMode currentDisplayMode;

private:
    bool writeTagsToImage(const QString &imageFileName, QSet<QString> &tags);

    QSet<QString> getCheckedTags(Qt::CheckState tagState);

    void setTagIcon(QTreeWidgetItem *tagItem, TagIcons icon);

    void setActiveViewMode(TagsDisplayMode mode);

    void applyUserAction(QTreeWidgetItem *item);

    void applyUserAction(const QList<QTreeWidgetItem *> &tagsList);

    void redrawTagTree();

    QSet<QString> imageFilteringTags;
    QAction *actionAddTag;
    QAction *addToSelectionAction;
    QAction *removeFromSelectionAction;
    QAction *actionClearTagsFilter;
    QAction *negateAction;
    QTreeWidgetItem *lastChangedTagItem;
    ThumbsViewer *thumbView;
    QTabBar *tabs;
    std::shared_ptr<MetadataCache> metadataCache;
    bool negateFilterEnabled;

private slots:

    void tagClicked(QTreeWidgetItem *item, int column);

    void saveLastChangedTag(QTreeWidgetItem *item, int column);

    void applyTagFiltering();

    void showMenu(QPoint point);

    void addNewTag();

    void addTagsToSelection();

    void clearTagFilters();

    void negateFilter();

    void removeTagsFromSelection();

    void tabsChanged(int index);

signals:

    void reloadThumbs();
};
