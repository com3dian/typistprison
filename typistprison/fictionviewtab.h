#ifndef FICTIONVIEWTAB_H
#define FICTIONVIEWTAB_H

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
#include "fictiontextedit.h"

class FictionViewTab : public QWidget {
    Q_OBJECT

public:
    explicit FictionViewTab(const QString &content, QWidget *parent = nullptr);

private:
    void setupTextEdit(const QString &content);
    void setupScrollBar();
    void syncScrollBar();
    void activateHighlightMode();
    void deactivateHighlightMode();

    FictionTextEdit *textEdit;
    QScrollBar *vScrollBar;
    QPushButton *button2;

    QHBoxLayout *globalLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topLeftLayout;
    QHBoxLayout *bottomLeftLayout;

};

#endif // FICTIONVIEWTAB_H
