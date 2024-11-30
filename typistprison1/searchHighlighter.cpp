#include "searchHighlighter.h"
#include <QBrush>
#include <QColor>
#include <QDebug>

SearchHighlighter::SearchHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent), searchString("") {}

void SearchHighlighter::setSearchString(const QString &searchString) {
    qDebug() << "search text for SearchHighlighter::highlightBlock: " << searchString;
    this->searchString = searchString;
    rehighlight(); // Trigger a rehighlight whenever the search string changes
}

QString SearchHighlighter::getSearchString() const {
    return searchString;
}

void SearchHighlighter::highlightBlock(const QString &text) {
    if (searchString.isEmpty()) {
        return;
    }

    QTextCharFormat highlightFormat;
    QColor customColor("#4F726C"); // #20604F
    highlightFormat.setBackground(QBrush(customColor));

    QString escapedSearchString = QRegularExpression::escape(searchString);
    QRegularExpression expression(escapedSearchString);
    
    int offset = 0;
    while (offset < text.length()) {
        QRegularExpressionMatch match = expression.match(text, offset);
        if (match.hasMatch()) {
            int start = match.capturedStart();
            int length = match.capturedLength();
            setFormat(start, length, highlightFormat);
            offset = start + 1; // Move offset one position forward to allow for overlapping matches
        } else {
            break;
        }
    }
}
