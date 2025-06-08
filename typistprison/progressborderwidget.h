#ifndef PROGRESSBORDERWIDGET_H
#define PROGRESSBORDERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainterPath>

class ProgressBorderWidget : public QWidget {
    Q_OBJECT

public:
    explicit ProgressBorderWidget(QWidget *parent = nullptr);
    void startTimerProgress(qreal totalTime);
    void clearTimerProgress();
    void setFullScreen(bool fullScreen);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateTimerProgress();
    void updateTypingProgress(qreal typingProgress);

private:
    void drawPath(QPainter &painter, const QPainterPath &path);
    void paintBorder(qreal startLength, QRectF borderRect, QPainter painter);

    int timerProgress;
    QTimer *prisonerTimer;
    qreal totalTime;
    bool isTimerRunning;
    bool isFullScreen;
    qreal typingProgress; // New member variable for typing progress
    QPainterPath progressPath; // New member variable for the progress path
    QPainterPath typingPath; // New member variable for the typing path
};

#endif // PROGRESSBORDERWIDGET_H