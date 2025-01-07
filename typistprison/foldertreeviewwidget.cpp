#include "foldertreeviewwidget.h"
#include "customtreestyle.h"
#include "customfilesystemmodel.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QDir>
#include <QScreen>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextBrowser>
#include <QFileDialog>

FolderTreeViewWidget::FolderTreeViewWidget(QWidget *parent, QString folderRoot)
    : QWidget(parent), fileModel(nullptr), fileTreeView(nullptr), layout(nullptr)
{
    // QScreen *screen = QGuiApplication::primaryScreen();
    // scalingFactor = screen->devicePixelRatio();

    // Initialize fileModel with the custom file system model
    fileModel = new CustomFileSystemModel(this);
    fileModel->setRootPath(folderRoot);

    qDebug() << "folderRoot ========================= " << folderRoot;

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
    buttonWidget->setFixedHeight(24);

    QSpacerItem *topSpacerLeft = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);

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
    newFileButton->setFixedSize(16, 16);
    connect(newFileButton, &QPushButton::clicked, this, &FolderTreeViewWidget::addFile);

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
    newFolderButton->setFixedSize(16, 16);

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
    refreshButton->setFixedSize(16, 16);

    // Create a horizontal layout to add buttons
    QHBoxLayout *buttonWidgetLayout = new QHBoxLayout;
    buttonWidgetLayout->setContentsMargins(0, 0, 8, 0);  // Remove margins to fit buttons neatly
    buttonWidgetLayout->setSpacing(12);  // Optional: set spacing between buttons
    
    // Add text to the layout
    buttonWidgetLayout->addItem(topSpacerLeft);

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
    fileTreeView->setRootIndex(fileModel->index(folderRoot));

    if (QDir(folderRoot).exists()) {
        qDebug() << "path exists";
    } else {
        qDebug() << "path not exist";
    }

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
        "                           color: #ff9c87; "
        "                           border: none;}"
        "QLineEdit { background-color: #3a424f;"
        "            color: #ff9c87;"
        "            selection-background-color: #ff9c87;"
        "            selection-color: #3a424f;}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: transparent;"
        "    width: 8px;"
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #4A515E;"
        "    min-height: 16px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #989A9C;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
        "    height: 0px;"
        "    width: 0px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}"
    );

    // Add fileTreeView to the vertical layout
    layout->addWidget(fileTreeView);

    fileTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    fileTreeView->setEditTriggers(QTreeView::NoEditTriggers);
    connect(fileTreeView, &QTreeView::customContextMenuRequested, this, &FolderTreeViewWidget::onCustomContextMenu);
    connect(fileTreeView, &QTreeView::doubleClicked, this, &FolderTreeViewWidget::onDoubleClicked);
}

// Toggle visibility of the file tree view
void FolderTreeViewWidget::toggleFileTreeView() {
    // Toggle visibility of the file tree view and button widget
    bool isVisible = fileTreeView->isVisible();
    fileTreeView->setVisible(!isVisible); // Toggle file tree view visibility
    buttonWidget->setVisible(!isVisible);   // Show buttons when tree view is hidden, and hide buttons when tree view is shown
}

void FolderTreeViewWidget::onCustomContextMenu(const QPoint &point) {
    // Create a context menu
    QMenu contextMenu(this);

    // Get the index of the item that was clicked
    QModelIndex index = fileTreeView->indexAt(point);

    if (!index.isValid()) {
        return;  // If no valid item was clicked, return
    }

    if (fileModel->isDir(fileTreeView->currentIndex())) {
        // Add actions to the menu
        QAction *openActionFolder = contextMenu.addAction("Open");
        QAction *deleteActionFolder = contextMenu.addAction("Delete");
        // rename folder action
        // TODO: maybe remove this for now?
        // no file refresh method yet
        QAction *renameActionFolder = contextMenu.addAction("Rename", [=](){
            fileTreeView->edit(index);
        });
        QAction *newFileActionFolder = contextMenu.addAction("New file");
        QAction *copyPathActionFolder = contextMenu.addAction("Copy Path");

        // Show the context menu at the cursor's global position
        QAction *selectedAction = contextMenu.exec(QCursor::pos());


        // Handle the action triggered
        if (selectedAction == openActionFolder) {
            // Open the file
            qDebug() << "Open clicked on item:" << index.data().toString();
        } else if (selectedAction == deleteActionFolder) {
            // Delete the file
            qDebug() << "Delete clicked on item:" << index.data().toString();
        } else if (selectedAction == renameActionFolder) {
            // Rename the file
            qDebug() << "Rename clicked on item:" << index.data().toString();
        } else if (selectedAction == renameActionFolder) {
            // new file
            qDebug() << "Rename clicked on item:" << index.data().toString();
        } else if (selectedAction == renameActionFolder) {
            // copy the path
            qDebug() << "Rename clicked on item:" << index.data().toString();
        }
    } else {
        QAction *openActionFile = contextMenu.addAction("Open");
        QAction *deleteActionFile = contextMenu.addAction("Delete");
        QAction *renameActionFile = contextMenu.addAction("Rename");
        QAction *copyPathActionFile = contextMenu.addAction("Copy Path");

        // Show the context menu at the cursor's global position
        QAction *selectedAction = contextMenu.exec(QCursor::pos());


        // Handle the action triggered
        if (selectedAction == openActionFile) {
            // Open the file
            qDebug() << "Open clicked on item:" << index.data().toString();
        } else if (selectedAction == deleteActionFile) {
            // Delete the file
            qDebug() << "Delete clicked on item:" << index.data().toString();
        } else if (selectedAction == renameActionFile) {
            // Rename the file
            qDebug() << "Rename clicked on item:" << index.data().toString();
        }
    }
}

void FolderTreeViewWidget::addFile()
{
    // Get the currently selected index from the file tree
    QModelIndex currentIndex = fileTreeView->currentIndex();

    // Check if the selected index is valid and corresponds to a directory
    QString currentDir;
    if (currentIndex.isValid() && fileModel->isDir(currentIndex)) {
        // If a directory is selected, get its absolute path
        currentDir = fileModel->fileInfo(currentIndex).absoluteFilePath();
    } else {
        // If no directory is selected, default to the root directory of the file model
        currentDir = fileModel->rootDirectory().absolutePath();
    }

    // Create a default new file name
    QString defaultFileName = "newfile.txt";
    QString newFilePath = QDir(currentDir).filePath(defaultFileName);

    // Ensure the default file doesn't exist already
    int fileCount = 1;
    while (QFile::exists(newFilePath)) {
        defaultFileName = QString("newfile_%1.txt").arg(fileCount++);
        newFilePath = QDir(currentDir).filePath(defaultFileName);
    }

    // Create the file
    QFile newFile(newFilePath);
    if (newFile.open(QIODevice::WriteOnly)) {
        newFile.close();  // Close after creating an empty file
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Failed to create the file"));
        return;
    }

    // Force the model to update to show the new file
    fileModel->setRootPath(fileModel->rootPath());

    // Get the index of the new file in the model
    QModelIndex newFileIndex = fileModel->index(newFilePath);

    // Make the new file name editable
    fileTreeView->setCurrentIndex(newFileIndex);
    fileTreeView->edit(newFileIndex);

    // Connect to the editingFinished signal for automatic saving
    connect(fileTreeView->itemDelegate(), &QAbstractItemDelegate::commitData, this, [this, newFilePath](QWidget *editor) {
        QModelIndex currentIndex = fileTreeView->currentIndex();
        QString newName = fileModel->data(currentIndex, Qt::EditRole).toString();

        if (!newName.isEmpty() && newName != QFileInfo(newFilePath).fileName()) {
            // Rename the file to the new name
            QString newPath = QFileInfo(newFilePath).absolutePath() + "/" + newName;
            QFile::rename(newFilePath, newPath);
            fileModel->setRootPath(fileModel->rootPath());  // Refresh the view
        }
    });
}

void FolderTreeViewWidget::onDoubleClicked(const QModelIndex &index)
{
    if (fileModel->isDir(index)) {
        return;
    }
    QString clickedfilePath = fileModel->filePath(index);
    emit doubleClickedOnFile(clickedfilePath);
}

void FolderTreeViewWidget::refresh(const QString &newFolderRoot) {
    // Clean up existing resources to prevent memory leaks
    if (fileModel) {
        delete fileModel;
        fileModel = nullptr;
    }
    if (fileTreeView) {
        delete fileTreeView;
        fileTreeView = nullptr;
    }
    if (layout) {
        delete layout;
        layout = nullptr;
    }

    // Update folderRoot with the new path (if provided)
    if (!newFolderRoot.isEmpty()) {
        folderRoot = newFolderRoot;
    }

    // Reinitialize fileModel
    fileModel = new CustomFileSystemModel(this);
    fileModel->setRootPath(folderRoot);

    qDebug() << "folderRoot refreshed ========================= " << folderRoot;

    // Reinitialize layout
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);  // No margins
    layout->setSpacing(0);                  // No spacing between widgets

    // Reinitialize fileTreeView
    fileTreeView = new QTreeView(this);
    CustomTreeStyle *customStyle = new CustomTreeStyle(":/icons/angle_right.png", ":/icons/angle_down.png");
    fileTreeView->setStyle(customStyle);

    // Reinitialize buttons and file tree view
    setupButton();
    setupFileTree();

    // Set the layout for the widget
    this->setLayout(layout);
}
