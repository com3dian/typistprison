#include <QFrame>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QDebug>
#include <QPainterPath>

class ImageFrame : public QFrame {
    QPixmap pixmap;
    QVBoxLayout *layout;

public:
    explicit ImageFrame(QWidget *parent = nullptr) : QFrame(parent) {
        setFrameStyle(QFrame::StyledPanel);
        setStyleSheet("background-color: transparent;"
        "border: 2px solid #1F2020;"
        "border-radius: 6px");
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
            painter.setRenderHint(QPainter::Antialiasing, true);
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

            // // Background color fill with rounded corners
            // QPainterPath backgroundPath;
            // backgroundPath.addRoundedRect(this->rect(), 6, 6); // match border radius
            // painter.fillPath(backgroundPath, QColor("#1F2020"));
            
            QPainterPath path;
            path.addRoundedRect(this->rect().adjusted(0, 0, 0, 0), 4, 4);
            painter.setClipPath(path);

            painter.drawPixmap(this->rect().adjusted(6, 6, -6, -6), pixmap);
        }
    }
};