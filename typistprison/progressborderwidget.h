#ifndef PROGRESSBORDERWIDGET_H
#define PROGRESSBORDERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainterPath>

class ProgressBorderWidget : public QWidget {
    Q_OBJECT

public:
    explicit ProgressBorderWidget(QWidget *parent = nullptr);
    void startTimerProgress(int timeLimit, int wordGoal);
    void clearTimerProgress();
    void setFullScreen(bool fullScreen);

signals:
    void needsRepaint();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateTimerProgress();
    void updateTypingProgress(int wordCount);

private:
    QTimer *prisonerTimer;
    qreal timerProgress;
    qreal totalTime; // total time in seconds
    int targetWordCount;
    bool isTimerRunning;
    bool isFullScreen;

    void drawPath(QPainter &painter, const QPainterPath &path);
    void drawPathTest(QPainter &painter, const QPainterPath &path);
    void paintBorder(qreal startLength, QPointF startPoint, QRectF borderRect, QPainter painter);
};

#endif // PROGRESSBORDERWIDGET_H