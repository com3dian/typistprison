#include "foldertreeviewwidget.h"
#include "customtreestyle.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QDir>
#include <QScreen>
#include <QPushButton>
#include <QTextBrowser>

FolderTreeViewWidget::FolderTreeViewWidget(QWidget *parent)
    : QWidget(parent), fileModel(nullptr), fileTreeView(nullptr), layout(nullptr)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    scalingFactor = screen->devicePixelRatio();

    // Initialize fileModel and set the root path to user's home directory
    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::homePath());

    // Initialize layout and fileTreeView
    layout = new QVBoxLayout(this);
    // Set margins and spacing to zero
    layout->setContentsMargins(0, 0, 0, 0);  // No margins
    layout->setSpacing(0);                    // No spacing between widgets

    fileTreeView = new QTreeView(this);
    CustomTreeStyle *customStyle = new CustomTreeStyle(":/icons/angle_right.png", ":/icons/angle_down.png");
    fileTreeView->setStyle(customStyle);

    setupButton();
    // Set up file tree view and add it to layout
    setupFileTree();
    this->setLayout(layout);
}

void FolderTreeViewWidget::setupButton() {
    buttonWidget = new QWidget;
    buttonWidget->setStyleSheet("background-color: #31363F;");
    buttonWidget->setFixedHeight(38 * scalingFactor);
    QTextBrowser *folderTextBrowser = new QTextBrowser(this);
    folderTextBrowser->setPlainText("Folder");

    QPushButton *newFileButton = new QPushButton(this);
    newFileButton->setStyleSheet(
            "QPushButton {"
            "border: none;"
            "border-image: url(:/icons/newfile_silent.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:hover {"
            "border-image: url(:/icons/newfile_hover.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:pressed {"
            "border-image: url(:/icons/newfile_clicked.png) 0 0 0 0 stretch stretch;"
            "}"
            "   padding: 10px;"
    );
    newFileButton->setFixedSize(20*scalingFactor, 20*scalingFactor);

    QPushButton *newFolderButton = new QPushButton(this);
    newFolderButton->setStyleSheet(
            "QPushButton {"
            "border: none;"
            "border-image: url(:/icons/newfolder_silent.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:hover {"
            "border-image: url(:/icons/newfolder_hover.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:pressed {"
            "border-image: url(:/icons/newfolder_clicked.png) 0 0 0 0 stretch stretch;"
            "}"
            "   padding: 10px;"
    );
    newFolderButton->setFixedSize(20*scalingFactor, 20*scalingFactor);

    QPushButton *refreshButton = new QPushButton(this);
    refreshButton->setStyleSheet(
            "QPushButton {"
            "border: none;"
            "border-image: url(:/icons/refresh_silent.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:hover {"
            "border-image: url(:/icons/refresh_hover.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:pressed {"
            "border-image: url(:/icons/refresh_clicked.png) 0 0 0 0 stretch stretch;"
            "}"
            "   padding: 10px;"
    );
    refreshButton->setFixedSize(20*scalingFactor, 20*scalingFactor);

    // Create a horizontal layout to add buttons
    QHBoxLayout *buttonWidgetLayout = new QHBoxLayout;
    buttonWidgetLayout->setContentsMargins(0, 0, 8*scalingFactor, 0);  // Remove margins to fit buttons neatly
    buttonWidgetLayout->setSpacing(8);  // Optional: set spacing between buttons
    
    // Add text to the layout
    buttonWidgetLayout->addWidget(folderTextBrowser);

    // Add buttons to the layout
    buttonWidgetLayout->addWidget(newFileButton);
    buttonWidgetLayout->addWidget(newFolderButton);
    buttonWidgetLayout->addWidget(refreshButton);

    // Set the layout for the widget
    buttonWidget->setLayout(buttonWidgetLayout);

    layout->addWidget(buttonWidget);
}

void FolderTreeViewWidget::setupFileTree() {
    // Set the model for the tree view
    fileTreeView->setModel(fileModel);
    fileTreeView->setRootIndex(fileModel->index(QDir::homePath()));

    // Hide all columns except the first one (file name column)
    for (int i = 1; i < fileModel->columnCount(); ++i) {
        fileTreeView->setColumnHidden(i, true);
    }

    // Hide the header (both horizontal and vertical)
    fileTreeView->header()->hide();
    fileTreeView->setHeaderHidden(true);

    // Customize the appearance of the file tree view
    fileTreeView->setStyleSheet(
        "QTreeView { background-color: #31363F; "
        "            border: none; "
        "            color: #C7C8CC;}"
        "QTreeView::item:hover { background-color: #3a424f; "
        "                        color: #C7C8CC; "
        "                        border: none;}"
        "QTreeView::item:selected { background-color: #3a424f; "
        "                           color: #ffa896; "
        "                           border: none;}"
    );

    // Add fileTreeView to the vertical layout
    layout->addWidget(fileTreeView);
}

// Toggle visibility of the file tree view
void FolderTreeViewWidget::toggleFileTreeView() {
    // Toggle visibility of the file tree view and button widget
    bool isVisible = fileTreeView->isVisible();
    fileTreeView->setVisible(!isVisible); // Toggle file tree view visibility
    buttonWidget->setVisible(!isVisible);   // Show buttons when tree view is hidden, and hide buttons when tree view is shown
}