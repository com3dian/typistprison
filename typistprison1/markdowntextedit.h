#ifndef MARKDOWNTEXTEDIT_H
#define MARKDOWNTEXTEDIT_H

#include <QTextEdit>
#include <QFont>
#include <QScrollBar>
#include <QMimeData>
#include "qmarkdowntextedit.h"
#include "searchHighlighter.h"

class MarkdownTextEdit : public QMarkdownTextEdit {
    Q_OBJECT

public:
    MarkdownTextEdit(QWidget *parent = nullptr);

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
    int globalFontSize;
    int matchStringIndex;
    SearchHighlighter* highlighter;

signals:
    void onPlaintextSearch(const QString &text);
    void focusGained();
};

#endif // MARKDOWNTEXTEDIT_H
