#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QTextEdit>
#include <QTabBar>
#include <QStylePainter>
#include <QPainterPath>
#include <QStyleOptionTab>
#include <QHBoxLayout>
#include <QPoint>

#include "projectmanager.h"


class CustomTabWidget : public QTabWidget {
    Q_OBJECT

public:
    CustomTabWidget(QWidget *parent = nullptr, ProjectManager *projectManager = nullptr);

    void createNewTab(const QString &filePath, bool isUntitled = false, int tabIndex = -1);
    void switchToFictionView();

public slots:
    void updateTabTitle(const QString &fileName);
    void updateFileType(const QString &previousFileName);
    void closeWindowIfNoTabs(int index);
    void onTabCloseRequested(int index, bool needAsking = true);
    void handleFileDeleted(const QString &deletedFilePath);
    void handleFileRenamed(const QString &originalFilePath, const QString &newFilePath);
    void showImageAt(const QString &imagePath, QPoint lastMousePos);
    void hideImage();
    void showWikiAt(const QString &wikiContent, QPoint lastMousePos);
    void hideWiki();

private:
    int untitledCount;
    ProjectManager *projectManager;

    void setupTabWidget();
    void setupTabBar();
    void setupStyles();
    void applyFictionViewStyles(QTextEdit *textEdit);
    void applyEditorViewStyles(QTextEdit *textEdit);
    int checkIdenticalOpenedFile(const QString &givenFilePath);

private slots:
    
    
protected:
    void tabInserted(int index) override;

signals:
    void lastTabClosed();
    void tabInsertedSignal(int index, const QString &label);
    void tabClosedFromSyncedTabWidgetSignal(int index);
    void updatedTabTitleSignal(int currentIndex, QString newTitle);
    void showImageAtSignal(const QString &imagePath, QPoint lastMousePos);
    void hideImageSignal();
    void showWikiAtSignal(const QString &imagePath, QPoint lastMousePos);
    void hideWikiSignal();

};

#endif // CUSTOMTABWIDGET_H
