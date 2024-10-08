#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QPushButton>
#include <QSplitter>
#include <QFileSystemModel>
#include <QTreeView>
#include <QHeaderView>
#include <QScreen>

#include "customtabwidget.h"
#include "ui_mainwindow.h"
#include "customtabwidget.h"
#include "customstyle.h"
#include "foldertreeviewwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile(); // Declaration of the openFile slot

private:
    Ui::MainWindow *ui;
    int untitledCount;
    CustomTabWidget *customTabWidget;
    FolderTreeViewWidget *folderTreeView;
    QSplitter *centralSplitter;
    float previousSplitterPosition;

    void setupUntitledTab();
    void createNewTab(const QString &tabName);
    void onLastTabClosed();
    void toggleFileTreeView();
};
#endif // MAINWINDOW_H
