#include "progressborderwidget.h"
#include <QPainter>

ProgressBorderWidget::ProgressBorderWidget(QWidget *parent)
    : QWidget(parent), timerProgress(0), isTimerRunning(false), isFullScreen(false) {
    prisonerTimer = new QTimer(this);
    connect(prisonerTimer, &QTimer::timeout, this, &ProgressBorderWidget::updateTimerProgress);
    // Timer starts only on button click
}

void ProgressBorderWidget::startTimerProgress(qreal inputTotalTime) {
    timerProgress = 0;
    // convert minutes time to seconds
    totalTime = inputTotalTime * 60 * 10;
    prisonerTimer->start(100); // update interval
    isTimerRunning = true;
}

void ProgressBorderWidget::clearTimerProgress() {
    prisonerTimer->stop();
    timerProgress = 0;
    update(); // repaint
    emit needsRepaint();
    isTimerRunning = false;
}

void ProgressBorderWidget::updateTimerProgress() {
    timerProgress = timerProgress + 1;
    update(); // repaint
    emit needsRepaint();
}

void ProgressBorderWidget::updateTypingProgress(qreal progress) {
    qreal typingProgress = progress;
    update();
    emit needsRepaint();
}

void ProgressBorderWidget::paintBorder(qreal startLength, QRectF borderRect, QPainter painter) {
    qreal width = borderRect.width();
    qreal height = borderRect.height();
    qreal perimeter = 2 * (width + height);
    qreal progressLength;
    if (totalTime == 0.0){
        progressLength = 0;
    } else {
        progressLength = (timerProgress / totalTime) * perimeter;
    }
    QPointF topLeft = borderRect.topLeft() + QPointF(4, 4);
    QPointF topRight = borderRect.topRight() + QPointF(-4, 4);
    QPointF bottomRight = borderRect.bottomRight() + QPointF(-4, -4);
    QPointF bottomLeft = borderRect.bottomLeft() + QPointF(4, -4);

    QPainterPath path;
    path.moveTo(topLeft);

    qreal remaining = progressLength;

    qreal topEdge = width;
    if (remaining <= topEdge) {
        path.lineTo(topLeft + QPointF(remaining, 0));
        drawPath(painter, path);
        return;
    } else {
        path.lineTo(topRight);
        remaining -= topEdge;
    }

    qreal rightEdge = height;
    if (remaining <= rightEdge) {
        path.lineTo(topRight + QPointF(0, remaining));
        drawPath(painter, path);
        return;
    } else {
        path.lineTo(bottomRight);
        remaining -= rightEdge;
    }

    qreal bottomEdge = width;
    if (remaining <= bottomEdge) {
        path.lineTo(bottomRight - QPointF(remaining, 0));
        drawPath(painter, path);
        return;
    } else {
        path.lineTo(bottomLeft);
        remaining -= bottomEdge;
    }

    qreal leftEdge = height;
    if (remaining <= leftEdge) {
        path.lineTo(bottomLeft - QPointF(0, remaining));
    } else {
        path.lineTo(topLeft);
    }

    drawPath(painter, path);
}

void ProgressBorderWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect = this->rect();
    qreal cornerRadius = 9.0;
    QPen borderPen; // Declare borderPen once
    if (isFullScreen) {
        borderPen.setColor(QColor("#000000")); // Set color for fullscreen
        painter.setBrush(QColor("#000000"));
        painter.setPen(borderPen); // Set the pen for the border
        painter.drawRoundedRect(rect, 0, 0);
    } else {
        borderPen.setColor(QColor("#393B3B")); // Set color for non-fullscreen
    }
    painter.setBrush(QColor("#1f2020"));
    borderPen.setWidth(1);
    painter.setPen(borderPen); // Set the pen for the border
    painter.drawRoundedRect(rect, cornerRadius, cornerRadius);

    int borderMargin = 4;
    int innerRadius = 6;

    qreal width = rect.width() - 2 * borderMargin -  2 * innerRadius;
    qreal height = rect.height() - 2 * borderMargin - 2 * innerRadius;
    qreal perimeter = 2 * (width + height);
    qreal progressLength;
    if (totalTime == 0.0){
        progressLength = 0;
    } else {
        progressLength = (timerProgress / totalTime) * perimeter;
    }

    QPointF topLeftArcTo = rect.topLeft() + QPointF(0.5, 0.5) +QPointF(2 * borderMargin + innerRadius, borderMargin);

    QPointF topRightArcFrom = rect.topRight() + QPointF(- 0.5, 0.5) + QPointF(- 2 * borderMargin - innerRadius, borderMargin);
    QPointF topRightArcTo = rect.topRight() + QPointF(- 0.5, 0.5) + QPointF(- borderMargin, 2 * borderMargin + innerRadius);

    QPointF bottomRightArcFrom = rect.bottomRight() + QPointF(- 0.5, - 0.5) + QPointF(-borderMargin, - 2 * borderMargin - innerRadius);
    QPointF bottomRightArcTo = rect.bottomRight() + QPointF(- 0.5, - 0.5) + QPointF(- 2 * borderMargin - innerRadius, -borderMargin);

    QPointF bottomLeft = rect.bottomLeft() + QPointF(borderMargin, -borderMargin);

    QPainterPath path;
    path.moveTo(topLeftArcTo);

    qreal remaining = progressLength;

    qreal topEdge = width - 2 * borderMargin;
    if (remaining <= topEdge) {
        path.lineTo(topLeftArcTo + QPointF(remaining, 0));
        drawPath(painter, path);
        return;
    } else {
        path.lineTo(topRightArcFrom);
        remaining -= topEdge;
    }

    qreal topRightArcRadius = borderMargin + innerRadius;
    qreal topRightArcLength = 3.1416 * topRightArcRadius * 0.5;
    QPointF topRightCenter = topRightArcFrom + QPointF(0, topRightArcRadius);
    qDebug() << "topRightCenter: " << topRightCenter;
    
    QRectF topRightRect(topRightCenter.x() - topRightArcRadius, topRightCenter.y() - topRightArcRadius, 2 * topRightArcRadius, 2 * topRightArcRadius);

    if (remaining <= topRightArcLength) {
        path.arcTo(topRightRect, 90.0, - remaining/topRightArcLength * 90.0);
        drawPath(painter, path);
        return;
    } else {
        path.arcTo(topRightRect, 90.0, - 90.0);
        remaining -=  topRightArcLength;
    }

    qreal rightEdge = height - 2 * borderMargin;
    if (remaining <= rightEdge) {        
        path.lineTo(topRightArcTo + QPointF(0, remaining));
        drawPath(painter, path);
        return;
    } else {
        path.lineTo(bottomRightArcFrom);
        remaining -= rightEdge;
    }

    qreal bottomEdge = width;
    if (remaining <= bottomEdge) {
        path.lineTo(bottomRightArcFrom - QPointF(remaining, 0));
        drawPath(painter, path);
        return;
    } else {
        path.lineTo(bottomLeft);
        remaining -= bottomEdge;
    }

    qreal leftEdge = height;
    if (remaining <= leftEdge) {
        path.lineTo(bottomLeft - QPointF(0, remaining));
    } else {
        path.lineTo(topLeftArcTo);
    }

    drawPath(painter, path);
}

void ProgressBorderWidget::drawPath(QPainter &painter, const QPainterPath &path) {
    QPen pen(QColor("#ACD3A8"), 8);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawPath(path);
}

void ProgressBorderWidget::drawPathTest(QPainter &painter, const QPainterPath &path) {
    QPen pen(QColor("#5652d1"), 8);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawPath(path);
}


void ProgressBorderWidget::setFullScreen(bool fullScreen) {
    isFullScreen = fullScreen;
    update();
    emit needsRepaint();
}