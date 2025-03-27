#include "prisonerfictiontextedit.h"

PrisonerFictionTextEdit::PrisonerFictionTextEdit(QWidget *parent, ProjectManager *projectManager)
    : FictionTextEdit(parent),
      isPrisoner(false)
{
    // Initialize additional features if needed
}

void PrisonerFictionTextEdit::activateSniperMode() {
    if (isPrisoner){
        return;
    }
    isSniperMode = true;
    QColor customColor("#454F61"); // gray color
    changeGlobalTextColor(customColor); // Change all text to gray

    // Update the previously centered block to the new highlight color
    updateFocusBlock();

    // Connect the scroll bar value change to the updateFocusBlock method
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateFocusBlock);
}

void PrisonerFictionTextEdit::deactivateSniperMode() {
    isSniperMode = false;
    changeGlobalTextColor(Qt::white); // Change all text to white

    disconnect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateFocusBlock);
}

void PrisonerFictionTextEdit::keyPressEvent(QKeyEvent *event)
{
    emit keyboardInput();
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Equal) {
            changeFontSize(1);
            return; // Return early to avoid further processing
        } else if (event->key() == Qt::Key_Minus) {
            changeFontSize(-1);
            return; // Return early to avoid further processing
        } else if (event->key() == Qt::Key_F) {
            QString selectedText = textCursor().selectedText();
            emit onFictionEditSearch(selectedText);
            return; // Return early to avoid further processing
        } else if (event->key() == Qt::Key_Z) {
            if (this->toPlainText().isEmpty()) {
                return;
            } else {
                undo();
                return;
            }
        } else if (event->key() == Qt::Key_Escape) {
            qDebug() << "escaping!";
            emit onEscape();
        }
        // // QTextEdit::keyPressEvent(event);
        // return;
    }

    // If the handler is at bottom and user hit enter, attach handler to bottom
    QTextCursor textCursor = this->textCursor();
    QTextBlock firstBlock = document()->firstBlock();
    QTextBlock secondBlock = firstBlock.next();
    bool isAtBottom = false;

    // Check if scrollbar exists and if it is at the bottom
    QScrollBar *vScrollBar = verticalScrollBar();
    if (vScrollBar) {
        isAtBottom = vScrollBar->value() == vScrollBar->maximum();
    }

    // If hit enter
    // Important: Do not change the following if{...}else{...}
    //            Otherwise in empty line hitting ENTER wno't insert new line
    // *** Need Fix ***
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)) {
        QTextCursor textCursorSmall = applyCharFormatting4NextBlock(textCursor);

        textCursorSmall.insertBlock();
        QTextBlock newBlock = textCursorSmall.block();
        applyBlockFormatting(newBlock);
        if (isAtBottom) {
            // Attach handler to bottom
            vScrollBar->setValue(vScrollBar->maximum());
        }
    } else {
        QTextEdit::keyPressEvent(event);
    }

    // Update the text color for the centered block
    if (isSniperMode) {
        updateFocusBlock();
    }
}


// Optionally, override other functions or add new features
