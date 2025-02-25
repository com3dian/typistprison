#ifndef CUSTOMTABBARWIDGET_H
#define CUSTOMTABBARWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabBar>
#include <QMenuBar>
#include <QPropertyAnimation>
#include <QEvent>
#include <QMouseEvent>
#include <QEasingCurve>
#include <QSpacerItem>
#include <QApplication>
#include <QLabel>
#include <QScrollArea>

#include "customtabwidget.h"
#include "functionbar/customtabbar.h"
#include "functionbar/paintcornerwidget.h"
#include "functionbar/paintleftedgewidget.h"


class CustomTabBarWidget : public QWidget {
    Q_OBJECT

public:
    explicit CustomTabBarWidget(QWidget *parent = nullptr, CustomTabWidget *syncedTabWidget = nullptr);
    ~CustomTabBarWidget();

    QList<QPushButton*> getAllButtons() const;

private:
    QFrame *menuBar;
    QPushButton *toggleButton;

    QPushButton *button1;
    QPushButton *button2;
    CustomTabBar *tabBar;
    PaintCornerWidget *paintCornerWidget;
    PaintLeftEdgeWidget *paintLeftEdgeWidget;
    QWidget *transparentLeftWidget;
    QWidget *transparentRightWidget;

    QSpacerItem *fixedSpacer1;
    QSpacerItem *fixedSpacer2;

    bool isScrollbuttonActive;

    QHBoxLayout *mainLayout;

    QMenu *menu;
    bool isExpanded;
    CustomTabWidget *syncedTabWidget;

    void showMenu();
    void setupMenuBar();
    void setupTabBar();
    void setupToggleButton();

    void onTabGainedAttention(int index);
    void onTabInserted(int index, const QString &label);
    void onTabRemoved(int index);
    void onTabClosedFromSyncedWidget(int index);
    void onTabMoved(int from, int to);
    void onTabTitleUpdated(int index, QString newTitle);

public slots:
    void toggleMenuBar();
    void showPaintCornerWidget();
    void hidePaintCornerWidget();
    void showPaintLeftEdgeWidget();
    void hidePaintLeftEdgeWidget();
    void hideBothPaintCornerWidget();
    void notHideBothPaintCornerWidget();
};

#endif // CUSTOMTABBARWIDGET_H
