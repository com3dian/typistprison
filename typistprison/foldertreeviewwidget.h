#ifndef FOLDERTREEVIEWWIDGET_H
#define FOLDERTREEVIEWWIDGET_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QVBoxLayout>

class FolderTreeViewWidget : public QWidget {
    Q_OBJECT

public:
    explicit FolderTreeViewWidget(QWidget *parent = nullptr);  // Constructor
    void toggleFileTreeView();

signals:
    void doubleClickedOnFile(const QString &filePath);

private:
    void setupFileTree();           // Function to set up the file tree
    void setupButton();
    void onItemClicked(const QModelIndex &index);
    void onCustomContextMenu(const QPoint &point);
    void addFile();
    void onDoubleClicked(const QModelIndex &index);

    QFileSystemModel *fileModel;    // Pointer to the file system model
    QTreeView *fileTreeView;        // Pointer to the QTreeView
    QWidget *buttonWidget;          // Add this member variable to hold the button widget
    QVBoxLayout *layout;            // Pointer to the QVBoxLayout
    qreal scalingFactor; // Correctly call devicePixelRatio()
};

#endif // FOLDERTREEVIEWWIDGET_H