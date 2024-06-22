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

    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &FictionTextEdit::updateTextColor);
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
    applyBlockFormatting(firstBlock, true);

    // Update the text color for the centered block
    updateTextColor();
}


int FictionTextEdit::getVisibleCenterY() {
    QRect visibleRect = viewport()->rect();
    int centerY = visibleRect.center().y();
    int scrollValue = verticalScrollBar()->value();
    centerY += scrollValue;
    return centerY;
}

int FictionTextEdit::getBlockCenterY(const QTextBlock &block) {
    QRectF blockRect = document()->documentLayout()->blockBoundingRect(block);
    return blockRect.top() + blockRect.height() / 2;
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
        int midY = getBlockCenterY(midBlock);

        if (midY < centerY) {
            low = midBlock.next();
        } else {
            high = midBlock;
        }

        // Break condition to prevent infinite loop
        if (low.blockNumber() == high.blockNumber()) {
            break;
        }
    }

    // Now low is the closest block below or at the center
    // Check the block before and after to find the closest one
    QTextBlock closestBlock = low;
    int minDistance = abs(getBlockCenterY(low) - centerY);

    if (low.previous().isValid()) {
        QTextBlock prevBlock = low.previous();
        int prevDistance = abs(getBlockCenterY(prevBlock) - centerY);
        if (prevDistance < minDistance) {
            closestBlock = prevBlock;
            minDistance = prevDistance;
        }
    }

    if (low.next().isValid()) {
        QTextBlock nextBlock = low.next();
        int nextDistance = abs(getBlockCenterY(nextBlock) - centerY);
        if (nextDistance < minDistance) {
            closestBlock = nextBlock;
        }
    }

    return closestBlock;
}

void FictionTextEdit::updateTextColor() {
    QTextDocument *doc = document();
    int centerY = getVisibleCenterY();

    // Check if the previously centered block is still close to the center
    if (previousCenteredBlock.isValid()) {
        int previousBlockCenterY = getBlockCenterY(previousCenteredBlock);
        int distance = abs(previousBlockCenterY - centerY);

        // Define a threshold for "close enough"
        const int threshold = 20; // Adjust this value as needed

        if (distance <= threshold) {
            return; // The previous centered block is still close enough, no need to update
        }
    }

    // Find the new centered block
    QTextBlock newCenteredBlock = findBlockClosestToCenter();

    // If there was a previously centered block, set it to grey
    if (previousCenteredBlock.isValid()) {
        QTextCursor previousCursor(previousCenteredBlock);
        previousCursor.select(QTextCursor::BlockUnderCursor);

        QTextCharFormat greyFormat = previousCursor.charFormat();
        greyFormat.setForeground(Qt::gray);
        previousCursor.mergeCharFormat(greyFormat);
    }

    // Set the new centered block to white
    QTextCursor newCursor(newCenteredBlock);
    newCursor.select(QTextCursor::BlockUnderCursor);

    QTextCharFormat whiteFormat = newCursor.charFormat();
    whiteFormat.setForeground(Qt::white);
    newCursor.mergeCharFormat(whiteFormat);

    // Update the previously centered block
    previousCenteredBlock = newCenteredBlock;
}
