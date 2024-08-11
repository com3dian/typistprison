#ifndef TEXTEDITHELPER_H
#define TEXTEDITHELPER_H

#include <QTextEdit>
#include <QScrollBar>
#include <QTextCharFormat>
#include <QMimeData>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QDebug>
#include "searchHighlighter.h"

class TextEditHelper {
public:
    TextEditHelper(QTextEdit* parent);

    void changeFontSize(int delta);
    void search(const QString &searchString);
    void searchPrev(const QString &searchString);
    void clearSearch();
    
private:
    QTextEdit* parentEdit;
    int globalFontSize;
    int matchStringIndex;
    SearchHighlighter* highlighter;
};

#endif // TEXTEDITHELPER_H
