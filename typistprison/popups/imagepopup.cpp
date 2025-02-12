/*
creating popup window display images
*/


#include "imagepopup.h"
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>

ImagePopup::ImagePopup(QWidget *parent) : QWidget(parent) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Create a drop shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(30);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#84e0a5"));
    setGraphicsEffect(shadow);

    // Image Label inside Popup
    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(true);
    imageLabel->setStyleSheet("background-color: white;");

    // Set a minimum size for the popup
    resize(100, 100); // Default size, will adjust based on the image
}


void ImagePopup::showImageAt(const QString &imagePath, QPoint pos) {
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        // Scale the image by 0.05 (5% of its original size) while maintaining aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(pixmap.size() * 0.05, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Set the scaled pixmap to the label
        imageLabel->setPixmap(scaledPixmap);

        // Resize the popup to fit the scaled image
        QSize imageSize = scaledPixmap.size();
        QSize popupSize = imageSize + QSize(40, 40); // Add padding for the shadow and margins
        resize(popupSize);

        // Center the imageLabel within the popup
        imageLabel->setGeometry(
            (popupSize.width() - imageSize.width()) / 2,  // Center horizontally
            (popupSize.height() - imageSize.height()) / 2, // Center vertically
            imageSize.width(),
            imageSize.height()
        );

        // Get the screen geometry to ensure the popup stays within the screen bounds
        QScreen *screen = QGuiApplication::screenAt(pos);
        if (screen) {
            QRect screenGeometry = screen->geometry();

            // Adjust the popup position to ensure it stays within the screen bounds
            QPoint popupPos = pos;
            popupPos.setX(std::max(screenGeometry.left(), std::min(popupPos.x(), screenGeometry.right() - popupSize.width())));
            popupPos.setY(std::max(screenGeometry.top(), std::min(popupPos.y(), screenGeometry.bottom() - popupSize.height())));

            // Move the popup to the adjusted position
            move(popupPos);
        }

        show();
    } else {
        qDebug() << "Failed to load image:" << imagePath;
    }
}