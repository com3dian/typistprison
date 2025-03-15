#include <QFrame>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QDebug>

class ImageFrame : public QFrame {
    QPixmap pixmap;

public:
    explicit ImageFrame(QWidget *parent = nullptr) : QFrame(parent) {
        setFrameStyle(QFrame::StyledPanel);
        setStyleSheet("background-color: transparent;"); // Make the frame background transparent
    }

    void setImage(const QPixmap &image) {
        pixmap = image;
        update();  // Trigger repaint
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        QFrame::paintEvent(event);
        if (!pixmap.isNull()) {
            QPainter painter(this);
            painter.setRenderHint(QPainter::SmoothPixmapTransform);
            painter.drawPixmap(10, 10, pixmap);
        }
    }
};