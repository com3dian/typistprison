#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QTextEdit>
#include <QFont>
#include <QMimeData>
#include <QScrollBar>
#include "plaintexthighlighter.h"

class PlaintextEdit : public QTextEdit {
    Q_OBJECT

public:
    PlaintextEdit(QWidget *parent = nullptr);

    void load(const QString& text);
    void changeFontSize(int delta);
    void insertFromMimeData(const QMimeData *source);
    void search(const QString &searchString);
    void searchPrev(const QString &searchString);
    void clearSearch();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *e) override;

private:
    PlaintextHighlighter* highlighter;
    int globalFontSize;
    int matchStringIndex;

signals:
    void onPlaintextSearch(const QString &text);
    void focusGained();
    void onSave();
    void keyboardInput();
};

#endif // PLAINTEXTEDIT_H
