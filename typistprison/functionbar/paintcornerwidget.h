#ifndef PAINTCORNERWIDGET_H
#define PAINTCORNERWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QRect>

class PaintCornerWidget : public QWidget {
    Q_OBJECT
public:
    explicit PaintCornerWidget(QWidget *parent = nullptr) : QWidget(parent) {
        // Set a fixed width (change 150 to your desired width)
        setFixedWidth(24);
    }

protected:
    // Override paintEvent to customize drawing
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // You can add more custom painting code here
        int slant = 2;  // Adjust this to control the slant amount
        int radius = 4;  // Corner radius
        int moveLeft = 3;

        QRect rect = this->rect();
        int xRightTopCorner = rect.left() - slant - moveLeft;
        int xRightBottomCorner = rect.left() + slant - moveLeft;

        int yLeftTopCorner = rect.bottom() - 48 + 9;
        int yLeftBottomCorner = rect.bottom() + 2;

        QPainterPath path;
        path.moveTo(xRightBottomCorner, yLeftBottomCorner);  // Start with rounded bottom-left corner

        path.lineTo(xRightTopCorner - 2.5 * radius, yLeftTopCorner);
        path.quadTo(xRightTopCorner - 1.75 * radius, yLeftTopCorner, xRightTopCorner - 1.25 * radius, yLeftTopCorner + radius);  // Rounded top-right corner

        path.lineTo(xRightBottomCorner + 1.25 * radius, yLeftBottomCorner - radius);
        path.quadTo(xRightBottomCorner + 2 * radius, yLeftBottomCorner, xRightBottomCorner + 3 * radius, yLeftBottomCorner);  // Rounded bottom-right corner

        path.closeSubpath();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#2c2c2c"));  // Background color for selected tab
        painter.drawPath(path);

        // Draw border by setting a pen
        QPen borderPen(QColor("#2c2c2c"), 2); 
        painter.setPen(borderPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(path);
    }
};

#endif // PAINTCORNERWIDGET_H
