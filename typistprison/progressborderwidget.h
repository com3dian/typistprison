#ifndef PROGRESSBORDERWIDGET_H
#define PROGRESSBORDERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainterPath>

class ProgressBorderWidget : public QWidget {
    Q_OBJECT

public:
    explicit ProgressBorderWidget(QWidget *parent = nullptr);
    void startProgress();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateProgress();

private:
    void drawPath(QPainter &painter, const QPainterPath &path);

    int m_progress;
    QTimer *m_timer;
};

#endif // PROGRESSBORDERWIDGET_H