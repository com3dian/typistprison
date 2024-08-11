#include "plaintextedit.h"

PlaintextEdit::PlaintextEdit(QWidget *parent)
    : QTextEdit(parent), globalFontSize(12), matchStringIndex(-1)

{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Highlight, QColor("#84e0a5"));
    palette.setColor(QPalette::HighlightedText, QColor("#31363F"));
    this->setPalette(palette);

    highlighter = new PlaintextHighlighter(this->document());
}

void PlaintextEdit::load(const QString &text)
{
    // Clear existing content
    clear();
    QFont font("Noto Mono", globalFontSize); // Set the font to Something, size 12
    this->setFont(font);

    // Load plain text
    this->setPlainText(text);
}

void PlaintextEdit::keyPressEvent(QKeyEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) {
            // If user key press "Ctrl" + "+", increase fontsize
            changeFontSize(1);
            return;
        } else if (event->key() == Qt::Key_Minus) {
            changeFontSize(-1);
            return;
        } else if (event->key() == Qt::Key_F) {
            QString selectedText = textCursor().selectedText();
            emit onPlaintextSearch(selectedText);
            return;
        }
    }
    QTextEdit::keyPressEvent(event);
}

void PlaintextEdit::insertFromMimeData(const QMimeData *source) {
    QString plainText = source->text();
    QTextCursor cursor = this->textCursor();
    cursor.insertText(plainText);
}

void PlaintextEdit::changeFontSize(int delta) {
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

void PlaintextEdit::focusInEvent(QFocusEvent *e) {
    QTextEdit::focusInEvent(e); // Call base class implementation
    emit focusGained(); // Emit the signal
}

void PlaintextEdit::search(const QString &searchString) {
    // Reset matchStringIndex if searchString has changed
    if (searchString != highlighter->getSearchString()) {
        matchStringIndex = -1;
    }

    QString documentText = this->document()->toPlainText();

    // Convert both document text and search string to lower case for case-insensitive comparison
    QString lowerDocumentText = documentText.toLower();
    QString lowerSearchString = searchString.toLower();

    // Perform the case-insensitive search
    matchStringIndex = lowerDocumentText.indexOf(lowerSearchString, matchStringIndex + 1);
    if (matchStringIndex == -1) {
        // Try searching again if the first search didn't find any match
        matchStringIndex = lowerDocumentText.indexOf(lowerSearchString, matchStringIndex + 1);
        if (matchStringIndex == -1) {
            return; // No match found
        }
    }

    // Update matchStringIndex to account for the position in the original document text
    QTextCursor cursor = this->textCursor();
    cursor.setPosition(matchStringIndex);
    cursor.setPosition(matchStringIndex + searchString.length(), QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);

    highlighter->setSearchString(searchString);
}

void PlaintextEdit::searchPrev(const QString &searchString) {
    QString documentText = this->document()->toPlainText();
    
    // Convert both document text and search string to lower case for case-insensitive comparison
    QString lowerDocumentText = documentText.toLower();
    QString lowerSearchString = searchString.toLower();

    // Find the last index of the search string in the document text, case-insensitively
    int searchStartIndex = (matchStringIndex == -1) ? documentText.length() - 1 : matchStringIndex - 1;
    matchStringIndex = lowerDocumentText.lastIndexOf(lowerSearchString, searchStartIndex);

    if (matchStringIndex == -1) {
        // First time search not found, search the entire document
        matchStringIndex = lowerDocumentText.lastIndexOf(lowerSearchString);
        if (matchStringIndex == -1) {
            // Second time search not found
            return;
        }
    }

    // Set the cursor to the position of the match
    QTextCursor cursor = this->textCursor();
    cursor.setPosition(matchStringIndex);
    cursor.setPosition(matchStringIndex + searchString.length(), QTextCursor::KeepAnchor);
    this->setTextCursor(cursor);
}

void PlaintextEdit::clearSearch() {
    highlighter->setSearchString("");
    matchStringIndex = -1;
}
