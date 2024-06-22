#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customtabwidget.h"
#include "customstyle.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , untitledCount(1)
{
    ui->setupUi(this);
    ui->menubar->setStyle(new CustomStyle());
    ui->menubar->setStyleSheet(
        "QMenuBar {"
        "    background-color: #2A2A2A;"
        "    border: none;"
        "    padding: 0px;"
        "}"
        "QMenuBar::item {"
        "    background-color: transparent;"
        "    padding-left: 8px; padding-right: 8px; padding-top: 2px; padding-bottom: 4px;"
        "    border: none;"
        "}"
        "QMenuBar::item:selected {"
        "    background-color: #31363F;"
        "}"
        "QMenuBar::item:pressed {"
        "    background-color: #31363F;"
        "}"
        "QMenu {"
        "    background-color: #2A2A2A;"
        "    border: none;"
        "    min-width: 180px;"
        "}"
        "QMenu::item {"
        "    margin: 0px;"  // No margin between menu items
        "    padding: 4px 16px;"  // Padding inside menu items
        "    border: none;"
        "    background-color: transparent;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #3A424F;"
        "}"
        "QMenu::separator {"
        "    height: 1px;"  // Adjust the height of the separator
        "    background-color: #555555;"  // Set the color of the separator
        "    margin: 4px 0;"  // Adjust the margin around the separator
        "}"
    );

    // load customWidget
    customTabWidget = new CustomTabWidget(this);

    // Set the central widget to the custom tab widget
    setCentralWidget(customTabWidget);

    // Connect the action of opening a file to the creation of a new tab
    ui->actionOpen_File->setShortcut(QKeySequence(Qt::CTRL  |  Qt::Key_O));
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::openFile);

    // Connect the action of creating a new file to the creation of a new untitled tab
    ui->actionNew_File->setShortcut(QKeySequence(Qt::CTRL  |  Qt::Key_N));
    connect(ui->actionNew_File, &QAction::triggered, this, &MainWindow::setupUntitledTab);

    // Connect the customTabWidget's lastTabClosed signal to the MainWindow's slot to close the window
    connect(customTabWidget, &CustomTabWidget::lastTabClosed, this, &MainWindow::onLastTabClosed);

    // Connect actionFiction_View to 
    // connect(ui->actionFiction_View, &QAction::triggered, customTabWidget, &CustomTabWidget::switchToFictionView);

    setupUntitledTab();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUntitledTab()
{
    // Create the default untitled tab
    customTabWidget->createNewTab("", "untitled-" + QString::number(untitledCount++));
}

void MainWindow::openFile()
{
    // Open a file dialog to select a file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*)"));

    // If a file is selected, create a new tab and load the file
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContents = in.readAll();
            file.close();

            // Create a new tab with the file name as the tab text
            customTabWidget->createNewTab(fileContents, QFileInfo(filePath).fileName());
        }
    }
}

void MainWindow::onLastTabClosed() {
    close();
}
