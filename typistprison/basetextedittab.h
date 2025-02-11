#ifndef BASETEXTEDITTAB_H
#define BASETEXTEDITTAB_H

#include <QWidget>
#include <QString>

class BaseTextEditTab : public QWidget {
    Q_OBJECT

public:
    explicit BaseTextEditTab(const QString &content, const QString &filePath, QWidget *parent = nullptr);
    virtual ~BaseTextEditTab();

    virtual QString getCurrentFilePath() const;
    virtual void setFilePath(const QString &path);
    virtual bool saveContent() = 0; // Pure virtual function

signals:
    void onChangeTabName(const QString &newName);
    void onChangeFileType(const QString &newName);

protected:
    QString currentFilePath;
    QString content;
};

#endif // BASETEXTEDITTAB_H