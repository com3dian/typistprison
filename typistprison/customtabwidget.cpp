#include "customtabwidget.h"
#include "fictiontextedit.h"
#include "fictionviewtab.h"
#include "plaintextedit.h"
#include "plaintextviewtab.h"
#include "savemessagebox.h"
#include "markdownviewtab.h"

#include <QVBoxLayout>
#include <QMainWindow>
#include <QStylePainter>
#include <QStyleOptionTab>
#include <QScrollArea>
#include <QPainterPath>
#include <QSpacerItem>
#include <QScrollBar>
#include <QRegularExpression>


CustomTabWidget::CustomTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setupTabBar();
    setupTabWidget();
    setupStyles();

    setTabPosition(QTabWidget::North);
}

void CustomTabWidget::setupTabBar() {
    // customTabBar = new CustomTabBar();
    // setTabBar(customTabBar);
    setTabPosition(QTabWidget::North); // Options: North, South, East, West
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
                            "    border: 0px;"
                            "    background-color: #31363F;"
                            "    margin: 0px;"
                            "}"
        "QTabBar { alignment: left; }"
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

        "QTabBar::close-button {"
        "   subcontrol-position: right;"
        "   image: url(:/icons/tab_close.png); "
        "   margin-right:8px;"
        "}"

        "QTabBar::close-button:hover {"
        "    image: url(:/icons/tab_hover.png);"
        "    margin-right:8px;"
        "}"

        "QTabBar::close-button:pressed {"
        "    image: url(:/icons/tab_clicked.png);"
        "    margin-right:8px;"
        "}"
    );
}

void CustomTabWidget::createNewTab(const QString &content,
                                   const QString &tabName,
                                   const QString &filePath,
                                   bool isUntitled) {
    QWidget *newTab;
    if (tabName.endsWith(".cell.txt") || isUntitled) {
        newTab = new FictionViewTab(content, filePath, this);
        connect(static_cast<FictionViewTab*>(newTab), &FictionViewTab::onChangeTabName, this, &CustomTabWidget::updateTabTitle);
    } else if (tabName.endsWith(".md")) {
        newTab = new MarkdownViewTab(content, filePath, this);
        connect(static_cast<MarkdownViewTab*>(newTab), &MarkdownViewTab::onChangeTabName, this, &CustomTabWidget::updateTabTitle);
    } else {
        newTab = new PlaintextViewTab(content, filePath, this);
        connect(static_cast<PlaintextViewTab*>(newTab), &PlaintextViewTab::onChangeTabName, this, &CustomTabWidget::updateTabTitle);
    }

    addTab(newTab, tabName);
    setCurrentWidget(newTab);
}

void CustomTabWidget::closeWindowIfNoTabs(int index) {
    onTabCloseRequested(index);
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

void CustomTabWidget::updateTabTitle(const QString &fileName) {
    int currentIndex = this->currentIndex();
    QString currentTitle = this->tabText(currentIndex);

    QString newTitle;
    if (currentTitle.endsWith("*") and fileName.endsWith("*")) {
        return;
    } else if (fileName == "*") {
        newTitle = currentTitle + fileName;
    } else {
        newTitle = fileName;
    }
    this->setTabText(currentIndex, newTitle);
}

void CustomTabWidget::onTabCloseRequested(int index) {
    QString currentTitle = this->tabText(index);
    if (currentTitle.endsWith("*")) {
        
        // QRect tabBarRect = this->customTabBar->geometry();

        SaveMessageBox msgBox(this);
        // msgBox.adjustSize();
        // int msgBoxWidth = msgBox.sizeHint().width();
        // int msgBoxHeight = msgBox.sizeHint().height();

        // int x = tabBarRect.x() + (tabBarRect .width() - msgBoxWidth) / 2;  // Horizontally center
        // int y = tabBarRect.y()  + tabBarRect.height();  // Bottom of the tab bar

        // qDebug() << "msgRect" << x << y;
        // msgBox.move(x, y);  // Set the position and size
        int ret = msgBox.exec();

        QWidget *newTab = nullptr;
        QString title;
        QRegularExpression regex("untitled-\\d+\\*");
        bool isSuccessful;

        switch (ret) {
            case QMessageBox::Save:
                // Save the document
                qDebug() << "Saving the document at index" << index;

                newTab = this->widget(index);
                title = this->tabText(index);
                qDebug() << regex.match(title).hasMatch();

                if (title.endsWith("cell.txt*") | regex.match(title).hasMatch()) {
                    isSuccessful = static_cast<FictionViewTab*>(newTab)->saveContent();
                } else if (title.endsWith(".md*")) {
                    isSuccessful = static_cast<MarkdownViewTab*>(newTab)->saveContent();
                } else {
                    isSuccessful = static_cast<PlaintextViewTab*>(newTab)->saveContent();
                }
                if (isSuccessful) {
                    removeTab(index);
                }
                break;
            case QMessageBox::Discard:
                // Discard changes and close the tab
                qDebug() << "Discarding changes and closing the tab at index" << index;
                removeTab(index);
                break;
            case QMessageBox::Cancel:
                // Cancel the close operation
                qDebug() << "Canceling the close operation for tab at index" << index;
                // Do nothing to cancel the close operation
                break;
            default:
                // Should never be reached
                break;
        }
    } else {
        removeTab(index);
    }
}