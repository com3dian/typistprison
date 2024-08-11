#ifndef SEARCHHIGHLIGHTER_H
#define SEARCHHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class SearchHighlighter : public QSyntaxHighlighter {
    Q_OBJECT

public:
    explicit SearchHighlighter(QTextDocument *parent = nullptr);

    void setSearchString(const QString &searchString);
    QString getSearchString() const;

protected:
    void highlightBlock(const QString &text) override;

private:
    QString searchString;
};

#endif // SEARCHHIGHLIGHTER_H
