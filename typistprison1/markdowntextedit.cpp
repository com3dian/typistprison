#include "markdowntextedit.h"

MarkdownTextEdit::MarkdownTextEdit(QWidget *parent)
    : QMarkdownTextEdit(parent), globalFontSize(12), matchStringIndex(-1)
{
    highlighter = new SearchHighlighter(this->document());
}

void MarkdownTextEdit::load(const QString &text)
{
    // Clear existing content
    clear();
    QFont font("Noto Mono", globalFontSize); // Set the font to Something, size 12
    this->setFont(font);

    // Load plain text & render into markdown
    // qmarkdowntextedit will handle this: qmarkdowntextedit {line 1442}
    this->setText(text);
}

void MarkdownTextEdit::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) {
            // If user key press "Ctrl" + "+", increase fontsize
            this->changeFontSize(1);
            return;
        } else if (event->key() == Qt::Key_Minus) {
            this->changeFontSize(-1);
            return;
        } else if (event->key() == Qt::Key_F) {
            QString selectedText = textCursor().selectedText();
            emit onPlaintextSearch(selectedText);
            return;
        }
    }
    QMarkdownTextEdit::keyPressEvent(event);
}

void MarkdownTextEdit::changeFontSize(int delta) {
    QScrollBar *vScrollBar = this->verticalScrollBar();
    int scrollBarPosition = vScrollBar->value();
    float positionRatio = static_cast<float>(scrollBarPosition) / static_cast<float>(vScrollBar->maximum());

    globalFontSize += delta;
    if (globalFontSize < 1) {
        globalFontSize = 1; // Prevent font size from becoming too small
    }

    QTextCursor cursor(this->document());
    cursor.select(QTextCursor::Document);

    QTextCharFormat format;
    format.setFontPointSize(globalFontSize);

    cursor.mergeCharFormat(format);

    int updatedPosition = static_cast<int>(positionRatio * static_cast<float>(vScrollBar->maximum()));
    vScrollBar->setValue(updatedPosition);
}

void MarkdownTextEdit::insertFromMimeData(const QMimeData *source) {
    QString plainText = source->text();
    QTextCursor cursor = this->textCursor();
    cursor.insertText(plainText);
}

void MarkdownTextEdit::focusInEvent(QFocusEvent *e) {
    QMarkdownTextEdit::focusInEvent(e); // Call base class implementation
    emit focusGained(); // Emit the signal
}

void MarkdownTextEdit::search(const QString &searchString) {
    if (searchString != highlighter->getSearchString()) {
        matchStringIndex = -1; // Reset matchStringIndex for a new search string
    }

    QString documentText = this->document()->toPlainText();

    matchStringIndex = documentText.indexOf(searchString, matchStringIndex + 1);
    if (matchStringIndex == -1) {
        matchStringIndex = documentText.indexOf(searchString, matchStringIndex + 1);
        if (matchStringIndex == -1) {
            return;
        }
    }

    QTextCursor cursor = this->textCursor();
    cursor.setPosition(matchStringIndex);
    cursor.setPosition(matchStringIndex + searchString.length(), QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);
    // matchStringIndex += 1;
    highlighter->setSearchString(searchString);
}

void MarkdownTextEdit::searchPrev(const QString &searchString) {
    QString documentText = this->document()->toPlainText();
    matchStringIndex = documentText.lastIndexOf(searchString, matchStringIndex - 1);

    if (matchStringIndex <= -1) {
        matchStringIndex = documentText.length();
        matchStringIndex = documentText.lastIndexOf(searchString);
        if (matchStringIndex == -1) {
            return;
        }
    }
    QTextCursor cursor = this->textCursor();
    cursor.setPosition(matchStringIndex);
    cursor.setPosition(matchStringIndex + searchString.length(), QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);
}

void MarkdownTextEdit::clearSearch() {
    highlighter->setSearchString("");
    matchStringIndex = -1;
}
