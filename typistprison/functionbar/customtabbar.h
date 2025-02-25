#ifndef CUSTOMTABBAR_H
#define CUSTOMTABBAR_H

#include <QTabBar>
#include <QPainter>
#include <QStyleOptionTab>
#include <QPainterPath>
#include <QSize>
#include <QMouseEvent>
#include <QStylePainter>
#include <QScreen>
#include <QToolButton>
#include <QTimer>

class CustomTabBar : public QTabBar {
    Q_OBJECT

public:
    explicit CustomTabBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void customizeScrollButtons();

signals:
    void lastTabFocus();
    void lastTabNoFocus();
    void firstTabFocus();
    void firstTabNoFocus();
    void scrollbuttonActivate();
    void scrollbuttonInactivate();
};

#endif // CUSTOMTABBAR_H
