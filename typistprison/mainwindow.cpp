#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , untitledCount(1)
    , previousSplitterPosition(0.166)
    , imageFrame(nullptr)
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
    "    background-color: #1F2020;"
    "}"
    );
    centralSplitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // Make the handle always visible by preventing complete collapse
    centralSplitter->setHandleWidth(8);  // Set the physical handle width to match CSS

    folderTreeView = new FolderTreeViewWidget;
    centralSplitter->addWidget(folderTreeView);  

    QWidget *editorWidget = new QWidget(this);
    QVBoxLayout *editorLayout = new QVBoxLayout;
    editorLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
    editorLayout->setSpacing(0); // Remove spacing
    customTabWidget = new CustomTabWidget(this, projectManager);

    // Double holder for tab widget
    QWidget *customTabWidgetHolder = new QWidget(this);
    customTabWidgetHolder->setAttribute(Qt::WA_TranslucentBackground);
    customTabWidgetHolder->setStyleSheet(
        "QWidget {"
        "    background-color: #2c2c2c;"
        "    border: none;"
        "    border-top-left-radius: 6px;"      // Top-left corner
        "    border-top-right-radius: 6px;"     // Top-right corner
        "    border-bottom-left-radius: 6px;"   // Bottom-left corner (more rounded)
        "    border-bottom-right-radius: 6px;"  // Bottom-right corner (more rounded)
        "}"
    );
    // Create layout for the holder
    QVBoxLayout *tabWidgetLayout = new QVBoxLayout(customTabWidgetHolder);
    tabWidgetLayout->setContentsMargins(0, 0, 8, 8); // Left, Right, Bottom margins (No top margin)
    tabWidgetLayout->setSpacing(0); // Optional, to avoid extra spacing
    tabWidgetLayout->addWidget(customTabWidget);

    tabBarWidget = new CustomTabBarWidget(this, customTabWidget);
    setupMenuButtons(tabBarWidget);

    editorLayout->addWidget(tabBarWidget);
    editorLayout->addWidget(customTabWidgetHolder);
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
    sidePanelButton->setVisible(false);

    // Move the button to the bottom-left corner dynamically
    adjustButtonPosition();

    // Connect the customTabWidget's lastTabClosed signal to the MainWindow's slot to close the window
    connect(customTabWidget, &CustomTabWidget::lastTabClosed, this, &MainWindow::onLastTabClosed);
    connect(customTabWidget, &CustomTabWidget::showImageAtSignal, this, &MainWindow::showMarkdownImage);
    connect(customTabWidget, &CustomTabWidget::hideImageSignal, this, &MainWindow::hideMarkdownImage);

    connect(folderTreeView, &FolderTreeViewWidget::doubleClickedOnFile, this, &MainWindow::openFile);
    connect(folderTreeView, &FolderTreeViewWidget::fileDeleted, customTabWidget, &CustomTabWidget::handleFileDeleted);
    connect(folderTreeView, &FolderTreeViewWidget::fileRenamed, customTabWidget, &CustomTabWidget::handleFileRenamed);

    // set up side panel button
    sidePanelButton->setEnabled(false);
    connect(sidePanelButton, &QPushButton::clicked, this, &MainWindow::toggleFileTreeView);
    
    setupUntitledTab();
    setupActions();

    qApp->installEventFilter(this);
}

void MainWindow::setupActions() {
    // Create actions for shortcuts instead of relying on invisible buttons
    QAction* actionOpenFile = new QAction("Open File", this);
    QAction* actionNewFile = new QAction("New File", this);
    QAction* actionSaveFile = new QAction("Save File", this);
    QAction* actionSearchFile = new QAction("Search File", this);
    QAction* actionOpenProject = new QAction("Open Project", this);
    QAction* actionSaveAllProject = new QAction("Save All Project", this);

    // Set shortcuts for the actions
    actionOpenFile->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    actionNewFile->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    actionSaveFile->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
    // actionSearchFile->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F)); // Added search shortcut

    // Add actions to the main window so they work globally
    this->addAction(actionOpenFile);
    this->addAction(actionNewFile);
    this->addAction(actionSaveFile);
    this->addAction(actionSearchFile);
    this->addAction(actionOpenProject);
    this->addAction(actionSaveAllProject);

    // Connect signals to the appropriate slots
    connect(actionOpenFile, &QAction::triggered, this, [this]() {
        openFile("");
    });
    connect(actionNewFile, &QAction::triggered, this, [this]() {
        setupUntitledTab();
    });
    connect(actionSaveFile, &QAction::triggered, this, [this]() {
        saveFile();
    });
    connect(actionSearchFile, &QAction::triggered, this, [this]() {
        searchFile();
    });
    connect(actionOpenProject, &QAction::triggered, this, [this]() {
        openProject();
    });
    connect(actionSaveAllProject, &QAction::triggered, this, [this]() {
        saveProject();
    });

    // Keep the invisible buttons for any other purpose if needed
    // but they're not necessary for shortcuts anymore
    // invisibleButtonHolder->setVisible(false);
}

// ---- Override resizeEvent to Keep Button in Bottom-Left ----
void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    adjustButtonPosition();
}

// ---- Adjust the Button's Position Based on Window Size ----
void MainWindow::adjustButtonPosition() {
    int margin = 16; // Adjust margin for spacing
    int buttonX = margin; 
    int buttonY = height() - sidePanelButton->height() - margin; // Bottom left corner
    sidePanelButton->move(buttonX, buttonY);
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::setupUntitledTab()
{
    // Create the default untitled tab
    customTabWidget->createNewTab("", true);
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

void MainWindow::saveFile() {
    // Get the current active tab from customTabWidget
    QWidget *currentTab = customTabWidget->currentWidget();
    
    // Check if there is an active tab
    if (currentTab) {
        // Cast to BaseTextEditTab since all tab types inherit from it
        BaseTextEditTab *textEditTab = qobject_cast<BaseTextEditTab*>(currentTab);
        
        // If the cast is successful, save the content
        if (textEditTab) {
            textEditTab->saveContent();
        }
    }
}

void MainWindow::searchFile()
{
    // Get the current tab widget
    QWidget* currentWidget = customTabWidget->currentWidget();
    if (!currentWidget)
        return;
    
    // Cast to BaseTextEditTab since all tab types inherit from it
    BaseTextEditTab* textEditTab = qobject_cast<BaseTextEditTab*>(currentWidget);
    if (textEditTab) {
        // Set focus to the text edit
        textEditTab->setFocus();
        
        // Find the search widget within the tab and give it focus
        SearchWidget* searchWidget = textEditTab->findChild<SearchWidget*>();
        if (searchWidget) {
            qDebug() << "SearchWidget";
            searchWidget->handleSearch("");
        }
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
    toggleFileTreeView();
    sidePanelButton->setVisible(true);
    return;
}

void MainWindow::saveProject() {
    for (int i = 0; i < customTabWidget->count(); ++i) {
        QWidget *tab = customTabWidget->widget(i);
        BaseTextEditTab *textEditTab = qobject_cast<BaseTextEditTab*>(tab);
        if (textEditTab) {
            textEditTab->saveContent();
        }
    }
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
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        
        // Convert the global position to the main window's coordinates.
        QPoint posInMainWindow = this->mapFromGlobal(mouseEvent->globalPos());
        if (this->rect().contains(posInMainWindow))
        {
            // If the drop-down frame is visible, handle it
            if (existingFrame && existingFrame->isVisible())
            {
                if (obj == menuToggleButton) {
                    return true;
                }
                // Convert mouse position to MainWindow coordinates
                QPoint posInMainWindow = this->mapFromGlobal(mouseEvent->globalPos());
                
                // Check if click is inside the frame
                if (existingFrame->geometry().contains(posInMainWindow)) {
                    
                    return false;
                }
                
                if (button1->geometry().contains(posInMainWindow) or button2->geometry().contains(posInMainWindow)) {
                    return true;
                }

                emit mouseClick();
                handleFocusLeaveMenuButton();
                disconnect(this, &MainWindow::mouseClick, tabBarWidget, &CustomTabBarWidget::closeMenuBar);
                
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MainWindow::handleMouseEnterMenuButton(QPushButton *button) {
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
        "QMenuButton { "
        "    margin: 0px; "
        "    padding: 0px; "
        "    border: none; "
        "    background-color: transparent; "
        "}  "
    );

    // Set up the layout for the frame with no margins and spacing between buttons.
    QVBoxLayout *frameLayout = new QVBoxLayout(newFrame);
    frameLayout->setContentsMargins(10, 4, 6, 6);
    frameLayout->setSpacing(0);

    #ifdef Q_OS_MAC
        #define CONTROL_SYMBOL "⌘ "  // Mac Command key
    #else
        #define CONTROL_SYMBOL "Ctrl+"  // Windows & Linux use "Ctrl"
    #endif

    if (button == button1) {
        // Create buttons for Button 1's frame.
        MenuButton *frameButtonNew = new MenuButton("New", CONTROL_SYMBOL "N", this);
        connect(frameButtonNew, &MenuButton::clicked, this, [this]() {
            emit mouseClick();
            handleFocusLeaveMenuButton();
            disconnect(this, &MainWindow::mouseClick, tabBarWidget, &CustomTabBarWidget::closeMenuBar);
            setupUntitledTab();
        });
        
        // Open file button
        MenuButton *frameButtonOpen = new MenuButton("Open", CONTROL_SYMBOL "O", newFrame);
        connect(frameButtonOpen, &MenuButton::clicked, this, [this]() {
            emit mouseClick();
            handleFocusLeaveMenuButton();
            disconnect(this, &MainWindow::mouseClick, tabBarWidget, &CustomTabBarWidget::closeMenuBar);
            openFile("");
        });
        
        // Save file button
        MenuButton *frameButtonSave = new MenuButton("Save", CONTROL_SYMBOL "S", newFrame);
        connect(frameButtonSave, &MenuButton::clicked, this, [this]() {
            emit mouseClick();
            handleFocusLeaveMenuButton();
            disconnect(this, &MainWindow::mouseClick, tabBarWidget, &CustomTabBarWidget::closeMenuBar);
            saveFile();
        });

        MenuButton *frameButtonSearch = new MenuButton("Search", "", newFrame);
        connect(frameButtonSearch, &QPushButton::clicked, this, [this]() {
            emit mouseClick();
            handleFocusLeaveMenuButton();
            disconnect(this, &MainWindow::mouseClick, tabBarWidget, &CustomTabBarWidget::closeMenuBar);
            searchFile();
        });
        
        frameButtonNew->setFixedSize(108, 28);
        frameButtonOpen->setFixedSize(108, 28);
        frameButtonSave->setFixedSize(108, 28);
        frameButtonSearch->setFixedSize(108, 28);

        frameLayout->addWidget(frameButtonNew);
        frameLayout->addWidget(frameButtonOpen);
        frameLayout->addWidget(frameButtonSave);
        frameLayout->addWidget(frameButtonSearch);
    }
    else if (button == button2) {
        // Create buttons for Button 2's frame.
        MenuButton *frameButtonOpen = new MenuButton("Open", "", newFrame);
        connect(frameButtonOpen, &MenuButton::clicked, this, [this]() {
            emit mouseClick();
            handleFocusLeaveMenuButton();
            disconnect(this, &MainWindow::mouseClick, tabBarWidget, &CustomTabBarWidget::closeMenuBar);
            openProject();
        });
        MenuButton *frameButtonSaveAll = new MenuButton("Save All", "", newFrame);
        MenuButton *frameButtonSwitch = new MenuButton("Switch...", "", newFrame);
        MenuButton *frameButtonEdit = new MenuButton("Paste", "", newFrame);

        // Set fixed sizes and unique background colors.
        frameButtonOpen->setFixedSize(108, 28);
        frameButtonSaveAll->setFixedSize(108, 28);
        frameButtonSwitch->setFixedSize(108, 28);
        frameButtonEdit->setFixedSize(108, 28);

        frameLayout->addWidget(frameButtonOpen);
        frameLayout->addWidget(frameButtonSaveAll);
        frameLayout->addWidget(frameButtonSwitch);
        frameLayout->addWidget(frameButtonEdit);
    }

    // Resize the frame to fit its contents.
    newFrame->adjustSize();

    // Position the frame just below the button (adjusting as needed).
    QPoint buttonBottomLeft = button->mapToGlobal(QPoint(0, button->height() + 14));
    newFrame->move(this->mapFromGlobal(buttonBottomLeft));

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
        tabBarWidget, &CustomTabBarWidget::closeMenuBar,
        Qt::UniqueConnection);
}

void MainWindow::handleFocusLeaveMenuButton() {
    if (existingFrame)
    {
        existingFrame->setVisible(false);
    }
}

void MainWindow::showMarkdownImage(const QString &imagePath, QPoint lastMousePos) {
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        // Scale the image by 0.2 (20% of its original size) while maintaining aspect ratio
        QPixmap scaledPixmap = pixmap.scaled(pixmap.size() * 0.2, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Create ImageFrame if it doesn't exist
        if (!imageFrame) {
            imageFrame = new ImageFrame(this);
        }

        // Set the image to the frame
        imageFrame->setImage(scaledPixmap);

        // Resize the frame to fit the scaled image with shadow padding
        QSize imageSize = scaledPixmap.size();
        QSize frameSize = imageSize + QSize(20, 20); // Extra padding for the shadow
        imageFrame->resize(frameSize);

        // Adjust position relative to the mouse or reference point
        QPoint popupPos = this->mapFromGlobal(lastMousePos);
        qDebug() << "popupPos: " << popupPos;
        imageFrame->move(popupPos);

        // Create and configure the drop shadow effect
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(24);   // Softer, larger shadow
        shadow->setColor(QColor("#1F2020")); // Shadow color
        shadow->setOffset(0, 0);     // Zero offset for uniform shadow

        // Apply the effect to the frame
        imageFrame->setGraphicsEffect(shadow);

        imageFrame->show();
    } else {
        qDebug() << "Failed to load image:" << imagePath;
    }
}

void MainWindow::hideMarkdownImage() {
    if (imageFrame) {
        imageFrame->hide();  // Hide the frame
    }
}
