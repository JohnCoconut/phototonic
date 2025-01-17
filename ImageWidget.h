/*
 *  Copyright (C) 2018 Shawn Rutledge <s@ecloud.org>
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

class ImageWidget : public QWidget {
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);

    bool empty() { return m_image.isNull(); }

    QImage image() { return m_image; }

    void setImage(const QImage &i);

    [[nodiscard]] qreal rotation() const { return m_rotation; }

    void setRotation(qreal r);

    QPoint mapToImage(QPoint p);

    [[nodiscard]] QSize imageSize() const { return m_image.size(); }

protected:
    [[nodiscard]] QSize sizeHint() const override { return m_image.size(); }

    void paintEvent(QPaintEvent *event) override;

private:
    QImage m_image;
    qreal m_rotation = 0;
};
