#ifndef MARKDOWNVIEWTAB_H
#define MARKDOWNVIEWTAB_H

#include <QWidget>
#include <QGridLayout>
#include <QScrollBar>
#include <QPushButton>
#include <QLineEdit>
#include <QMargins>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "qmarkdowntextedit.h"
#include "searchWidget.h"
#include "basetextedittab.h"

class MarkdownViewTab : public BaseTextEditTab {
    Q_OBJECT

public:
    explicit MarkdownViewTab(const QString &content, const QString &filePath, QWidget *parent = nullptr);
    // bool saveContent();
    QString getTextContent() const override;

private:
    QMarkdownTextEdit *textEdit;
    QString currentFilePath;
    QScrollBar *vScrollBar;
    QHBoxLayout *globalLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topLeftLayout;
    QHBoxLayout *bottomLeftLayout;

    void setupTextEdit(const QString &content);
    void setupScrollBar();
    void syncScrollBar();
    void activateHighlightMode();
    void deactivateHighlightMode();
    void editContent();

signals:
    void onChangeTabName(const QString &fileName);

};

#endif // MARKDOWNVIEWTAB_H