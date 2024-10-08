#include "plaintextedit.h"

PlaintextEdit::PlaintextEdit(QWidget *parent)
    : QTextEdit(parent), helper(new TextEditHelper(this)), globalFontSize(12) 

{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Highlight, QColor("#84e0a5"));
    palette.setColor(QPalette::HighlightedText, QColor("#31363F"));
    this->setPalette(palette);
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
            helper->changeFontSize(1);
            return;
        } else if (event->key() == Qt::Key_Minus) {
            helper->changeFontSize(-1);
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

void PlaintextEdit::focusInEvent(QFocusEvent *e) {
    QTextEdit::focusInEvent(e); // Call base class implementation
    emit focusGained(); // Emit the signal
}

void PlaintextEdit::search(const QString &searchString) {
    helper->search(searchString);
}

void PlaintextEdit::searchPrev(const QString &searchString) {
    helper->searchPrev(searchString);
}

void PlaintextEdit::clearSearch() {
    helper->clearSearch();
}
