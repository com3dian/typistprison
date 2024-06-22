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

FictionTextEdit::FictionTextEdit(QWidget *parent)
    : QTextEdit(parent), globalFontSize(12)
{
    setTopMargin(256);
}

void FictionTextEdit::setTopMargin(int margin)
{
    QTextDocument *doc = this->document();
    QTextFrame *rootFrame = doc->rootFrame();

    QTextFrameFormat format = rootFrame->frameFormat();
    format.setTopMargin(margin);
    rootFrame->setFrameFormat(format);
}

void FictionTextEdit::applyBlockFormatting(QTextBlock &block, bool isFirstBlock)
{
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
    if (textCursor.blockNumber() == 0) {
        // if edit the first block
        applyBlockFormatting(firstBlock, true);

        // If hit enter
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            textCursor.insertBlock();
            QTextBlock newBlock = textCursor.block();
            applyBlockFormatting(newBlock, false);

        } else {
            QTextEdit::keyPressEvent(event);
        }
    } else {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            textCursor.insertBlock();
            QTextBlock newBlock = textCursor.block();
            applyBlockFormatting(newBlock, false);
        } else {
            QTextEdit::keyPressEvent(event);
        }
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
    applyBlockFormatting(firstBlock, true);

    // Iterate through each line
    for (int i = 1; i < lines.size(); ++i) {
        // Insert a block for each line
        cursor.insertBlock();
        QTextBlock newBlock = cursor.block();

        // Apply formatting
        applyBlockFormatting(newBlock, false);

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
        applyBlockFormatting(newBlock, cursor.blockNumber() == 0);
    }
}

void FictionTextEdit::changeFontSize(int delta)
{
    globalFontSize += delta;
    if (globalFontSize < 1) {
        globalFontSize = 1; // Prevent font size from becoming too small
    }

    QTextCursor cursor(document());
    cursor.select(QTextCursor::Document);
    QTextCharFormat format;
    format.setFontPointSize(globalFontSize);
    cursor.setCharFormat(format);


    // Update all blocks with the new font size
    QTextBlock firstBlock = document()->firstBlock();
    applyBlockFormatting(firstBlock, true);
}

// Helper function to get the center Y position of the visible area
int getVisibleCenterY(QTextEdit *textEdit) {
    QRect visibleRect = textEdit->viewport()->rect();
    int centerY = visibleRect.center().y();
    int scrollValue = textEdit->verticalScrollBar()->value();
    centerY += scrollValue;
    return centerY;
}

// Helper function to get the Y position of a block
int getBlockCenterY(QTextEdit *textEdit, const QTextBlock &block) {
    QRectF blockRect = textEdit->document()->documentLayout()->blockBoundingRect(block);
    return blockRect.top() + blockRect.height() / 2;
}

// Function to find the block closest to the center of the visible area
QTextBlock findBlockClosestToCenter(QTextEdit *textEdit) {
    QTextDocument *document = textEdit->document();
    int centerY = getVisibleCenterY(textEdit);

    QTextBlock low = document->firstBlock();
    QTextBlock high = document->lastBlock();

    // Binary search to narrow down the range
    while (low.blockNumber() <= high.blockNumber()) {
        int midBlockNumber = (low.blockNumber() + high.blockNumber()) / 2;
        QTextBlock midBlock = document->findBlockByNumber(midBlockNumber);
        int midY = getBlockCenterY(textEdit, midBlock);

        if (midY < centerY) {
            low = midBlock.next();
        } else {
            high = midBlock.previous();
        }
    }

    // Now low is the first block below the center, high is the last block above the center
    // Check the blocks around the center to find the closest one
    QTextBlock closestBlock = low;
    int minDistance = INT_MAX;

    for (QTextBlock block = low.previous(); block.isValid(); block = block.previous()) {
        int blockCenterY = getBlockCenterY(textEdit, block);
        int distance = abs(blockCenterY - centerY);

        if (distance < minDistance) {
            minDistance = distance;
            closestBlock = block;
        }

        // Stop if blocks are further away
        if (blockCenterY < centerY && distance > minDistance) {
            break;
        }
    }

    for (QTextBlock block = low; block.isValid(); block = block.next()) {
        int blockCenterY = getBlockCenterY(textEdit, block);
        int distance = abs(blockCenterY - centerY);

        if (distance < minDistance) {
            minDistance = distance;
            closestBlock = block;
        }

        // Stop if blocks are further away
        if (blockCenterY > centerY && distance > minDistance) {
            break;
        }
    }

    return closestBlock;
}
