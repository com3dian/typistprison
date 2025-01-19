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

#include "customtabwidget.h"

class CustomTabBarWidget : public QWidget {
    Q_OBJECT

public:
    explicit CustomTabBarWidget(QWidget *parent = nullptr, CustomTabWidget *syncedTabWidget = nullptr);
    ~CustomTabBarWidget();

private:
    QFrame *menuBar;
    QPushButton *toggleButton;

    QPushButton *button1;
    QPushButton *button2;

    QTabBar *tabBar;
    
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

private slots:
    void toggleMenuBar();
};

#endif // CUSTOMTABBARWIDGET_H
