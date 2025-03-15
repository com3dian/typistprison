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
#include <QPoint>
#include <QMessageBox>
#include <QStackedWidget>
#include <QGraphicsDropShadowEffect>

#include "customtabwidget.h"
#include "ui_mainwindow.h"
#include "customstyle.h"
#include "foldertreeviewwidget.h"
#include "functionbar.h"
#include "projectmanager.h"
#include "basetextedittab.h"
#include "searchWidget.h"
#include "popups/imageframe.h"
#include "functionbar/menubutton.h"

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

public slots:
    void openFile(const QString &filePath = ""); // Declaration of the openFile slot
    void saveFile();
    void searchFile();
    void openProject();
    void saveProject();
    void showMarkdownImage(const QString &imagePath, QPoint pos);
    void hideMarkdownImage();

private:
    Ui::MainWindow *ui;
    int untitledCount;
    CustomTabWidget *customTabWidget;
    FolderTreeViewWidget *folderTreeView;
    QSplitter *centralSplitter;
    float previousSplitterPosition;
    ProjectManager *projectManager;
    QPushButton *sidePanelButton;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *menuToggleButton;
    QFrame *existingFrame = nullptr;
    CustomTabBarWidget *tabBarWidget;
    ImageFrame *imageFrame;
    QLabel *imageLabel;

    void setupUntitledTab();
    void setupMenuButtons(CustomTabBarWidget *tabBarWidget);
    void createNewTab(const QString &tabName);
    void onLastTabClosed();
    void toggleFileTreeView();
    void resizeEvent(QResizeEvent *event) override;
    void adjustButtonPosition();
    void handleMouseEnterMenuButton(QPushButton *button);
    void handleFocusLeaveMenuButton();
    void setupActions();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void mouseClick();

};
#endif // MAINWINDOW_H
