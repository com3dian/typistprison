#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , untitledCount(1)
    , previousSplitterPosition(0.166)
{
    projectManager = new ProjectManager();
    ui->setupUi(this);
    this->addAction(ui->actionOpen_File);   // add action for open file
    this->addAction(ui->actionNew_File);   // add action for new file

    // Create the main container widget
    QWidget *mainContainer = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(mainContainer);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create a wrapper for the splitter (to ensure full expansion)
    QWidget *splitterContainer = new QWidget(mainContainer);
    QVBoxLayout *splitterLayout = new QVBoxLayout(splitterContainer);
    splitterLayout->setContentsMargins(0, 0, 0, 0);
    splitterLayout->setSpacing(0);

    // Create the splitter
    centralSplitter = new QSplitter(splitterContainer);
    centralSplitter->setStyleSheet(
    "QSplitter { background-color: #1F2020; }"  // Background of the splitter itself
    "QSplitter::handle {"
    "    background-color: #1F2020;"  // Change this to your desired color
    "    width: 1px;"  // Adjust thickness
    "}"
    );
    centralSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    folderTreeView = new FolderTreeViewWidget;
    centralSplitter->addWidget(folderTreeView);  

    QWidget *editorWidget = new QWidget(this);
    QVBoxLayout *editorLayout = new QVBoxLayout;
    editorLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
    editorLayout->setSpacing(0); // Remove spacing
    customTabWidget = new CustomTabWidget(this, projectManager);
    tabBarWidget = new CustomTabBarWidget(this, customTabWidget);
    setupMenuButtons(tabBarWidget);

    editorLayout->addWidget(tabBarWidget);
    editorLayout->addWidget(customTabWidget);
    editorWidget->setLayout(editorLayout);

    centralSplitter->addWidget(editorWidget);
    centralSplitter->setCollapsible(1, false);
    centralSplitter->setSizes(QList<int>({0, centralSplitter->size().width()}));

    // Add splitter to its container layout
    splitterLayout->addWidget(centralSplitter);
    splitterContainer->setLayout(splitterLayout);
    splitterContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Add splitter container to main layout
    mainLayout->addWidget(splitterContainer);

    // Set main container as the central widget
    setCentralWidget(mainContainer);

    // ---- Create the Floating Side Panel Button ----
    sidePanelButton = new QPushButton(this);
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
    sidePanelButton->setFixedSize(32, 32);
    sidePanelButton->raise(); // Bring to front

    // Move the button to the bottom-left corner dynamically
    adjustButtonPosition();

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

    // connect(sidePanelButton, &QPushButton::clicked, this, &MainWindow::toggleFileTreeView);

    connect(folderTreeView, &FolderTreeViewWidget::doubleClickedOnFile, this, &MainWindow::openFile);
    connect(folderTreeView, &FolderTreeViewWidget::fileDeleted, customTabWidget, &CustomTabWidget::handleFileDeleted);

    connect(ui->actionOpenProject, &QAction::triggered, this, &MainWindow::openProject);

    // set up side panel button
    sidePanelButton->setEnabled(false);
    connect(sidePanelButton, &QPushButton::clicked, this, &MainWindow::toggleFileTreeView);

    setupUntitledTab();
    ui->menubar->setVisible(false);

    qApp->installEventFilter(this);
}

// ---- Override resizeEvent to Keep Button in Bottom-Left ----
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    adjustButtonPosition();
}

// ---- Adjust the Button's Position Based on Window Size ----
void MainWindow::adjustButtonPosition() {
    int margin = 4; // Adjust margin for spacing
    int buttonX = margin; 
    int buttonY = height() - sidePanelButton->height() - margin; // Bottom left corner
    sidePanelButton->move(buttonX, buttonY);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUntitledTab()
{
    // Create the default untitled tab
    customTabWidget->createNewTab("", true);
}

void MainWindow::setupMenuButtons(CustomTabBarWidget *tabBarWidget) {
    QList<QPushButton*> buttonList = tabBarWidget->getAllButtons();
    if (buttonList.size() >= 3) {
        menuToggleButton = buttonList.at(0);
        button1 = buttonList.at(1);
        button2 = buttonList.at(2);
    }
    button1->installEventFilter(this);
    button2->installEventFilter(this);
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
        // Create a new tab with the file name as the tab text
        customTabWidget->createNewTab(selectedFilePath);
    }
}

void MainWindow::openProject() {
    QString selectedProjectRoot = QFileDialog::getExistingDirectory(
        this, 
        tr("Open Project Folder"), 
        QDir::homePath(), 
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (selectedProjectRoot.isEmpty()) {
        QMessageBox::warning(this, tr("No Folder Selected"), tr("No project folder was selected."));
        return;
    }

    // project manager load
    projectManager->open(selectedProjectRoot);
    
    // Re-initialize folderTreeView
    folderTreeView->refresh(selectedProjectRoot);

    sidePanelButton->setEnabled(true);
    return;
}


void MainWindow::onLastTabClosed() {
    close();
}

void MainWindow::toggleFileTreeView() {
    // qDebug() << "--------------";
    // qDebug() << "void MainWindow::toggleFileTreeView()";
    QList<int> sizes = centralSplitter->sizes(); // Get current splitter sizes
    
    // Only calculate the previousSplitterPosition if sizes[0] is non-zero (i.e., left widget is visible)
    if (sizes[0] != 0) {
        // Save the current splitter position before hiding the left widget
        previousSplitterPosition = static_cast<float>(sizes[0]) / centralSplitter->size().width();

        // Hide the left widget and adjust the splitter size
        // folderTreeView->toggleFileTreeView();
        centralSplitter->setSizes(QList<int>({0, centralSplitter->size().width()})); // Right widget takes over
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
    } else {
        if (previousSplitterPosition == 0) {
            previousSplitterPosition = 0.166;
        }
        // Restore sizes based on the stored previousSplitterPosition
        centralSplitter->setSizes(
            QList<int>({static_cast<int>(centralSplitter->size().width() * previousSplitterPosition), 
                        static_cast<int>(centralSplitter->size().width() * (1 - previousSplitterPosition))}));

        // Reset previousSplitterPosition after restoring
        previousSplitterPosition = 0.0;
        sidePanelButton->setStyleSheet(
            "QPushButton {"
            "border: none;"
            "border-image: url(:/icons/sidebar_triggerred.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:hover {"
            "border-image: url(:/icons/sidebar_triggerred.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:pressed {"
            "border-image: url(:/icons/sidebar_triggerred.png) 0 0 0 0 stretch stretch;"
            "}"
        );
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Check if the event is for one of our menu bar buttons
    if (obj == button1 || obj == button2)
    {
        if (event->type() == QEvent::Enter)
        {
            handleMouseEnterMenuButton(static_cast<QPushButton *>(obj));
        }
    }
        // Now, handle all mouse press events in the application
    if (event->type() == QEvent::MouseButtonPress)
    {
        if (obj == button1 || obj == button2 || obj == menuToggleButton) {
            return QWidget::eventFilter(obj, event);
        }

        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        
        // Optionally, ensure the click is within the main window.
        // Convert the global position to the main window's coordinates.
        QPoint posInMainWindow = this->mapFromGlobal(mouseEvent->globalPos());
        if (this->rect().contains(posInMainWindow))
        {
            // If the drop-down frame is visible, hide it.
            if (existingFrame && existingFrame->isVisible())
            {
                qDebug() << "emit mouseClick();";
                handleFocusLeaveMenuButton();
                emit mouseClick();
                disconnect(this, &MainWindow::mouseClick, tabBarWidget, &CustomTabBarWidget::toggleMenuBar);
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::handleMouseEnterMenuButton(QPushButton *button)
{
    // Remove any existing frame if present.
    if (existingFrame) {
        existingFrame->setParent(nullptr);
        existingFrame->deleteLater();
        existingFrame = nullptr;
    }

    // Create a new frame for the hovered button.
    QFrame *newFrame = new QFrame(this);
    newFrame->setFrameShape(QFrame::NoFrame); // No default frame margins
    newFrame->setStyleSheet(
        "QFrame {   "
        "    margin: 0px; "
        "    padding: 0px; "
        "    background-color: #1F2020;"
        "    border: 1px solid #1F2020; "
        "    border-radius: 4px; "
        "}"
        "QPushButton { "
        "    margin: 0px; "
        "    padding: 0px; "
        "    border: none; "
        "    text-align: left; "
        "    color: #BDBDBD; "
        "    background-color: transparent; "
        "}  "
        "QPushButton:hover {"
            "    color: #FFFFFF;"  // Replace with your desired hover color
        "}"
    );

    // Set up the layout for the frame with no margins and spacing between buttons.
    QVBoxLayout *frameLayout = new QVBoxLayout(newFrame);
    frameLayout->setContentsMargins(12, 4, 12, 12);
    frameLayout->setSpacing(0);

    if (button == button1) {
        // Create buttons for Button 1's frame.
        QPushButton *frameButton1 = new QPushButton("New", newFrame);
        QPushButton *frameButton2 = new QPushButton("Open", newFrame);
        QPushButton *frameButton3 = new QPushButton("Save", newFrame);
        QPushButton *frameButton4 = new QPushButton("Save All", newFrame);
        
        frameButton1->setFixedSize(108, 28);
        frameButton2->setFixedSize(108, 28);
        frameButton3->setFixedSize(108, 28);
        frameButton4->setFixedSize(108, 28);

        frameLayout->addWidget(frameButton1);
        frameLayout->addWidget(frameButton2);
        frameLayout->addWidget(frameButton3);
        frameLayout->addWidget(frameButton4);
    }
    else if (button == button2) {
        // Create buttons for Button 2's frame.
        QPushButton *frameButton1 = new QPushButton("Open", newFrame);
        QPushButton *frameButton2 = new QPushButton("Switch...", newFrame);
        QPushButton *frameButton3 = new QPushButton("Paste", newFrame);

        // Set fixed sizes and unique background colors.
        frameButton1->setFixedSize(108, 28);
        frameButton2->setFixedSize(108, 28);
        frameButton3->setFixedSize(108, 28);

        frameLayout->addWidget(frameButton1);
        frameLayout->addWidget(frameButton2);
        frameLayout->addWidget(frameButton3);
    }

    // Resize the frame to fit its contents.
    newFrame->adjustSize();

    // Position the frame just below the button (adjusting as needed).
    QPoint buttonBottomLeft = button->mapToParent(QPoint(-2, button->height() + 14));
    newFrame->move(buttonBottomLeft);

    // Show the new frame and save it in the member variable.
    newFrame->setVisible(true);
    existingFrame = newFrame;

    // Create and configure the drop shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(24);   // Increase for a softer, larger shadow
    shadow->setColor(QColor("#1F2020")); // Shadow color
    shadow->setOffset(0, 0);     // Zero offset to get shadow on all sides

    // Apply the effect to your frame
    newFrame->setGraphicsEffect(shadow);

    connect(this, &MainWindow::mouseClick,
        tabBarWidget, &CustomTabBarWidget::toggleMenuBar,
        Qt::UniqueConnection);
}

void MainWindow::handleFocusLeaveMenuButton() {
    if (existingFrame)
    {
        existingFrame->setVisible(false);
    }
}