#include "progressborderwidget.h"
#include <QPainter>

ProgressBorderWidget::ProgressBorderWidget(QWidget *parent)
    : QWidget(parent),
    timerProgress(0.0),
    isTimerRunning(false),
    isFullScreen(false) {
    prisonerTimer = new QTimer(this);
    connect(prisonerTimer, &QTimer::timeout, this, &ProgressBorderWidget::updateTimerProgress);
    // Timer starts only on button click
}

void ProgressBorderWidget::startTimerProgress(int timeLimit, int wordGoal) {
    timerProgress = 0.0;
    // convert minutes time to seconds
    totalTime = timeLimit * 60 * 10;
    targetWordCount = wordGoal;
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

void ProgressBorderWidget::updateTypingProgress(int wordCount) {
    qreal typingProgress = static_cast<qreal>(wordCount) / targetWordCount;
    update();
    emit needsRepaint();
}

void ProgressBorderWidget::paintBorder(qreal startLength, QPointF startPoint, QRectF borderRect, QPainter painter) {
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
    int innerRadius = 4;

    qreal width = rect.width() - 2 * borderMargin -  2 * innerRadius;
    qreal height = rect.height() - 2 * borderMargin - 2 * innerRadius;
    qreal perimeter = 2 * (width + height);
    qreal progressLength;
    if (totalTime == 0.0){
        progressLength = 0;
    } else {
        progressLength = (timerProgress / totalTime) * perimeter;
    }

    QPointF topLeftArcFrom = rect.topLeft() + QPointF(borderMargin, 2 * borderMargin + innerRadius);
    QPointF topLeftArcTo = rect.topLeft() + QPointF(2 * borderMargin + innerRadius, borderMargin);

    QPointF topRightArcFrom = rect.topRight() + QPointF(- 2 * borderMargin - innerRadius, borderMargin);
    QPointF topRightArcTo = rect.topRight() + QPointF(- borderMargin, 2 * borderMargin + innerRadius);

    QPointF bottomRightArcFrom = rect.bottomRight() + QPointF(-borderMargin, - 2 * borderMargin - innerRadius);
    QPointF bottomRightArcTo = rect.bottomRight() + QPointF(- 2 * borderMargin - innerRadius, -borderMargin);

    QPointF bottomLeftArcFrom = rect.bottomLeft() + QPointF(2 * borderMargin + innerRadius, -borderMargin);
    QPointF bottomLeftArcTo = rect.bottomLeft() + QPointF(borderMargin, - 2 * borderMargin - innerRadius);

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

    qreal cornerArcRadius = borderMargin + innerRadius;
    qreal cornerArcLength = 3.1416 * cornerArcRadius * 0.5;
    QPointF topRightCenter = topRightArcFrom + QPointF(0, cornerArcRadius);
    
    QRectF topRightRect(topRightCenter.x() - cornerArcRadius,
                        topRightCenter.y() - cornerArcRadius,
                        2 * cornerArcRadius,
                        2 * cornerArcRadius);

    if (remaining <= cornerArcLength) {
        path.arcTo(topRightRect, 90.0, - remaining/cornerArcLength * 90.0);
        drawPath(painter, path);
        return;
    } else {
        path.arcTo(topRightRect, 90.0, - 90.0);
        remaining -=  cornerArcLength;
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

    // bottom right corner arc
    QPointF bottomRightCenter = bottomRightArcFrom + QPointF(-cornerArcRadius, 0);
    QRectF bottomRightRect(bottomRightCenter.x() - cornerArcRadius,
                           bottomRightCenter.y() - cornerArcRadius,
                           2 * cornerArcRadius,
                           2 * cornerArcRadius);

    if (remaining <= cornerArcLength) {
        path.arcTo(bottomRightRect, 0.0, - remaining/cornerArcLength * 90.0);
        drawPath(painter, path);
        return;
    } else {
        path.arcTo(bottomRightRect, 0.0, - 90.0);
        remaining -=  cornerArcLength;
    }

    qreal bottomEdge = width - 2 * borderMargin;
    if (remaining <= bottomEdge) {
        path.lineTo(bottomRightArcTo + QPointF(-remaining, 0));
        drawPath(painter, path);
        return;
    } else {
        path.lineTo(bottomLeftArcFrom);
        remaining -= bottomEdge;
    }

    // bottom left corner arc
    QPointF bottomLeftCenter = bottomLeftArcFrom + QPointF(0, -cornerArcRadius);
    QRectF bottomLeftRect(bottomLeftCenter.x() - cornerArcRadius,
                          bottomLeftCenter.y() - cornerArcRadius,
                          2 * cornerArcRadius,
                          2 * cornerArcRadius);

    if (remaining <= cornerArcLength) {
        path.arcTo(bottomLeftRect, -90.0, - remaining/cornerArcLength * 90.0);
        drawPath(painter, path);
        return;
    } else {
        path.arcTo(bottomLeftRect, -90.0, - 90.0);
        remaining -=  cornerArcLength;
    }

    qreal leftEdge = height - 2 * borderMargin;
    if (remaining <= leftEdge) {
        path.lineTo(bottomLeftArcTo - QPointF(0, remaining));
    } else {
        path.lineTo(topLeftArcFrom);
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