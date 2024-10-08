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
#include "qmarkdowntextedit.h"
#include "searchWidget.h"

class MarkdownViewTab : public QWidget {
    Q_OBJECT

public:
    explicit MarkdownViewTab(const QString &content, QWidget *parent = nullptr);

private:
    void setupTextEdit(const QString &content);
    void setupScrollBar();
    void syncScrollBar();
    void activateHighlightMode();
    void deactivateHighlightMode();

    QMarkdownTextEdit *textEdit;
    QScrollBar *vScrollBar;

    QHBoxLayout *globalLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topLeftLayout;
    QHBoxLayout *bottomLeftLayout;

};

#endif // MARKDOWNVIEWTAB_H