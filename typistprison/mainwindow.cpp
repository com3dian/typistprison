#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , untitledCount(1)
    , previousSplitterPosition(0.166)
{
    ui->setupUi(this);

    // QWidget* centralWidget = this->centralWidget();  // Assuming centralWidget is the largest
    // centralWidget->setStyleSheet("background-color: #ADD8E6;");

    ui->menubar->setStyle(new CustomStyle());

    QScreen *screen = QGuiApplication::primaryScreen(); // Use QGuiApplication to get the primary screen
    qreal scalingFactor = screen->devicePixelRatio(); // Correctly call devicePixelRatio()

    QString menubarStyleSheet = QString(
        "QMenuBar {"
        "    background-color: #2c2f30;"
        "    border: none;"
        "    padding: 0px;"
        "}"
        "QMenuBar::item {"
        "    background-color: transparent;"
        "    margin: 0px;"
        "    padding-left: %1px;"
        "    padding-right: %2px;"
        "    padding-top: %3px;"
        "    padding-bottom: %4px;"
        "    border: none;"
        "}"
        "QMenuBar::item:selected {"
        "    background-color: #31363F;"
        "}"
        "QMenuBar::item:pressed {"
        "    background-color: #31363F;"
        "}"
        "QMenu {"
        "    background-color: #2c2f30;"
        "    border: none;"
        "    min-width: %5px;"
        "}"
        "QMenu::item {"
        "    margin: 0px;"  // No margin between menu items
        "    padding: %6px %7px %8px %9px;"  // Padding inside menu items
        "    border: none;"
        "    background-color: transparent;"
        "}"
        "QMenu::item:selected {"
        "    background-color: #3A424F;"
        "}"
        "QMenu::separator {"
        "    height: 1px;"  // Adjust the height of the separator
        "    background-color: #555555;"  // Set the color of the separator
        "    margin: 0px 0px;"  // Adjust the margin around the separator
        "}"
        ).arg(8)
         .arg(8)
         .arg(1)
         .arg(4)
         .arg(180)
         .arg(2)
         .arg(32)
         .arg(4)
         .arg(8);

    ui->menubar->setStyleSheet(menubarStyleSheet);

    ui->statusbar->setStyleSheet("QStatusBar { background-color: #2c2f30;}");
    ui->statusbar->setContentsMargins(2.8, 0, 2.8, 0);

    QPushButton *sidePanelButton = new QPushButton(this);
    sidePanelButton->setStyleSheet(
            "QPushButton {"
            "border: none;"
            "border-image: url(:/icons/sidebar_silent.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:hover {"
            "border-image: url(:/icons/sidebar_hover.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:pressed {"
            "border-image: url(:/icons/sidebar_clicked.png) 0 0 0 0 stretch stretch;"
            "}"
    );
    sidePanelButton->setFixedSize(14, 14);

    // Add the button to the status bar
    ui->statusbar->addWidget(sidePanelButton);

    folderTreeView = new FolderTreeViewWidget;
    folderTreeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // QFileSystemModel *fileModel = new QFileSystemModel(this);
    // fileModel->setRootPath(QDir::rootPath()); // Set the root directory to the file system root

    // QTreeView *fileTreeView = new QTreeView(this);
    // fileTreeView->setModel(fileModel);  // Set the model for the tree view
    // fileTreeView->setRootIndex(fileModel->index(QDir::homePath()));  // Set the initial root to the user's home directory
    // for (int i = 1; i < fileModel->columnCount(); ++i) {
    //     fileTreeView->setColumnHidden(i, true);
    // }

    // // Hide the headers (both horizontal and vertical)
    // fileTreeView->header()->hide();   // Hide horizontal header (column names)
    // fileTreeView->setHeaderHidden(true);  // Alternative to ensure header is hidden
    // fileTreeView->setRootIsDecorated(true); // Keep the expand/collapse arrows

    // fileTreeView->setStyleSheet(
    //     "QTreeView { background-color: #31363F; "
    //     "            border: none; "
    //     "            color: #C7C8CC;}"
    //     "QTreeView::item:hover { background-color: #3a424f; "
    //     "                        color: #C7C8CC; "
    //     "                        border: none}"
    //     "QTreeView::item:selected { background-color: #3a424f; "
    //     "                           color: #ffa896; "
    //     "                           border: none;}" // Selected item background and text color
    //     );

    // load customWidget
    customTabWidget = new CustomTabWidget(this);

    centralSplitter = new QSplitter(this);
    centralSplitter->setStyleSheet("QSplitter { background-color: #2c2f30; }");
    centralSplitter->addWidget(folderTreeView);  // Left pane: file explorer
    centralSplitter->addWidget(customTabWidget);     // Right pane: tab widget
    centralSplitter->setCollapsible(1, false); // Disable collapsing for the right widget


    // Set the central widget to the custom tab widget
    setCentralWidget(centralSplitter);

    // Connect the action of opening a file to the creation of a new tab
    ui->actionOpen_File->setShortcut(QKeySequence(Qt::CTRL  |  Qt::Key_O));
    connect(ui->actionOpen_File, &QAction::triggered, this, [this]() {
        // Call the openFile method with an empty string or a specific path
        openFile("");  // Or provide a default path if needed
    });

    // Connect the action of creating a new file to the creation of a new untitled tab
    ui->actionNew_File->setShortcut(QKeySequence(Qt::CTRL  |  Qt::Key_N));
    connect(ui->actionNew_File, &QAction::triggered, this, &MainWindow::setupUntitledTab);

    // Connect the customTabWidget's lastTabClosed signal to the MainWindow's slot to close the window
    connect(customTabWidget, &CustomTabWidget::lastTabClosed, this, &MainWindow::onLastTabClosed);

    // Connect actionFiction_View to
    // connect(ui->actionFiction_View, &QAction::triggered, customTabWidget, &CustomTabWidget::switchToFictionView);

    connect(sidePanelButton, &QPushButton::clicked, this, &MainWindow::toggleFileTreeView);

    connect(folderTreeView, &FolderTreeViewWidget::doubleClickedOnFile, this, &MainWindow::openFile);


    setupUntitledTab();
    // previousSplitterPosition = 0.166;
    toggleFileTreeView();
    toggleFileTreeView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUntitledTab()
{
    // Create the default untitled tab
    customTabWidget->createNewTab("", "untitled-" + QString::number(untitledCount++), "", true);
}

void MainWindow::openFile(const QString &filePath)  // No default argument here
{
    QString selectedFilePath = filePath;

    // If no filePath is provided, open a file dialog to select a file
    if (selectedFilePath.isEmpty()) {
        selectedFilePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("All Files (*)"));
    }

    // If a file is selected (either from the dialog or the provided path), load the file
    if (!selectedFilePath.isEmpty()) {
        QFile file(selectedFilePath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString fileContents = in.readAll();
            file.close();

            // Create a new tab with the file name as the tab text
            customTabWidget->createNewTab(fileContents, QFileInfo(selectedFilePath).fileName(), selectedFilePath);
        }
    }
}


void MainWindow::onLastTabClosed() {
    close();
}

void MainWindow::toggleFileTreeView() {
    qDebug() << "void MainWindow::toggleFileTreeView()";
    QList<int> sizes = centralSplitter->sizes(); // Get current splitter sizes
    
    // Only calculate the previousSplitterPosition if sizes[0] is non-zero (i.e., left widget is visible)
    if (sizes[0] != 0) {
        // Save the current splitter position before hiding the left widget
        previousSplitterPosition = static_cast<float>(sizes[0]) / centralSplitter->size().width();
        qDebug() << "splitter close";
        qDebug() << "previousSplitterPosition" << previousSplitterPosition;

        // Hide the left widget and adjust the splitter size
        // folderTreeView->toggleFileTreeView();
        centralSplitter->setSizes(QList<int>({0, centralSplitter->size().width()})); // Right widget takes over
    } else {
        qDebug() << "splitter open";
        qDebug() << "previousSplitterPosition" << previousSplitterPosition;
        // Restore sizes based on the stored previousSplitterPosition
        centralSplitter->setSizes(
            QList<int>({static_cast<int>(centralSplitter->size().width() * previousSplitterPosition), 
                        static_cast<int>(centralSplitter->size().width() * (1 - previousSplitterPosition))}));

        // Reset previousSplitterPosition after restoring
        previousSplitterPosition = 0.0;

        // folderTreeView->toggleFileTreeView();
    }
}
