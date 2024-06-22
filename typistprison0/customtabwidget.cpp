#include "customtabwidget.h"
#include "fictiontextedit.h"
#include "fictionviewtab.h"
#include <QVBoxLayout>
#include <QMainWindow>
#include <QStylePainter>
#include <QStyleOptionTab>
#include <QScrollArea>
#include <QPainterPath>
#include <QSpacerItem>
#include <QScrollBar>


CustomTabWidget::CustomTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setupTabBar();
    setupTabWidget();
    setupStyles();
}

void CustomTabWidget::setupTabBar() {
    setTabBar(new CustomTabBar());
}

void CustomTabWidget::setupTabWidget() {
    setTabsClosable(true);
    setMovable(true);

    // Connect the signal for closing tabs to the closeWindowIfNoTabs method
    connect(this, &QTabWidget::tabCloseRequested, this, &CustomTabWidget::closeWindowIfNoTabs);
}

void CustomTabWidget::setupStyles() {
    setStyleSheet(
        "QTabWidget::pane {"
                            "    border: 2px;"
                            "    background-color: #31363F;"
                            "    margin: 0px;"
                            "}"
        "QTabBar::tab:selected { background-color: #31363F; color: #ffa896; border-top: 1px solid #ffa896;}"
        "QTabBar::tab:hover { background-color: #3a424f; color: #ffffff}"
        "QTabBar::tab {"
                        "background-color: #454f61;"
                        "color: #C7C8CC;"
                        "min-width: 8ex;"
                        "padding-left: 20px; padding-right: 40px; padding-top: 6px; padding-bottom: 8px;"
                        "border-top-left-radius: 0px; border-top-right-radius: 0px;"
                        "border-bottom-left-radius: 0px;"
                        "border-bottom-right-radius: 0px;"
                        "border: 0px;"
                        "margin-right: -1px;"
                        "margin-bottom: -1px;"
        "}"
    );
}

void CustomTabWidget::createNewTab(const QString &content, const QString &tabName) {
    FictionViewTab *newTab = new FictionViewTab(content, this);
    addTab(newTab, tabName);
    setCurrentWidget(newTab);
}

void CustomTabWidget::closeWindowIfNoTabs(int index) {
    removeTab(index);
    if (count() == 0) {
        emit lastTabClosed();
    }
}

void CustomTabWidget::switchToFictionView() {
    QWidget *current = currentWidget();
    if (!current) return;

    // Find the FictionTextEdit in the current tab
    FictionTextEdit *textEdit = current->findChild<FictionTextEdit *>();
    if (textEdit) {
        // Apply desired styles or actions

    }
}
