#ifndef PLAINTEXTEDIT_H
#define PLAINTEXTEDIT_H

#include <QTextEdit>
#include <QList>
#include <QTextBlock>
#include <string>
#include "searchHighlighter.h"

class PlaintextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit PlaintextEdit(QWidget *parent = nullptr);
    void load(const QString& text);
    void search(const QString &searchString);
    void clearSearch();

signals:
    void onPlaintextSearch(const QString &text);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    SearchHighlighter *highlighter;

private:
    void changeFontSize(int delta);
    int globalFontSize;
};

#endif // PLAINTEXTEDIT_H
