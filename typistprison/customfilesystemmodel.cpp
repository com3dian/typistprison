/*
file system model for file tree panel
*/

#include "customfilesystemmodel.h"

CustomFileSystemModel::CustomFileSystemModel(QObject *parent)
    : QFileSystemModel(parent)  // Call the base class constructor
{
    // Any additional initialization can be done here if needed
}

/*
override data method to show different file type with unique color

*/
QVariant CustomFileSystemModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::ForegroundRole) {
        QString fileName = QFileSystemModel::fileName(index);
        
        // Different colors for different file types
        if (fileName.endsWith(".txt")) {
            return QColor("#87CEEB");
        } else if (fileName.endsWith(".md")) {
            return QColor("#98FB98");
        } else if (fileName.endsWith(".cell.txt")) {
            return QColor("#DDA0DD");
        } else if (!isDir(index)) {
            return QColor("#B8B8B8");  // Gray color for other file types
        }
    }
    else if (role == Qt::DecorationRole) {
        // Check if it's a directory first
        if (isDir(index)) {
            return QIcon(":/icons/folder.png");
        }
        // Then handle files
        QString fileName = QFileSystemModel::fileName(index);
        
        if (fileName.endsWith(".txt")) {
            return QIcon(":/icons/txt_icon.png");
        } else if (fileName.endsWith(".md")) {
            return QIcon(":/icons/md_icon.png");
        } else if (fileName.endsWith(".cell.txt")) {
            return QIcon(":/icons/cell_icon.png");
        } else {
            return QIcon(":/icons/file.png");  // Default icon for other file types
        }
    }
    
    return QFileSystemModel::data(index, role);
}

Qt::ItemFlags CustomFileSystemModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}