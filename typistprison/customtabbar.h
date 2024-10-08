#ifndef CUSTOMTABBAR_H
#define CUSTOMTABBAR_H

#include <QTabBar>

class CustomTabBar : public QTabBar {
    Q_OBJECT // This macro is necessary for all classes that define signals and slots

public:
    explicit CustomTabBar(QWidget *parent = nullptr);
    ~CustomTabBar();
};

#endif // CUSTOMTABBAR_H
