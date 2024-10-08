#include "fictiontextedit.h"

FictionTextEdit::FictionTextEdit(QWidget *parent)
    : QTextEdit(parent), 
    globalFontSize(12), 
    matchStringIndex(-1)
    // cursorVisible(true)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Highlight, QColor("#84e0a5"));
    palette.setColor(QPalette::HighlightedText, QColor("#31363F"));
    this->setPalette(palette);

    setUndoRedoEnabled(false); // forbid undo
                               // do our settings
    setTopMargin(256);         // set top margin as 256

    isSniperMode = false;      // default not sniperMode

    // set `Noto Sans Regular` as default font for fictiontextedit 
    QString notoSansRegularFamily = FontManager::instance().getNotoSansRegularFamily();
    QFont font(notoSansRegularFamily, globalFontSize);
    this->setFont(font);
    this->setCursorWidth(2);

    // To be discussed (Between me and me):
    // Do we want a colored blinking cursor (carent)?
    // cursorTimer = new QTimer(this);
    // connect(cursorTimer, &QTimer::timeout, this, &FictionTextEdit::toggleCursorVisibility);
    // cursorTimer->setInterval(750); // Blink every 750 milliseconds
    // cursorTimer->start();
    setUndoRedoEnabled(true); // allow undo

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

QTextCursor FictionTextEdit::applyCharFormatting4NextBlock(QTextCursor &cursor)
{
    bool isFirstBlock = cursor.blockNumber() == 0;

    QTextCharFormat charFormat = cursor.charFormat();  // Get current char format

    QFont font = charFormat.font();
    font.setPointSize(globalFontSize);  // Adjust font size
    charFormat.setFont(font);


    // Check if scrollbar exists and if it is at the bottom
    // If yes then this is a special case
    QScrollBar *vScrollBar = verticalScrollBar();
    bool isAtBottom;
    if (vScrollBar) {
        isAtBottom = vScrollBar->value() == vScrollBar->maximum();
    } else {
        isAtBottom = false;
    }

    if (isSniperMode) {
        int centerY = getVisibleCenterY();

        QFontMetricsF fontMetrics(font);

        // Calculate the height of the text (ascent + descent)
        qreal fontHeight = fontMetrics.height();
        qreal totalHeight = fontHeight + 32;

        QRectF blockRect = document()->documentLayout()->blockBoundingRect(cursor.block());
        
        if (isAtBottom) {
            if ((blockRect.bottom() + 16 - totalHeight <= centerY) && (centerY <= blockRect.bottom() + 16)) {
                qDebug() << "@@@@@@@@@@@@@@@@@@@@1111111111111";
                charFormat.setForeground(Qt::white);
            } else {
                qDebug() << "@@@@@@@@@@@@@@@@@@@@2222222222222";
                QColor customColor("#454F61");
                charFormat.setForeground(customColor);
            }
        } else {
            if ((blockRect.bottom() + 16 <= centerY) && (centerY <= blockRect.bottom() + totalHeight + 16)) {
                qDebug() << "@@@@@@@@@@@@@@@@@@@@3333333333333";
                charFormat.setForeground(Qt::white);
                // previousCenteredBlock = cursor.block();  // Update previous centered block
            } else {
                qDebug() << "@@@@@@@@@@@@@@@@@@@@4444444444444";
                QColor customColor("#454F61");
                charFormat.setForeground(customColor);
            }
        }
    } else {
        charFormat.setForeground(Qt::white);
    }

    cursor.setCharFormat(charFormat);  // Apply the new char format to the cursor

    return cursor;
}

// void FictionTextEdit::paintEvent(QPaintEvent *event) {
//     QTextEdit::paintEvent(event);

//     // Drawing a custom cursor
//     if (hasFocus()) {
//         QPainter painter(viewport());
//         QTextCursor cursor = textCursor();
        
//         // Get the rectangle representing the cursor's position
//         QRect cursorRect = this->cursorRect();

//         // Adjust the cursor width as desired
//         int cursorWidth = 2;
//         cursorRect.setWidth(cursorWidth);
//         QColor cursorColor;
//         if (cursorVisible) {
//             cursorColor = QColor("#84e0a5");  // Cursor color when visible
//         } else {
//             cursorColor = QColor("#31363F");
//         }
//         painter.fillRect(cursorRect, cursorColor);
//     }
// }

// void FictionTextEdit::toggleCursorVisibility() {
//     cursorVisible = !cursorVisible;  // Toggle cursor visibility
//     update();  // Trigger a repaint
// }

// void FictionTextEdit::focusOutEvent(QFocusEvent *event) {
//     QTextEdit::focusOutEvent(event);
//     cursorTimer->stop();  // Stop blinking when focus is lost
//     cursorVisible = true; // Ensure cursor is visible when focus is lost
//     update(); // Trigger a repaint to hide the cursor
// }

void FictionTextEdit::keyPressEvent(QKeyEvent *event)
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

void FictionTextEdit::load(const QString &text)
{
    // Clear existing content
    clear();

    // Set font format
    QTextCursor cursor = this->textCursor();
    QFont font  = this->font();
    font.setPointSize(globalFontSize);

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
    // Find the most centered block
    QTextBlock centerBlock = findBlockClosestToCenter();
    QRectF centerBlockRect = document()->documentLayout()->blockBoundingRect(centerBlock);
    int centerBlockTop = centerBlockRect.top();
    int centerBlockBottom = centerBlockRect.bottom();
    int centerBlockMiddle = static_cast<int>(centerBlockTop + (centerBlockBottom - centerBlockTop) / 2);
    // // Get the scroll position to calculate the distance from the top of the document
    // int scrollPosition = verticalScrollBar()->value();

    // // Calculate the relative position of the center block
    // int relativePosition = centerBlockMiddle + scrollPosition;
    // qDebug() << "relativePosition" << relativePosition;


    highlighter->changeFontSize(delta);
    // set default font
    QFont font = this->font();
    font.setPointSize(globalFontSize); // Set default font size
    this->setFont(font);

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

    // After changing the font size, scroll to keep the old center block in view
    QRectF newCenterBlockRect = document()->documentLayout()->blockBoundingRect(centerBlock);
    int newCenterBlockTop = newCenterBlockRect.top();
    int newCenterBlockBottom = newCenterBlockRect.bottom();
    int newCenterBlockMiddle = static_cast<int>(newCenterBlockTop + (newCenterBlockBottom - newCenterBlockTop) / 2);

    // Calculate the relative position of the center block
    int halfViewportHeight = (viewport()->height() - 256) / 2;
    verticalScrollBar()->setValue(newCenterBlockMiddle - halfViewportHeight);
}


int FictionTextEdit::getVisibleCenterY() {
    QRect visibleRect = viewport()->rect();
    int centerY = visibleRect.top() * 0.58 + visibleRect.bottom() * 0.42;
    // int centerY = visibleRect.center().y();
    int scrollValue = verticalScrollBar()->value();
    centerY += scrollValue;
    return centerY;
}

int FictionTextEdit::checkVisibleCenterBlock(const QTextBlock &block) {
    QRectF blockRect = document()->documentLayout()->blockBoundingRect(block);
    int centerY = getVisibleCenterY();

    if ((blockRect.top() - 16 <= centerY) && (blockRect.bottom() + 16 >= centerY)) {
        return 0;

    } else if (blockRect.top() - 16 > centerY) {
        return centerY - blockRect.top(); // positive value
        // current block is below the middleline

    } else if (blockRect.bottom() + 16 < centerY) {
        return centerY - blockRect.bottom(); // negative value
    }
    return 0; // make compiler happy
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

    QRectF lowBlockRect = document()->documentLayout()->blockBoundingRect(low);
    QRectF highBlockRect = document()->documentLayout()->blockBoundingRect(high);

    // Ensure the binary search terminates properly
    while (low.blockNumber() < high.blockNumber()) {
        int midBlockNumber;

        QTextBlock midBlock = doc->findBlockByNumber(midBlockNumber);
        int position = checkVisibleCenterBlock(midBlock);
        if (position == 0) {
            return midBlock;
        } else if (position < 0) {
            high = midBlock;
            highBlockRect = document()->documentLayout()->blockBoundingRect(high);
            midBlockNumber = 
                high.blockNumber() +  (high.blockNumber() - low.blockNumber()) * position / (highBlockRect.top() - lowBlockRect.top());


        } else if (position > 0) {
            low = midBlock.next();
            lowBlockRect = document()->documentLayout()->blockBoundingRect(low);

            midBlockNumber = 
                low.blockNumber() +  (high.blockNumber() - low.blockNumber()) * position / (highBlockRect.top() - lowBlockRect.top());
        }

        // Break condition to prevent infinite loop
        if (low.blockNumber() == high.blockNumber()) {
            break;
        }
    }

    // Now low is the closest block below or at the center
    // Check the block before and after to find the closest one
    QTextBlock closestBlock = low;
    int position = checkVisibleCenterBlock(closestBlock);
    if (position == 0) {
        return closestBlock;
    } else if (position < 0) {
        if (closestBlock.previous().isValid()) {
            return  closestBlock.previous();
        } else {
            return closestBlock;
        }
        
    } else if (position > 0) {
        if (closestBlock.next().isValid()) {
            return closestBlock.next();
        } else {
            return closestBlock;
        }
    }
}

void FictionTextEdit::updateFocusBlock() {
    qDebug() << "updateFocusBlock";
    int centerY = getVisibleCenterY();

    // Check if the previously centered block is still close to the center
    if (previousCenteredBlock.isValid()) {
        qDebug() << "++++++++++++++++++++++ 11111";
        int position = checkVisibleCenterBlock(previousCenteredBlock);
        if (position == 0) {

            return; // The previous centered block is still close enough, no need to update
        }
    }

    // If there was a previously centered block, set it to grey
    if (previousCenteredBlock.isValid()) {
        qDebug() << "++++++++++++++++++++++ 22222";
        applyBlockFormatting(previousCenteredBlock);
    }

    // check the prevous & next blocks
    int positionPrevious = checkVisibleCenterBlock(previousCenteredBlock.previous());
    if (positionPrevious == 0) {
        qDebug() << "++++++++++++++++++++++ 33333";
        newCenteredBlock = previousCenteredBlock.previous();
        applyBlockFormatting(newCenteredBlock);
        previousCenteredBlock = newCenteredBlock;
        return;
    }
    int positionNext = checkVisibleCenterBlock(previousCenteredBlock.next());
    if (positionNext == 0) {
        qDebug() << "++++++++++++++++++++++ 44444";
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
    // cursorTimer->start(); // Start cursor timer
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