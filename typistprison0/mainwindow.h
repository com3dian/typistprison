#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "customtabwidget.h"

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

    void setupUntitledTab();
    void createNewTab(const QString &tabName);
    void onLastTabClosed();
};
#endif // MAINWINDOW_H
