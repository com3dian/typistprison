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

#include <QtConcurrent/QtConcurrent>

FictionTextEdit::FictionTextEdit(QWidget *parent)
    : QTextEdit(parent), globalFontSize(12)
{
    setTopMargin(256);

    // connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateTextColor);
    isHighlightMode = false;
}

void FictionTextEdit::setTopMargin(int margin)
{
    QTextDocument *doc = this->document();
    QTextFrame *rootFrame = doc->rootFrame();

    QTextFrameFormat format = rootFrame->frameFormat();
    format.setTopMargin(margin);
    rootFrame->setFrameFormat(format);
}

void FictionTextEdit::applyBlockFormatting(QTextBlock &block, bool isCheck)
{
    bool isFirstBlock = block.blockNumber() == 0;
    // Create a cursor at the start of the block
    QTextCursor cursor(block);
    bool emptyblock = block.text().isEmpty();
    if (emptyblock) {
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.insertText("+");
    }

    // Set block format
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setTopMargin(0); // Set the top margin to 10 pixels
    blockFormat.setBottomMargin(32); // Set the bottom margin to 32 pixels
     // TODO: scale the margin

    blockFormat.setLeftMargin(0);
    blockFormat.setRightMargin(0);
    cursor.setBlockFormat(blockFormat);

    // Set character format
    // TODO: system fontsize
    QTextCharFormat charFormat;
    charFormat.setFontPointSize(isFirstBlock ? globalFontSize + 8 : globalFontSize);

    if (isHighlightMode && isCheck) {
        int centerY = getVisibleCenterY();
        QRectF blockRect = document()->documentLayout()->blockBoundingRect(block);

        QTextBlock previousBlock = block.previous();
        QRectF previousblockRect = document()->documentLayout()->blockBoundingRect(previousBlock);

        if ((blockRect.top() - 16 <= centerY) && (centerY <= blockRect.bottom() + 16)) {
            charFormat.setForeground(Qt::white);
            previousCenteredBlock = block;
        } else {
            charFormat.setForeground(Qt::gray);
        }
    } else {
        charFormat.setForeground(Qt::white);
    }

    // Apply the character format to the whole block
    cursor.select(QTextCursor::BlockUnderCursor);
    cursor.setCharFormat(charFormat);

    if (emptyblock) {
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.deleteChar();
    }
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
        }
    }

    QTextCursor textCursor = this->textCursor();
    QTextBlock firstBlock = document()->firstBlock();
    bool isAtBottom = false;

    // Check if scrollbar exists and if it is at the bottom
    QScrollBar *vScrollBar = verticalScrollBar();
    if (vScrollBar) {
        isAtBottom = vScrollBar->value() == vScrollBar->maximum();
    }

    if (textCursor.blockNumber() == 0) {
        // if edit the first block
        applyBlockFormatting(firstBlock);
    }

    // If hit enter
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        textCursor.insertBlock();
        QTextBlock newBlock = textCursor.block();
        applyBlockFormatting(newBlock);

        // Attach handler to bottom
        if (isAtBottom) {
            vScrollBar->setValue(vScrollBar->maximum());
        }
    } else {
            QTextEdit::keyPressEvent(event);
    }
}

void FictionTextEdit::load(const QString &text)
{
    // Clear existing content
    clear();

    // Split the input text by line breaks
    QStringList lines = text.split("\n");

    QTextCursor cursor(document());
    cursor.insertText(lines[0]);

    // First Block
    QTextBlock firstBlock = document()->firstBlock();
    applyBlockFormatting(firstBlock);

    // Iterate through each line
    for (int i = 1; i < lines.size(); ++i) {
        // Insert a block for each line
        cursor.insertBlock();
        QTextBlock newBlock = cursor.block();

        // Apply formatting
        applyBlockFormatting(newBlock);

        // Insert the line text
        cursor.insertText(lines[i]);

        // Move to the end of the block for the next iteration
        cursor.movePosition(QTextCursor::EndOfBlock);
    }
    cursor.movePosition(QTextCursor::Start);
    this->setTextCursor(cursor);
}

void FictionTextEdit::insertFromMimeData(const QMimeData *source)
{
    // Get plain text from the source
    QString plainText = source->text();

    // Split the plain text by line breaks
    QStringList lines = plainText.split('\n');

    // Get the current text cursor
    QTextCursor cursor = this->textCursor();

    // Insert and format each line
    for (int i = 0; i <lines.size(); ++i){
        if (i >= 1){
            cursor.insertBlock();
        }
        QTextBlock newBlock = cursor.block();
        cursor.insertText(lines[i]);
        applyBlockFormatting(newBlock);
    }
}

void FictionTextEdit::changeFontSize(int delta) {
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
    if (isHighlightMode) {
        updateFocusBlock();
    }
}


int FictionTextEdit::getVisibleCenterY() {
    QRect visibleRect = viewport()->rect();
    int centerY = visibleRect.center().y();
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
}

// Function to find the block closest to the center of the visible area
QTextBlock FictionTextEdit::findBlockClosestToCenter() {
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
        if (position == "On") {
            return; // The previous centered block is still close enough, no need to update
        }
    }

    // If there was a previously centered block, set it to grey
    if (previousCenteredBlock.isValid()) {
        applyBlockFormatting(previousCenteredBlock);
    }


    // check the prevous & next blocks
    std::string positionPrevious = checkVisibleCenterBlock(previousCenteredBlock.previous());
    if (positionPrevious == "On") {
        newCenteredBlock = previousCenteredBlock.previous();
        applyBlockFormatting(newCenteredBlock);
        previousCenteredBlock = newCenteredBlock;
    }
    std::string positionNext = checkVisibleCenterBlock(previousCenteredBlock.next());
    if (positionNext == "On") {
        newCenteredBlock = previousCenteredBlock.next();
        applyBlockFormatting(newCenteredBlock);
        previousCenteredBlock = newCenteredBlock;
    }

    // Find the new centered block
    newCenteredBlock = findBlockClosestToCenter();
    // Set the new centered block to white
    applyBlockFormatting(newCenteredBlock);

    // Update the previously centered block
    previousCenteredBlock = newCenteredBlock;
}

void FictionTextEdit::changeGlobalTextColor(const QColor &color) {
    
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

void FictionTextEdit::activateHighlightMode() {
    isHighlightMode = true;
    changeGlobalTextColor(Qt::gray); // Change all text to gray

    // Update the previously centered block to the new highlight color
    updateFocusBlock();

    // Connect the scroll bar value change to the updateFocusBlock method
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateFocusBlock);
}

void FictionTextEdit::deactivateHighlightMode() {
    isHighlightMode = false;
    changeGlobalTextColor(Qt::white); // Change all text to white

    disconnect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateFocusBlock);
}
