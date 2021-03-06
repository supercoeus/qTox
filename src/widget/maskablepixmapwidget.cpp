/*
    Copyright © 2014-2015 by The qTox Project

    This file is part of qTox, a Qt-based graphical interface for Tox.

    qTox is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qTox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qTox.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "maskablepixmapwidget.h"
#include <QPainter>

MaskablePixmapWidget::MaskablePixmapWidget(QWidget *parent, QSize size, QString maskName)
    : QWidget(parent)
    , maskName(maskName)
    , clickable(false)
{
    setSize(size);
}

MaskablePixmapWidget::~MaskablePixmapWidget()
{
    delete renderTarget;
}

void MaskablePixmapWidget::setClickable(bool clickable)
{
    this->clickable = clickable;

    if (clickable)
        setCursor(Qt::PointingHandCursor);
    else
        unsetCursor();
}

void MaskablePixmapWidget::setPixmap(const QPixmap &pmap)
{
    if (!pmap.isNull())
    {
        unscaled = pmap;
        pixmap = pmap.scaled(width() - 2, height() - 2, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        update();
    }
}

QPixmap MaskablePixmapWidget::getPixmap() const
{
    return *renderTarget;
}

void MaskablePixmapWidget::setSize(QSize size)
{
    setFixedSize(size);
    delete renderTarget;
    renderTarget = new QPixmap(size);

    QPixmap pmapMask = QPixmap(maskName);
    if (!pmapMask.isNull())
        mask = pmapMask.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    if (!unscaled.isNull())
    {
        pixmap = unscaled.scaled(width() - 2, height() - 2, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        update();
    }
}

void MaskablePixmapWidget::paintEvent(QPaintEvent *)
{
    renderTarget->fill(Qt::transparent);

    QPoint offset((width() - pixmap.size().width())/2,(height() - pixmap.size().height())/2); // centering the pixmap

    QPainter painter(renderTarget);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(offset,pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawPixmap(0,0,mask);
    painter.end();

    painter.begin(this);
    painter.drawPixmap(0,0,*renderTarget);
}

void MaskablePixmapWidget::mousePressEvent(QMouseEvent*)
{
    if (clickable)
        emit clicked();
}
