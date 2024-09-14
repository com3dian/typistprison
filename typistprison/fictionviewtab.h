#ifndef FICTIONVIEWTAB_H
#define FICTIONVIEWTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollBar>
#include <QString>
#include <QFile>
#include <QPushButton>
#include <QLineEdit>
#include <QMargins>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>
#include "fictiontextedit.h"
#include "searchWidget.h"


class FictionViewTab : public QWidget {
    Q_OBJECT

public:
    explicit FictionViewTab(const QString &content, const QString &filePath, QWidget *parent = nullptr);
    bool saveContent();

private:
    void setupTextEdit(const QString &content);
    void setupScrollBar();
    void syncScrollBar();
    void activateSniperMode();
    void deactivateSniperMode();
    void editContent();
    void updateWordcount();

    FictionTextEdit *textEdit;
    QScrollBar *vScrollBar;
    QPushButton *button2;

    QHBoxLayout *globalLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topLeftLayout;
    QHBoxLayout *bottomLeftLayout;
    QString currentFilePath;
    QLabel *wordCountLabel;
    QString oldTextContent;

signals:
    void onChangeTabName(const QString &fileName);
};

#endif // FICTIONVIEWTAB_H
