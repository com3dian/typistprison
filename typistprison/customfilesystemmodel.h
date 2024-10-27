#ifndef CUSTOMFILESYSTEMMODEL_H
#define CUSTOMFILESYSTEMMODEL_H

#include <QFileSystemModel>
#include <QIcon>

class CustomFileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    explicit CustomFileSystemModel(QObject *parent = nullptr);  // Constructor

    // Override the data method to customize the icons for files and folders
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // CUSTOMFILESYSTEMMODEL_H
