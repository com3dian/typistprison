#include "textedithelper.h"

TextEditHelper::TextEditHelper(QTextEdit* parent)
    : parentEdit(parent), globalFontSize(12), matchStringIndex(-1)
{
    highlighter = new SearchHighlighter(parentEdit->document());
}

void TextEditHelper::changeFontSize(int delta) {
    QScrollBar *vScrollBar = parentEdit->verticalScrollBar();
    int scrollBarPosition = vScrollBar->value();
    float positionRatio = static_cast<float>(scrollBarPosition) / static_cast<float>(vScrollBar->maximum());

    globalFontSize += delta;
    if (globalFontSize < 1) {
        globalFontSize = 1; // Prevent font size from becoming too small
    }

    QTextCursor cursor(parentEdit->document());
    cursor.select(QTextCursor::Document);

    QTextCharFormat format;
    format.setFontPointSize(globalFontSize);

    cursor.mergeCharFormat(format);

    int updatedPosition = static_cast<int>(positionRatio * static_cast<float>(vScrollBar->maximum()));
    vScrollBar->setValue(updatedPosition);
}

void TextEditHelper::search(const QString &searchString) {
    qDebug() << "--------------------------";
    qDebug() << ">>>>>>>>>>>>>>>>>>>>> search input" << matchStringIndex;
    if (searchString != highlighter->getSearchString()) {
        matchStringIndex = -1; // Reset matchStringIndex for a new search string
    }

    QString documentText = parentEdit->document()->toPlainText();

    // if (matchStringIndex == -1) {
    //     matchStringIndex += 1;
    // }
    matchStringIndex = documentText.indexOf(searchString, matchStringIndex + 1);
    if (matchStringIndex == -1) {
        matchStringIndex = documentText.indexOf(searchString, matchStringIndex + 1);
        if (matchStringIndex == -1) {
            return;
        }
    }
    qDebug() << ">>>>>>>>>>>>>>>>>>>>> search output" << matchStringIndex;

    QTextCursor cursor = parentEdit->textCursor();
    cursor.setPosition(matchStringIndex);
    cursor.setPosition(matchStringIndex + searchString.length(), QTextCursor::KeepAnchor);
    parentEdit->setTextCursor(cursor);
    // matchStringIndex += 1;
    highlighter->setSearchString(searchString);
}

void TextEditHelper::searchPrev(const QString &searchString) {
    qDebug() << "--------------------------";
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>> searchPrev input" << matchStringIndex;
    QString documentText = parentEdit->document()->toPlainText();
    // if (matchStringIndex < searchString.length() || matchStringIndex > documentText.length()) {
    //     matchStringIndex = documentText.length();
    // }
    matchStringIndex = documentText.lastIndexOf(searchString, matchStringIndex - 1);

    if (matchStringIndex <= -1) {
        qDebug() << "    First time search not found;";
        matchStringIndex = documentText.length();
        matchStringIndex = documentText.lastIndexOf(searchString);
        if (matchStringIndex == -1) {
            qDebug() << "    Second time search not found;";
            return;
        }
    }
    qDebug() << ">>>>>>>>>>>>>>>>>>>>>> searchPrev output" << matchStringIndex;
    QTextCursor cursor = parentEdit->textCursor();
    cursor.setPosition(matchStringIndex);
    cursor.setPosition(matchStringIndex + searchString.length(), QTextCursor::KeepAnchor);
    parentEdit->setTextCursor(cursor);
}

void TextEditHelper::clearSearch() {
    highlighter->setSearchString("");
    matchStringIndex = -1;
}
