#include "hovericonbutton.h"

// HoverIconButton implementation
HoverIconButton::HoverIconButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent), iconSize(16, 16) {
    // Set default styles
    setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            border: 1px solid #5A5A5A;
            color: #BDBDBD;
            border-radius: 4px;
            padding: 4px 8px;
        }
        QPushButton:hover {
            background-color: transparent;
            border: 1px solid #5A5A5A;
            color: #FFFFFF;
        }
    )");
}

void HoverIconButton::setIcons(const QString &normalIconPath, const QString &hoverIconPath) {
    normalIcon = QIcon(normalIconPath);
    hoverIcon = QIcon(hoverIconPath);
    
    // Set the initial icon
    setIcon(normalIcon);
    QPushButton::setIconSize(iconSize);
}

void HoverIconButton::setIconSize(const QSize &size) {
    iconSize = size;
    QPushButton::setIconSize(iconSize);
}

void HoverIconButton::enterEvent(QEnterEvent *event) {
    if (!hoverIcon.isNull()) {
        setIcon(hoverIcon);
    }
    QPushButton::enterEvent(event);
}

void HoverIconButton::leaveEvent(QEvent *event) {
    if (!normalIcon.isNull()) {
        setIcon(normalIcon);
    }
    QPushButton::leaveEvent(event);
}