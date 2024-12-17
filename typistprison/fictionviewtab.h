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
    QString getCurrentFilePath() const;

private:
    FictionTextEdit *textEdit;
    QString currentFilePath;
    QScrollBar *vScrollBar;
    QPushButton *sniperButton;
    QHBoxLayout *globalLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topLeftLayout;
    QHBoxLayout *bottomLeftLayout;
    QLabel *wordCountLabel;
    QString oldTextContent;

    void setupTextEdit(const QString &content);
    void setupScrollBar();
    void syncScrollBar();
    void activateSniperMode();
    void deactivateSniperMode();
    void editContent();
    void updateWordcount();

signals:
    void onChangeTabName(const QString &fileName);
    void onChangeFileType(const QString &fileName);
};

#endif // FICTIONVIEWTAB_H
