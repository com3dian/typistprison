#include "plaintextedit.h"
#include "searchWidget.h"
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextBlock>
#include <QMimeData>
#include <QKeyEvent>
#include <QScrollBar>
#include <QAbstractTextDocumentLayout>
#include <QDebug>
#include <QList>
#include <string>

#include <QtConcurrent/QtConcurrent>

PlaintextEdit::PlaintextEdit(QWidget *parent)
    : QTextEdit(parent), globalFontSize(12), highlighter(new SearchHighlighter(document()))
{

}

void PlaintextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) {
            changeFontSize(1);
            return; // Return early to avoid further processing
        } else if (event->key() == Qt::Key_Minus) {
            changeFontSize(-1);
            return; // Return early to avoid further processing
        } else if (event->key() == Qt::Key_F) {
            QString selectedText = textCursor().selectedText();
            qDebug() << "selectedText: " << selectedText ;
            emit onPlaintextSearch(selectedText);
            return; // Return early to avoid further processing
        }
    }
    QTextEdit::keyPressEvent(event);
}

void PlaintextEdit::load(const QString &text)
{
    // Clear existing content
    clear();

    // Load plain text
    this->setPlainText(text);
}

void PlaintextEdit::changeFontSize(int delta) {
    QScrollBar *vScrollBar = verticalScrollBar();
    int scrollBarPosition = vScrollBar->value();
    float positionRatio = static_cast<float>(scrollBarPosition)/vScrollBar->maximum();
    qDebug() << "ratio: " << positionRatio << "bar maximum: " << vScrollBar->maximum();

    globalFontSize += delta;
    if (globalFontSize < 1) {
        globalFontSize = 1; // Prevent font size from becoming too small
    }

    // Select the entire document
    QTextCursor cursor(document());
    cursor.select(QTextCursor::Document);

    // Get the current character format of the selected text
    QTextCharFormat format;
    format.setFontPointSize(globalFontSize);

    // Merge the new format with the existing format to preserve colors
    cursor.mergeCharFormat(format);

    int updatedPosition = static_cast<int>(positionRatio * vScrollBar->maximum());
    vScrollBar->setValue(updatedPosition);
}

void PlaintextEdit::search(const QString &searchString) {
    highlighter->setSearchString(searchString);
}

void PlaintextEdit::clearSearch() {
    highlighter->setSearchString("");
}