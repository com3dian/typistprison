/*
FictionViewTab

mot
*/

#include "fictionviewtab.h"


FictionViewTab::FictionViewTab(const QString &content,
                               const QString &filePath,
                               QWidget *parent,
                               bool isPrisoner,
                               ProjectManager *projectManager)
    : BaseTextEditTab(content, filePath, parent), 
      vScrollBar(new QScrollBar(Qt::Vertical, this)),
      wordCountSpacerRight(nullptr),
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
    wordCountLabel->setStyleSheet("QLabel { color: #BDBDBD; background-color: #1F2020; border-radius: 4px; }");
    QFont font = wordCountLabel->font();  // Get the current font of the QLabel
    // font.setBold(true);                   // Set the font to bold
    // Apply the font to the QLabel
    wordCountLabel->setFont(font);
    wordCountLabel->setContentsMargins(8, 5, 8, 8);
    wordCountLabel->setVisible(false);  // Hide the label by default
    
    // Create a holder widget for the word count label
    QWidget *wordCountHolder = new QWidget(this);
    QHBoxLayout *wordCountLayout = new QHBoxLayout(wordCountHolder);
    wordCountLayout->setContentsMargins(0, 0, 0, 8);
    wordCountLayout->setSpacing(0);
    
    // Add spacer to push the label to the right
    QSpacerItem *wordCountSpacerLeft = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    wordCountLayout->addItem(wordCountSpacerLeft);
    wordCountLayout->addWidget(wordCountLabel);
    
    // Replace spacer item with widget
    wordCountSpacerRight = new QWidget(this);
    wordCountSpacerRight->setFixedWidth(8);
    wordCountSpacerRight->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    wordCountSpacerRight->setStyleSheet("background: transparent; background-color: transparent");
    wordCountLayout->addWidget(wordCountSpacerRight);
    
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
    prisonerButton = new HoverButton("Prisoner", this);
    prisonerButton->setIcons(QIcon(":/icons/emptyicon.png"), QIcon(":/icons/prisoner.png"));
    prisonerButton->setLayoutDirection(Qt::RightToLeft);

    // sniper button
    sniperButton = new HoverButton("Sniper", this);
    sniperButton->setIcons(QIcon(":/icons/emptyicon.png"), QIcon(":/icons/sniper.png"));
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

    // add stupid fucking word count
    QLayout *spaceAndCounterLayout = new QVBoxLayout();
    QWidget *spaceAndCounterWidget = new QWidget(this);
    spaceAndCounterWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    spaceAndCounterLayout->setSpacing(0);
    spaceAndCounterLayout->setContentsMargins(0, 0, 0, 0);
    spaceAndCounterWidget->setContentsMargins(0, 0, 0, 0);
    spaceAndCounterWidget->setStyleSheet("background-color: transparent; border-radius: 6px;" );

    QWidget *spacerWidgetRight = new QWidget();
    spacerWidgetRight->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Expanding);

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
    connect(prisonerButton, &QPushButton::clicked, this, &FictionViewTab::activatePrisonerMode);
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
        "    min-height: 32px;"
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
    bool isScrollBarVisible = internalScrollBar->minimum() != internalScrollBar->maximum();
    vScrollBar->setVisible(isScrollBarVisible);
    wordCountSpacerRight->setVisible(!isScrollBarVisible);
}

void FictionViewTab::activateSniperMode() {
    textEdit->activateSniperMode();
    disconnect(textEdit, &QTextEdit::textChanged, this, &FictionViewTab::updateWordcount);
    this->updateWordcount(); // update word count immediately after switching mode
    if(wordCountLabel->isVisible()) {
        wordCountLabel->setVisible(false);
    }
    disconnect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::activateSniperMode);
    connect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::deactivateSniperMode);
}

void FictionViewTab::deactivateSniperMode() {
    textEdit->deactivateSniperMode();
    connect(textEdit, &QTextEdit::textChanged, this, &FictionViewTab::updateWordcount);
    if(!wordCountLabel->isVisible()) {
        wordCountLabel->setVisible(true);
    }
    disconnect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::deactivateSniperMode);
    connect(sniperButton, &QPushButton::clicked, this, &FictionViewTab::activateSniperMode);
}

/*
Save the content into `currentFilePath` file.

if `currentFilePath` is empty, popup a file-saving dialog; and then save.
*/
bool FictionViewTab::saveContent() {
    if (isPrisoner) { // if in prisoner mode, do nothing
        return false;
    }

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
    wordCountLabel->setText(QString::number(wordCount) + " words");

    if (isPrisoner) {
        // update typist progress
        

    }
}

void FictionViewTab::activatePrisonerMode() {
    // create a dialog for setting goal and time limit
    // if user click cancel, do nothing
    // if user click ok, save the content and activate prisoner mode

    // Save content before showing dialog
    if (!saveContent()) {
        return;  // If save failed or was cancelled, don't proceed
    }

    int dialogWordGoal;
    int dialogTimeLimit;

    // Create and show the prisoner dialog
    PrisonerDialog dialog(this);
    connect(&dialog, &PrisonerDialog::prisonerSettings,
            this, [&](int wordGoal, int timeLimit) {
        // Handle the prisoner settings here
        qDebug() << "Word Goal:" << wordGoal << "Time Limit:" << timeLimit;

        dialogWordGoal = wordGoal;
        dialogTimeLimit = timeLimit;
        // Store these values or use them as needed
    });

    if (dialog.exec() != QDialog::Accepted) {
        return;  // User cancelled the dialog
    }

    emit activatePrisonerModeSignal(dialogTimeLimit, dialogWordGoal);

    disconnect(prisonerButton, &QPushButton::clicked, this, &FictionViewTab::activatePrisonerMode);
    connect(prisonerButton, &QPushButton::clicked, this, &FictionViewTab::deactivatePrisonerMode);
}

void FictionViewTab::deactivatePrisonerMode() {
    emit deactivatePrisonerModeSignal();

    connect(prisonerButton, &QPushButton::clicked, this, &FictionViewTab::activatePrisonerMode);
    disconnect(prisonerButton, &QPushButton::clicked, this, &FictionViewTab::deactivatePrisonerMode);
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
    if (isPrisoner) {
        qDebug() << "showWikiFunc: prisoner mode";
    }
    emit showWikiAt(wikiContent, lastMousePos);
}

void FictionViewTab::hideWikiFunc() {
    emit hideWiki();
}