// imagepopup.h
#ifndef IMAGEPOPUP_H
#define IMAGEPOPUP_H

#include <QWidget>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QFrame>


class ImagePopup : public QWidget {
    Q_OBJECT

public:
    explicit ImagePopup(QWidget *parent = nullptr);
    void showImageAt(const QString &imagePath, QPoint pos);

private:
    QLabel *imageLabel;
    QFrame *frame;
};

#endif // IMAGEPOPUP_H
