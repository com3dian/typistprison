/*
    Creating a popup window to display images with a surrounding shadow
*/

#include "imagepopup.h"
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QGraphicsDropShadowEffect>

ImagePopup::ImagePopup(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Image Label inside the Popup
    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("background-color: transparent;");
}

void ImagePopup::showImageAt(const QString &imagePath, QPoint pos) {
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        // Scale the image by 0.2 (20% of its original size) while maintaining aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(pixmap.size() * 0.2, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Set the scaled pixmap to the label
        imageLabel->setPixmap(scaledPixmap);

        // Resize the popup to fit the scaled image with shadow padding
        QSize imageSize = scaledPixmap.size();
        QSize popupSize = imageSize + QSize(20, 20); // Extra padding for the shadow
        resize(popupSize);
        imageLabel->setGeometry(10, 10, imageSize.width(), imageSize.height());

        // Adjust position relative to button or reference point
        QPoint popupPos = this->mapFromGlobal(pos);
        qDebug() << "popupPos:              " << popupPos;
        move(popupPos);

        // Create and configure the drop shadow effect
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(24);   // Softer, larger shadow
        shadow->setColor(QColor("#1F2020")); // Shadow color
        shadow->setOffset(0, 0);     // Zero offset for uniform shadow

        // Apply the effect to the popup
        setGraphicsEffect(shadow);

        show();
    } else {
        qDebug() << "Failed to load image:" << imagePath;
    }
}
