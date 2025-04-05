/*
FictionViewTab

mot
*/

#include "fictionviewtab.h"


FictionViewTab::FictionViewTab(const QString &content, const QString &filePath, QWidget *parent, bool isPrisoner, ProjectManager *projectManager)
    : BaseTextEditTab(content, filePath, parent), 
      vScrollBar(new QScrollBar(Qt::Vertical, this)),
      oldTextContent(""),
      isPrisoner(isPrisoner),
      projectManager(projectManager)
{
    // Remove currentFilePath initialization as it's handled by BaseTextEditTab
    globalLayout = new QHBoxLayout(this);
    leftLayout = new QVBoxLayout();
    topLeftLayout = new QHBoxLayout();
    bottomLeftLayout = new QHBoxLayout();

    // Add wordcount label
    wordCountLabel = new QLabel(this);
    wordCountLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    wordCountLabel->setStyleSheet("QLabel { color: #656565; background-color: #1F2020; border-radius: 4px; }");
    QFont font = wordCountLabel->font();  // Get the current font of the QLabel
    // font.setBold(true);                   // Set the font to bold
    // Apply the font to the QLabel
    wordCountLabel->setFont(font);
    wordCountLabel->setContentsMargins(8, 5, 4, 8);
    wordCountLabel->setVisible(false);  // Hide the label by default
    
    // Create a holder widget for the word count label
    QWidget *wordCountHolder = new QWidget(this);
    QHBoxLayout *wordCountLayout = new QHBoxLayout(wordCountHolder);
    wordCountLayout->setContentsMargins(0, 0, 0, 0);
    wordCountLayout->setSpacing(0);
    
    // Add spacer to push the label to the right
    QSpacerItem *wordCountSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    wordCountLayout->addItem(wordCountSpacer);
    wordCountLayout->addWidget(wordCountLabel);
    
    // Set the holder widget's policy to expand
    wordCountHolder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    
    globalLayout->setContentsMargins(0, 0, 0, 0);
    globalLayout->setSpacing(0);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    topLeftLayout->setContentsMargins(0, 0, 0, 0);
    topLeftLayout->setSpacing(16);
    bottomLeftLayout->setContentsMargins(0, 0, 0, 0);
    bottomLeftLayout->setSpacing(0);

    QSpacerItem *topLeftSpacerLeft1 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);
    QSpacerItem *topLeftSpacerLeft2 = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *topLeftSpacerRight = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

    SearchWidget *searchWidget = new SearchWidget();

    // add prisoner button
    prisonerButton = new HoverButton("Prisoner", QIcon(":/icons/prisoner.png"), this);
    prisonerButton->setLayoutDirection(Qt::RightToLeft);

    // sniper button
    sniperButton = new HoverButton("Sniper", QIcon(":/icons/sniper.png"), this);
    sniperButton->setLayoutDirection(Qt::RightToLeft);
    if (isPrisoner) {
        textEdit = new PrisonerFictionTextEdit(this, projectManager);

        sniperButton->setVisible(false);
    } else {
        textEdit = new FictionTextEdit(this, projectManager);

        sniperButton->setVisible(true);
    }
    
    topLeftLayout->addItem(topLeftSpacerLeft1);
    topLeftLayout->addItem(topLeftSpacerLeft2);
    topLeftLayout->addWidget(prisonerButton);
    topLeftLayout->addWidget(sniperButton);
    topLeftLayout->addWidget(searchWidget);
    topLeftLayout->addItem(topLeftSpacerRight);

    QWidget *topLeftWidget = new QWidget(this);
    topLeftWidget->setLayout(topLeftLayout);

    QMargins margins = topLeftWidget->contentsMargins();
    margins.setTop(8); // Set the desired top margin
    topLeftWidget->setContentsMargins(margins);

    setupTextEdit(content);
    setupScrollBar();
    syncScrollBar();

    // QSpacerItem *bottomLeftSpacer = new QSpacerItem(64, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);
    // bottomLeftSpacer->setMaxWidth(160);
    QWidget *spacerWidgetLeft = new QWidget();
    spacerWidgetLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // spacerWidgetLeft->setMaximumWidth(320); // Limit the max width of the spacer widget
    // spacerWidgetLeft->setMinimumWidth(10);

    // add stupid fucking word count
    QLayout *spaceAndCounterLayout = new QVBoxLayout();
    QWidget *spaceAndCounterWidget = new QWidget(this);
    spaceAndCounterWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // spaceAndCounterWidget->setMaximumWidth(320);
    // spaceAndCounterWidget->setMinimumWidth(10);

    spaceAndCounterLayout->setSpacing(0);
    spaceAndCounterWidget->setContentsMargins(0, 0, 0, 0);

    // QSpacerItem *bottomRightSpacer = new QSpacerItem(64, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);
    // bottomRightSpacer->setMaxWidth(150);
    // spaceAndCounterLayout->addItem(bottomRightSpacer);
    QWidget *spacerWidgetRight = new QWidget();
    spacerWidgetRight->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);
    // spacerWidgetRight->setMaximumWidth(320); // Limit the max width of the spacer widget
    // spacerWidgetRight->setMinimumWidth(10);

    spaceAndCounterLayout->addWidget(spacerWidgetRight);
    QSpacerItem *verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    spaceAndCounterLayout->addItem(verticalSpacer);
    spaceAndCounterLayout->addWidget(wordCountHolder);
    spaceAndCounterWidget->setLayout(spaceAndCounterLayout);


    QWidget *textEditWidget = new QWidget();
    QVBoxLayout *textEditLayout = new QVBoxLayout;
    textEditLayout->addWidget(textEdit);

    // Set the layout's margins to zero
    textEditLayout->setContentsMargins(0, 0, 0, 0);
    textEditWidget->setLayout(textEditLayout);

    bottomLeftLayout->addWidget(spacerWidgetLeft);
    bottomLeftLayout->addWidget(textEdit);
    bottomLeftLayout->addWidget(spaceAndCounterWidget);


    leftLayout->addWidget(topLeftWidget);
    leftLayout->addLayout(bottomLeftLayout);

    globalLayout->addLayout(leftLayout);
    globalLayout->addWidget(vScrollBar);

    setLayout(globalLayout);
    connect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::activateSniperMode);
    connect(textEdit, &FictionTextEdit::textChanged, this, &FictionViewTab::editContent);
    connect(textEdit, &FictionTextEdit::onFictionEditSearch, searchWidget, &SearchWidget::handleSearch);
    connect(textEdit, &FictionTextEdit::focusGained, searchWidget, &SearchWidget::loseAttention);
    connect(searchWidget, &SearchWidget::onSearch, textEdit, &FictionTextEdit::search);
    connect(searchWidget, &SearchWidget::onClear, textEdit, &FictionTextEdit::clearSearch);
    connect(searchWidget, &SearchWidget::onSearchPrev, textEdit, &FictionTextEdit::searchPrev);
    connect(textEdit, &FictionTextEdit::textChanged, this, &FictionViewTab::updateWordcount);
    connect(textEdit, &FictionTextEdit::showWikiAt, this, &FictionViewTab::showWikiFunc);
    connect(textEdit, &FictionTextEdit::hideWiki, this, &FictionViewTab::hideWikiFunc);

    if (isPrisoner) {
        // // Connect signals unique to PrisonerTextEdit
        // connect(static_cast<PrisonerFictionTextEdit*>(textEdit), &PrisonerFictionTextEdit::onEscape,
        //         this, &FictionViewTab::deactivatePrisonerMode);
        // connect(prisonerButton, &QPushButton::clicked, this, &FictionViewTab::deactivatePrisonerMode);
    } else {
        connect(prisonerButton, &QPushButton::clicked, this, &FictionViewTab::activatePrisonerMode); 
    }
}

void FictionViewTab::setupTextEdit(const QString &content) {
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->load(content);
    textEdit->setStyleSheet(
        "QTextEdit {"
        "   background-color: transparent;"
        "   border: none;"
        "}"
        );
    
    // Set initial minimum width, but we'll adjust this dynamically
    textEdit->setMinimumWidth(360); // minimum width
    textEdit->setMaximumWidth(960); // maximum width
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QTextDocument *doc = textEdit->document();
    QTextFrame *rootFrame = doc->rootFrame();
    QTextFrameFormat format = rootFrame->frameFormat();
    format.setTopMargin(128);
    format.setBottomMargin(256);
    format.setLeftMargin(16);
    format.setRightMargin(16);
    rootFrame->setFrameFormat(format);
}

void FictionViewTab::setupScrollBar() {
    vScrollBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    vScrollBar->setStyleSheet(
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: transparent;"
        "    width: 8px;"
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #262626;"
        "    min-height: 16px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #1f2020;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {"
        "    height: 0px;"
        "    width: 0px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}"
        );

    connect(vScrollBar, &QScrollBar::valueChanged, textEdit->verticalScrollBar(), &QScrollBar::setValue);
    connect(textEdit->verticalScrollBar(), &QScrollBar::valueChanged, vScrollBar, &QScrollBar::setValue);
    connect(textEdit->verticalScrollBar(), &QScrollBar::rangeChanged, this, &FictionViewTab::syncScrollBar);
}

void FictionViewTab::syncScrollBar() {
    QScrollBar* internalScrollBar = textEdit->verticalScrollBar();
    vScrollBar->setRange(internalScrollBar->minimum(), internalScrollBar->maximum());
    vScrollBar->setPageStep(internalScrollBar->pageStep());
    vScrollBar->setValue(internalScrollBar->value());
    vScrollBar->setVisible(internalScrollBar->minimum() != internalScrollBar->maximum());
}

void FictionViewTab::activateSniperMode() {
    textEdit->activateSniperMode();
    disconnect(textEdit, &QTextEdit::textChanged, this, &FictionViewTab::updateWordcount);
    qDebug() << "activateHighlightMode";
    disconnect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::activateSniperMode);
    connect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::deactivateSniperMode);
}

void FictionViewTab::deactivateSniperMode() {
    textEdit->deactivateSniperMode();
    qDebug() << "DeactivateHighlightMode";
    disconnect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::deactivateSniperMode);
    connect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::activateSniperMode);
}

bool FictionViewTab::saveContent() {
    if (currentFilePath.isEmpty()) {
        // If no file path is provided, prompt the user to select a save location
        QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");
        if (fileName.isEmpty()) {
            // If the user cancels the save dialog, do nothing
            return false;
        }
        currentFilePath = fileName;
        
        QFile file(currentFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Save Error", "Unable to open file for writing.");
            return false;
        }

        QTextStream out(&file);
        out << textEdit->toPlainText();
        file.close();

        emit onChangeFileType(fileName);

        return true;
    }

    QFile file(currentFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Save Error", "Unable to open file for writing.");
        return false;
    }

    QTextStream out(&file);
    out << textEdit->toPlainText();
    file.close();

    emit onChangeTabName(QFileInfo(currentFilePath).fileName());
    return true;
}

void FictionViewTab::editContent() {
    emit onChangeTabName(QFileInfo(currentFilePath).fileName() + "*");
}

void FictionViewTab::updateWordcount() {
    // if word count label is not visible, make it visible
    if (!wordCountLabel->isVisible()) {
        wordCountLabel->setVisible(true);
    }
    // prevent lagging in snipper mode
    QString newTextContent = textEdit->toPlainText();
    if (newTextContent == oldTextContent) {
        return;
    }

    QString text = textEdit->toPlainText();
    if (text.isEmpty()) {
        wordCountLabel->setText(QString::number(0) + " words  ");
        return;
    }

    qDebug() << "word count height" << wordCountLabel->height();
    int wordCount = 0;

    // Regular expression for alphabetic languages (English, etc.)
    QRegularExpression alphabeticRegex("\\b\\w+\\b");

    // Regular expression for CJK (Chinese, Japanese, Korean) characters
    QRegularExpression cjkRegex("[\\p{Han}\\p{Hiragana}\\p{Katakana}]");

    QRegularExpressionMatchIterator i = alphabeticRegex.globalMatch(text);
    while (i.hasNext()) {
        i.next();
        wordCount++;
    }

    i = cjkRegex.globalMatch(text);
    while (i.hasNext()) {
        i.next();
        wordCount++;
    }
    wordCountLabel->setText(QString::number(wordCount) + " words  ");
}

void FictionViewTab::activatePrisonerMode() {
    // create a dialog for setting goal and time limit
    // if user click cancel, do nothing
    // if user click ok, save the content and activate prisoner mode

    // Save content before showing dialog
    if (!saveContent()) {
        return;  // If save failed or was cancelled, don't proceed
    }

    // // Create and show the prisoner dialog
    PrisonerDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;  // User cancelled the dialog
    }
    

    // new FictionViewTab
    qDebug() << "activate prisoner mode";
    QString content = textEdit->toPlainText();
    QString filePath = getCurrentFilePath();
    QWidget *parentWidget = nullptr;

    // TODO: need some extra check;
    // 
    FictionViewTab *fullScreenFictionViewTab = new FictionViewTab(textEdit->toPlainText(), filePath, parentWidget, true, projectManager);

    // connect(prisonerButton, &QPushButton::clicked, this, &FictionViewTab::deactivatePrisonerMode);

    // Create a QDialog to host the FictionViewTab in full screen
    QDialog *fullScreenDialog = new QDialog();
    fullScreenDialog->setStyleSheet("background-color: #31363F;");
    fullScreenDialog->setWindowFlag(Qt::Window);                  // Set as a window
    fullScreenDialog->setWindowFlag(Qt::FramelessWindowHint);     // Remove borders and title bar
    fullScreenDialog->setWindowModality(Qt::ApplicationModal);    // Block interaction with other windows
    fullScreenDialog->setWindowState(Qt::WindowFullScreen);       // Set full screen
    fullScreenDialog->setAttribute(Qt::WA_DeleteOnClose);         // Automatically delete when closed

    // Set FictionViewTab as the content of the dialog
    QVBoxLayout *layout = new QVBoxLayout(fullScreenDialog);
    layout->addWidget(fullScreenFictionViewTab);
    fullScreenDialog->setLayout(layout);

    // Store the dialog pointer in a member variable for future reference
    prisonerDialog = fullScreenDialog;
    prisonerFictionViewTab = fullScreenFictionViewTab; // New member to track the FictionViewTab
    fullScreenPrisonerTextEdit = static_cast<PrisonerFictionTextEdit*>(prisonerFictionViewTab->textEdit);
    QPushButton *fullScreenPrisonerButton = prisonerFictionViewTab->prisonerButton;

    connect(fullScreenPrisonerTextEdit, &PrisonerFictionTextEdit::onEscape, this, &FictionViewTab::deactivatePrisonerMode);
    connect(fullScreenPrisonerButton, &QPushButton::clicked, this, &FictionViewTab::deactivatePrisonerMode);

    // Show the dialog in full-screen mode
    fullScreenDialog->show();
}

void FictionViewTab::deactivatePrisonerMode() {
    // Close the prisoner dialog if it exists
    if (prisonerDialog) {
        // Get the text from the FictionViewTab before closing
        if (prisonerFictionViewTab) {
            QString prisonerText = prisonerFictionViewTab->textEdit->toPlainText();
            textEdit->load(prisonerText);
        }

        prisonerDialog->close();
        prisonerDialog = nullptr;  // Reset the pointer
        prisonerFictionViewTab = nullptr; // Reset the pointer
    }
}

QString FictionViewTab::getTextContent() const {
    return textEdit->toPlainText();
}

void FictionViewTab::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    
    // Calculate a proportional minimum width based on the tab's width
    // For example, make it 40% of the tab width, but not less than 360px
    int newMinWidth = qMin(960, qMax(360, static_cast<int>(width() * 0.72)));
    
    // Update the textEdit's minimum width
    textEdit->setMinimumWidth(newMinWidth);
}

void FictionViewTab::showWikiFunc(const QString &wikiContent, QPoint lastMousePos) {
    emit showWikiAt(wikiContent, lastMousePos);
}

void FictionViewTab::hideWikiFunc() {
    emit hideWiki();
}