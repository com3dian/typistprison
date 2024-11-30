#include "customfilesystemmodel.h"

CustomFileSystemModel::CustomFileSystemModel(QObject *parent)
    : QFileSystemModel(parent)  // Call the base class constructor
{
    // Any additional initialization can be done here if needed
}

QVariant CustomFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DecorationRole) {
        // Check if the item is a directory (folder)
        if (isDir(index)) {
            // Return a custom folder icon
            return QIcon(":/icons/folder.png");
        } else {
            // Return a custom file icon
            return QIcon(":/icons/file.png");
        }
    }

    // For other roles, call the base class implementation
    return QFileSystemModel::data(index, role);
}

Qt::ItemFlags CustomFileSystemModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}