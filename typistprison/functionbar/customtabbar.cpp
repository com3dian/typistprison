#include "customtabbar.h"

CustomTabBar::CustomTabBar(QWidget *parent) : QTabBar(parent) {

    this->setContentsMargins(0, 0, 0, 0);
    this->setStyleSheet("QTabBar::tab { margin: 0px; padding: 0px; }");

    //this->setIconSize(QSize(24, 24));

    QTimer::singleShot(0, this, SLOT(customizeScrollButtons()));
}

void CustomTabBar::customizeScrollButtons() {
    // In your environment the buttons have object names "ScrollLeftButton" and "ScrollRightButton"
    QToolButton *leftButton = findChild<QToolButton*>("ScrollLeftButton");
    QToolButton *rightButton = findChild<QToolButton*>("ScrollRightButton");

    QString noStyle = "QToolButton { border: none; background-color: transparent; } "
                  "QToolButton:hover { background-color: transparent; }";

    if (leftButton) {
        leftButton->setIconSize(QSize(0, 0));
        leftButton->setFixedSize(0, 0);
        leftButton->setStyleSheet(noStyle);
    }
    if (rightButton) {
        rightButton->setIconSize(QSize(0, 0));
        rightButton->setFixedSize(QSize(0, 0));
        rightButton->setStyleSheet(noStyle);
    }
}

void CustomTabBar::paintEvent(QPaintEvent *event) {
    QStylePainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // Smoother edges

    for (int i = 0; i < count(); ++i) {
        QStyleOptionTab opt;
        initStyleOption(&opt, i);

        QRect rect = tabRect(i);
        QPainterPath path;

        if (i == currentIndex()) {

            int slant = 2;  // Adjust this to control the slant amount
            int radius = 4;  // Corner radius
            int moveLeft = 2;

            int xLeftBottomCorner = rect.left() - 2*radius;
            int yLeftBottomCorner = rect.bottom() + 2;

            int xLeftTopCorner = rect.left();
            int yLeftTopCorner = rect.top() + 9;

            int xRightTopCorner = rect.right() - slant - moveLeft;
            int xRightBottomCorner = rect.right() + slant - moveLeft;
            
            if (i == count() - 1) {
                emit lastTabFocus();
            } else {
                emit lastTabNoFocus();
            }

            if (i == 0) {
                emit firstTabFocus();
            } else {
                emit firstTabNoFocus();
            }

            QPainterPath path;
            path.moveTo(xLeftBottomCorner, yLeftBottomCorner);  // Start with rounded bottom-left corner
            path.quadTo(xLeftTopCorner, yLeftBottomCorner, xLeftTopCorner, yLeftBottomCorner - 2*radius);  // Rounded corner

            path.lineTo(xLeftTopCorner, yLeftTopCorner + radius);
            path.quadTo(xLeftTopCorner, yLeftTopCorner, xLeftTopCorner + radius, yLeftTopCorner);  // Rounded corner

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
        } else {
            // Draw normal tab as a rectangle
            painter.fillRect(rect, QColor("transparent"));

            if (not ( i == (count() - 1) or i == (currentIndex() - 1) )) {
                // Draw a white vertical line at the right border
                painter.fillRect(QRect(rect.right() - 2, rect.top() + 16, 2, rect.height() - 28), QColor("#3A3B3B"));
            }
            
        }

        // Draw the tab text within the tab boundaries
        painter.setPen(QColor("#BDBDBD"));
        painter.drawText(rect.adjusted(16, 2, -16, 0), Qt::AlignLeft | Qt::AlignVCenter, tabText(i));
    }
}