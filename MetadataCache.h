/*
 *  Copyright (C) 2013-2015 Ofer Kashayov <oferkv@live.com>
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

#include <QMap>
#include <QSet>

class ImageMetadata {
public:
    QSet<QString> tags;
    long orientation;
};

class MetadataCache {
private:
    QMap<QString, ImageMetadata> cache;

public:
    void updateImageTags(const QString &imageFileName, QSet<QString> tags);

    void addTagToImage(const QString &imageFileName, const QString &tagName);

    bool removeTagFromImage(const QString &imageFileName, const QString &tagName);

    void removeImage(const QString &imageFileName);

    QSet<QString> &getImageTags(const QString &imageFileName);

    void setImageTags(const QString &imageFileName, const QSet<QString> &tags);

    void clear();

    bool loadImageMetadata(const QString &imageFullPath);

    long getImageOrientation(const QString &imageFileName);
};
