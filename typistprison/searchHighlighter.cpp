#include "searchHighlighter.h"
#include <QBrush>
#include <QColor>

SearchHighlighter::SearchHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent), searchString("") {}

void SearchHighlighter::setSearchString(const QString &searchString) {
    this->searchString = searchString;
    rehighlight(); // Trigger a rehighlight whenever the search string changes
}

void SearchHighlighter::highlightBlock(const QString &text) {
    if (searchString.isEmpty()) {
        return;
    }

    QTextCharFormat highlightFormat;
    QColor customColor("#F17C67");
    highlightFormat.setBackground(QBrush(customColor));

    QRegularExpression expression(QRegularExpression::escape(searchString));
    QRegularExpressionMatchIterator i = expression.globalMatch(text);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(), match.capturedLength(), highlightFormat);
    }
}
