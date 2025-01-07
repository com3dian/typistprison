#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QTextEdit>
#include <QTabBar>
#include <QStylePainter>
#include <QPainterPath>
#include <QStyleOptionTab>

#include "customtabbar.h"
#include "projectmanager.h"


class CustomTabWidget : public QTabWidget {
    Q_OBJECT

public:
    CustomTabWidget(QWidget *parent = nullptr, ProjectManager *projectManager = nullptr);

    CustomTabBar *customTabBar;

    void createNewTab(const QString &filePath, bool isUntitled = false, int tabIndex = -1);
    void switchToFictionView();

public slots:
    void updateTabTitle(const QString &fileName);
    void updateFileType(const QString &previousFileName);

private:
    int untitledCount;
    ProjectManager *projectManager;

    void setupTabWidget();
    void setupTabBar();
    void setupStyles();
    void closeWindowIfNoTabs(int index);
    void applyFictionViewStyles(QTextEdit *textEdit);
    void applyEditorViewStyles(QTextEdit *textEdit);

private slots:
    void onTabCloseRequested(int index, bool needAsking = true);
    

signals:
    void lastTabClosed();

};

#endif // CUSTOMTABWIDGET_H
