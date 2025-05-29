#include "progressborderwidget.h"
#include <QPainter>

ProgressBorderWidget::ProgressBorderWidget(QWidget *parent)
    : QWidget(parent), m_progress(0) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ProgressBorderWidget::updateProgress);
    // Timer starts only on button click
}

void ProgressBorderWidget::startProgress() {
    m_progress = 0;
    m_timer->start(100); // 100 ms update interval
}

void ProgressBorderWidget::updateProgress() {
    m_progress = (m_progress + 1) % 101;
    update(); // repaint
}

void ProgressBorderWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect = this->rect();
    qreal cornerRadius = 9.0;
    painter.setBrush(QColor("#1f2020"));
    painter.setPen(Qt::NoPen);
    // painter.drawRoundedRect(rect, cornerRadius, cornerRadius);

    qreal width = rect.width();
    qreal height = rect.height();
    qreal perimeter = 2 * (width + height);
    qreal progressLength = (m_progress / 100.0) * perimeter;

    QPointF topLeft = rect.topLeft();
    QPointF topRight = rect.topRight();
    QPointF bottomRight = rect.bottomRight();
    QPointF bottomLeft = rect.bottomLeft();

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

void ProgressBorderWidget::drawPath(QPainter &painter, const QPainterPath &path) {
    QPen pen(Qt::blue, 5);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    painter.drawPath(path);
}
