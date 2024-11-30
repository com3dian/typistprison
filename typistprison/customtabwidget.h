#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QTextEdit>
#include <QTabBar>
#include <QStylePainter>
#include <QPainterPath>
#include <QStyleOptionTab>

#include "customtabbar.h"


class CustomTabWidget : public QTabWidget {
    Q_OBJECT

public:
    CustomTabBar *customTabBar;

    CustomTabWidget(QWidget *parent = nullptr);
    void createNewTab(const QString &content, const QString &tabName, const QString &filePath, bool isUntitled = false);
    void switchToFictionView();

public slots:
    void updateTabTitle(const QString &fileName);

private:
    void setupTabWidget();
    void setupTabBar();
    void setupStyles();
    void closeWindowIfNoTabs(int index);
    void applyFictionViewStyles(QTextEdit *textEdit);
    void applyEditorViewStyles(QTextEdit *textEdit);

private slots:
    void onTabCloseRequested(int index);
    

signals:
    void lastTabClosed();

};

#endif // CUSTOMTABWIDGET_H
