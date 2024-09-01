#include "fictiontextedit.h"
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
#include <QPalette>
#include <QStringMatcher>
#include <QtConcurrent/QtConcurrent>


FictionTextEdit::FictionTextEdit(QWidget *parent)
    : QTextEdit(parent), globalFontSize(12), matchStringIndex(-1)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Highlight, QColor("#84e0a5"));
    palette.setColor(QPalette::HighlightedText, QColor("#31363F"));
    this->setPalette(palette);

    setUndoRedoEnabled(false);
    setTopMargin(256);

    // connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateTextColor);
    isSniperMode = false;
    setUndoRedoEnabled(true);

    highlighter = new FictionHighlighter(this->document());
}

void FictionTextEdit::setTopMargin(int margin)
{
    QTextDocument *doc = this->document();
    QTextFrame *rootFrame = doc->rootFrame();

    QTextFrameFormat format = rootFrame->frameFormat();
    format.setTopMargin(margin);
    rootFrame->setFrameFormat(format);
}

void FictionTextEdit::applyBlockFormatting(QTextBlock &block)
{
    bool isFirstBlock = block.blockNumber() == 0;

    // Create a cursor at the start of the block
    QTextCursor cursor(block);

    // Set block format
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setTopMargin(0); // Set the top margin to 10 pixels
    blockFormat.setBottomMargin(32); // Set the bottom margin
    // TODO: scale the margin

    blockFormat.setLeftMargin(16);
    blockFormat.setRightMargin(16);
    cursor.setBlockFormat(blockFormat);

    // Set character format
    // Apply the character format to the whole block
    cursor.select(QTextCursor::BlockUnderCursor);
    cursor = applyCharFormatting(cursor);
}

QTextCursor FictionTextEdit::applyCharFormatting(QTextCursor &cursor, bool insertLargeFont)
{
    bool isFirstBlock = cursor.blockNumber() == 0;

    QTextCharFormat charFormat = cursor.charFormat();  // Get current char format

    QFont font = charFormat.font();
    font.setPointSize((isFirstBlock && insertLargeFont) ? (1.6 * globalFontSize) : globalFontSize);  // Adjust font size
    charFormat.setFont(font);

    if (isSniperMode) {
        int centerY = getVisibleCenterY();
        QRectF blockRect = document()->documentLayout()->blockBoundingRect(cursor.block());

        if ((blockRect.top() - 16 <= centerY) && (centerY <= blockRect.bottom() + 16)) {
            charFormat.setForeground(Qt::white);
            // previousCenteredBlock = cursor.block();  // Update previous centered block
        } else {
            QColor customColor("#454F61");
            charFormat.setForeground(customColor);
        }
    } else {
        charFormat.setForeground(Qt::white);
    }

    cursor.setCharFormat(charFormat);  // Apply the new char format to the cursor

    return cursor;
}


void FictionTextEdit::keyPressEvent(QKeyEvent *event)
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
            emit onFictionEditSearch(selectedText);
            return; // Return early to avoid further processing
        } else if (event->key() == Qt::Key_Z) {
            qDebug() << "ctrl + Z";
            qDebug() << this->toPlainText().isEmpty();
            if (this->toPlainText().isEmpty()) {
                qDebug() << "empty";
                return;
            } else {
                undo();
                return;
            }
        } else if (event->key() == Qt::Key_S) {
            emit onSave();
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
        qDebug() << "key press ENTER";
        QTextCursor textCursorSmall = applyCharFormatting(textCursor, false);
        textCursorSmall.insertBlock();
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

void FictionTextEdit::load(const QString &text)
{
    // Clear existing content
    clear();

    // Set font format
    QTextCursor cursor = this->textCursor();
    QFont font("Noto Sans CJK SC Light", 1.6 * globalFontSize); // If we need special font

    // Set cursor format
    QTextCharFormat charFormat = cursor.charFormat();
    charFormat.setFont(font);
    cursor.setCharFormat(charFormat);

    if (text.isEmpty()) {
        // Set the modified cursor back to the text edit
        this->setTextCursor(applyCharFormatting(cursor));
        QTextBlock newBlock = cursor.block();
        // Apply formatting
        applyBlockFormatting(newBlock);

    } else {
        // Split the input text by line breaks
        QStringList lines = text.split("\n");

        QTextCursor cursor = this->textCursor();
        cursor.insertText(lines[0]);

        // First Block
        QTextBlock firstBlock = document()->firstBlock();
        applyBlockFormatting(firstBlock);

        // Iterate through each line
        for (int i = 1; i < lines.size(); ++i) {
            // Insert a block for each line
            cursor.insertBlock();
            QTextBlock newBlock = cursor.block();

            // Insert the line text
            cursor.insertText(lines[i]);

            // Apply formatting
            applyBlockFormatting(newBlock);

            // Move to the end of the block for the next iteration
            cursor.movePosition(QTextCursor::EndOfBlock);
        }
        cursor.movePosition(QTextCursor::Start);
        this->setTextCursor(cursor);
    }
}

void FictionTextEdit::insertFromMimeData(const QMimeData *source)
{
    qDebug() << "insertFromMimeData";
    // Get plain text from the source
    QString plainText = source->text();

    // If empty return
    if (plainText.isEmpty()) {
        return;
    }

    // Get the current text cursor
    QTextCursor cursor = this->textCursor();

    QTextBlock firstBlock = this->document()->firstBlock();
    QTextBlock currentBlock = cursor.block();
    if ((cursor.blockNumber() != 0) || (not plainText.contains('\n'))) {
        // direct insert if not in first block
        qDebug() << "direct insertion";
        cursor.insertText(plainText);
        QTextBlock newBlock = cursor.block();
        applyBlockFormatting(newBlock);
    } else {
        // Split the plain text by line breaks
        QStringList lines = plainText.split('\n');
        for (int i = 0; i <lines.size(); ++i){
            if (i == 0) {
                cursor.insertText(lines[i]);
                qDebug() << "insert lines 0";
            } else if (i == 1) {
                qDebug() << "insert lines 1";
                cursor = applyCharFormatting(cursor, false);
                cursor.insertBlock();
                QTextBlock newBlock = cursor.block();
                cursor.insertText(lines[i]);
            } else {
                qDebug() << "insert lines 1+";
                QStringList sublines = lines.mid(i+1);
                qDebug() << sublines.join('\n');
                QString plainTextTail = '\n' + sublines.join('\n');
                cursor.insertText(plainTextTail);

                return;
            }
        }
    }
}

void FictionTextEdit::changeFontSize(int delta) {
    highlighter->changeFontSize(delta);
    // set default font
    QFont font = this->font();
    font.setPointSize(1.6 * globalFontSize); // Set default font size
    this->setFont(font);

    QScrollBar *vScrollBar = verticalScrollBar();
    int scrollBarPosition = vScrollBar->value();
    float positionRatio = static_cast<float>(scrollBarPosition)/static_cast<float>(vScrollBar->maximum());

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

    // Ensure the first block is formatted correctly
    QTextBlock firstBlock = document()->firstBlock();
    applyBlockFormatting(firstBlock);

    // Update the text color for the centered block
    if (isSniperMode) {
        updateFocusBlock();
    }

    int updatedPosition = static_cast<int>(positionRatio * static_cast<float>(vScrollBar->maximum()));
    qDebug() << updatedPosition << vScrollBar->maximum();

    // TODO: use findBlockClosestToCenter to find the most centered block->first token, 
    // TODO: then set that token to the same position on the screen.
    vScrollBar->setValue(updatedPosition);
}


int FictionTextEdit::getVisibleCenterY() {
    QRect visibleRect = viewport()->rect();
    int centerY = visibleRect.top() * 0.55 + visibleRect.bottom() * 0.45;
    // int centerY = visibleRect.center().y();
    int scrollValue = verticalScrollBar()->value();
    centerY += scrollValue;
    return centerY;
}

std::string FictionTextEdit::checkVisibleCenterBlock(const QTextBlock &block) {
    QRectF blockRect = document()->documentLayout()->blockBoundingRect(block);
    int centerY = getVisibleCenterY();

    if ((blockRect.top() - 16 <= centerY) && (blockRect.bottom() + 16 >= centerY)) {
        return "In";
    } else if (blockRect.top() - 16 > centerY) {
        return "On top";
    } else if (blockRect.bottom() + 16 < centerY) {
        return "On bottom";
    }
    return "make gcc happy";
}

// Function to find the block closest to the center of the visible area
QTextBlock FictionTextEdit::findBlockClosestToCenter() {
    qDebug() << "binary";
    QTextDocument *doc = document();
    int centerY = getVisibleCenterY();

    QTextBlock low = doc->firstBlock();
    QTextBlock high = doc->lastBlock();

    if (!low.isValid() || !high.isValid()) {
        return QTextBlock(); // Return an invalid block if there are no blocks
    }

    // Ensure the binary search terminates properly
    while (low.blockNumber() < high.blockNumber()) {
        int midBlockNumber = (low.blockNumber() + high.blockNumber()) / 2;
        QTextBlock midBlock = doc->findBlockByNumber(midBlockNumber);
        std::string position = checkVisibleCenterBlock(midBlock);
        if (position == "In") {
            return midBlock;
        } else if (position == "On top") {
            high = midBlock;
        } else if (position == "On bottom") {
            low = midBlock.next();
        }

        // Break condition to prevent infinite loop
        if (low.blockNumber() == high.blockNumber()) {
            break;
        }
    }

    // Now low is the closest block below or at the center
    // Check the block before and after to find the closest one
    QTextBlock closestBlock = low;
    std::string position = checkVisibleCenterBlock(closestBlock);
    if (position == "In") {
        return closestBlock;
    } else if (position == "On top") {
        if (closestBlock.previous().isValid()) {
            return  closestBlock.previous();
        } else {
            return closestBlock;
        }
        
    } else if (position == "On bottom") {
        if (closestBlock.next().isValid()) {
            return closestBlock.next();
        } else {
            return closestBlock;
        }
    }
}

void FictionTextEdit::updateFocusBlock() {
    int centerY = getVisibleCenterY();

    // Check if the previously centered block is still close to the center
    if (previousCenteredBlock.isValid()) {
        std::string position = checkVisibleCenterBlock(previousCenteredBlock);
        qDebug() << position;
        if (position == "In") {

            return; // The previous centered block is still close enough, no need to update
        }
    }

    // If there was a previously centered block, set it to grey
    if (previousCenteredBlock.isValid()) {
        applyBlockFormatting(previousCenteredBlock);
    }

    // check the prevous & next blocks
    std::string positionPrevious = checkVisibleCenterBlock(previousCenteredBlock.previous());
    if (positionPrevious == "In") {
        newCenteredBlock = previousCenteredBlock.previous();
        applyBlockFormatting(newCenteredBlock);
        previousCenteredBlock = newCenteredBlock;
        return;
    }
    std::string positionNext = checkVisibleCenterBlock(previousCenteredBlock.next());
    if (positionNext == "In") {
        newCenteredBlock = previousCenteredBlock.next();
        applyBlockFormatting(newCenteredBlock);
        previousCenteredBlock = newCenteredBlock;
        return;
    }

    // Find the new centered block
    newCenteredBlock = findBlockClosestToCenter();
    // Set the new centered block to white
    applyBlockFormatting(newCenteredBlock);

    // Update the previously centered block
    previousCenteredBlock = newCenteredBlock;
}

void FictionTextEdit::changeGlobalTextColor(const QColor &color)
{
    // Select the entire document
    QTextCursor cursor(this->document());
    cursor.select(QTextCursor::Document);

    // Get the current character format of the selected text
    QTextCharFormat format;
    format.setForeground(color);

    // Merge the new format with the existing format to preserve colors
    cursor.mergeCharFormat(format);

    // Ensure the centered block is formatted correctly
    newCenteredBlock = findBlockClosestToCenter();

    applyBlockFormatting(newCenteredBlock);
    previousCenteredBlock = newCenteredBlock;
}

void FictionTextEdit::activateSniperMode() {
    isSniperMode = true;
    QColor customColor("#454F61"); // gray color
    changeGlobalTextColor(customColor); // Change all text to gray

    // Update the previously centered block to the new highlight color
    updateFocusBlock();

    // Connect the scroll bar value change to the updateFocusBlock method
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateFocusBlock);
}

void FictionTextEdit::deactivateSniperMode() {
    isSniperMode = false;
    changeGlobalTextColor(Qt::white); // Change all text to white

    disconnect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateFocusBlock);
}

void FictionTextEdit::focusInEvent(QFocusEvent *e) {
    QTextEdit::focusInEvent(e); // Call base class implementation
    emit focusGained(); // Emit the signal
}

void FictionTextEdit::search(const QString &searchString) {
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

void FictionTextEdit::searchPrev(const QString &searchString) {
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

void FictionTextEdit::clearSearch() {
    highlighter->setSearchString("");
    matchStringIndex = -1;
}