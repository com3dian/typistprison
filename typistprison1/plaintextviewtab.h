#ifndef PLAINTEXTVIEWTAB_H
#define PLAINTEXTVIEWTAB_H

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
#include "plaintextedit.h"
#include "searchWidget.h"

class PlaintextViewTab : public QWidget {
    Q_OBJECT

public:
    explicit PlaintextViewTab(const QString &content, QWidget *parent = nullptr);

private:
    void setupTextEdit(const QString &content);
    void setupScrollBar();
    void syncScrollBar();
    void activateHighlightMode();
    void deactivateHighlightMode();

    PlaintextEdit *textEdit;
    QScrollBar *vScrollBar;

    QHBoxLayout *globalLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *topLeftLayout;
    QHBoxLayout *bottomLeftLayout;

};

#endif // PLIANTEXTVIEWTAB_H
