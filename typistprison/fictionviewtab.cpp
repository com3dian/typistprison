#include "fictionviewtab.h"


FictionViewTab::FictionViewTab(const QString &content, const QString &filePath, QWidget *parent, bool isPrisoner, ProjectManager *projectManager)
    : QWidget(parent), 
      vScrollBar(new QScrollBar(Qt::Vertical, this)), 
      currentFilePath(filePath),
      oldTextContent("")
{
    globalLayout = new QHBoxLayout(this);
    leftLayout = new QVBoxLayout();
    topLeftLayout = new QHBoxLayout();
    bottomLeftLayout = new QHBoxLayout();

    // Add wordcount label
    wordCountLabel = new QLabel(this);
    wordCountLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);
    wordCountLabel->setStyleSheet("QLabel { color : #454F61; }"); // #989A9C
    QFont font = wordCountLabel->font();  // Get the current font of the QLabel
    font.setBold(true);                   // Set the font to bold
    // Apply the font to the QLabel
    wordCountLabel->setFont(font);
    wordCountLabel->setContentsMargins(0, 0, 0, 1);

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
    QSpacerItem *topLeftSpacerRight = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    SearchWidget *searchWidget = new SearchWidget();

    // QPushButton *button1 = new QPushButton();
    // QPixmap normalPixmap("/home/com3dian/Downloads/background.png");
    // button1->setIcon(QIcon(normalPixmap));
    // button1->setStyleSheet(
    //     "QPushButton {"
    //     "   background-color: transparent;"
    //     "   border: none;"
    //     "}"
    //     "QPushButton:hover {"
    //     "   border-image: url(/home/com3dian/Downloads/noun-remove-1075149.svg);"
    //     "}"
    //     );
    // 

    // prisoner button
    prisonerButton = new QPushButton(this);
    prisonerButton->setStyleSheet(
            "QPushButton {"
            "border: none;"
            "border-image: url(:/icons/newfile_silent.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:hover {"
            "border-image: url(:/icons/newfile_hover.png) 0 0 0 0 stretch stretch;"
            "}"
            "QPushButton:pressed {"
            "border-image: url(:/icons/newfile_clicked.png) 0 0 0 0 stretch stretch;"
            "}"
    );
    prisonerButton->setFixedSize(16, 16);

    if (projectManager) {
        qDebug() << "project manager in FictionViewTab";
    } else {
        qDebug() << "no project manager in FictionViewTab";
    }

    // sniper button
    if (isPrisoner) {
        textEdit = new PrisonerFictionTextEdit(this, projectManager);

        sniperButton = new QPushButton("No Sniper Button", this);
    } else {
        textEdit = new FictionTextEdit(this, projectManager);

        sniperButton = new QPushButton("Sniper Button", this);
    }
    
    topLeftLayout->addItem(topLeftSpacerLeft1);
    topLeftLayout->addItem(topLeftSpacerLeft2);
    topLeftLayout->addWidget(searchWidget);
    topLeftLayout->addWidget(prisonerButton);
    topLeftLayout->addWidget(sniperButton);
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
    spacerWidgetLeft->setMaximumWidth(320); // Limit the max width of the spacer widget
    // spacerWidgetLeft->setMinimumWidth(10);

    // add stupid fucking word count
    QLayout *spaceAndCounterLayout = new QVBoxLayout();
    QWidget *spaceAndCounterWidget = new QWidget(this);
    spaceAndCounterWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    spaceAndCounterWidget->setMaximumWidth(320);
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
    spaceAndCounterLayout->addWidget(wordCountLabel);
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
    connect(textEdit, &FictionTextEdit::onSave, this, &FictionViewTab::saveContent);
    connect(textEdit, &FictionTextEdit::textChanged, this, &FictionViewTab::editContent);
    connect(textEdit, &FictionTextEdit::onFictionEditSearch, searchWidget, &SearchWidget::handleSearch);
    connect(textEdit, &FictionTextEdit::focusGained, searchWidget, &SearchWidget::loseAttention);
    connect(searchWidget, &SearchWidget::onSearch, textEdit, &FictionTextEdit::search);
    connect(searchWidget, &SearchWidget::onClear, textEdit, &FictionTextEdit::clearSearch);
    connect(searchWidget, &SearchWidget::onSearchPrev, textEdit, &FictionTextEdit::searchPrev);
    connect(textEdit, &FictionTextEdit::textChanged, this, &FictionViewTab::updateWordcount);

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
        "   background-color: #31363F;"
        "   border: none;"
        "}"
        );
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
        "    background: #4A515E;"
        "    min-height: 16px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #989A9C;"
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

QString FictionViewTab::getCurrentFilePath() const {
    return currentFilePath;
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
    wordCountLabel->setText(QString::number(wordCount) + " words  ");
}

void FictionViewTab::activatePrisonerMode() {
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



















