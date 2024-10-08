#include "fictionviewtab.h"


FictionViewTab::FictionViewTab(const QString &content, const QString &filePath, QWidget *parent)
    : QWidget(parent), 
      textEdit(new FictionTextEdit(this)), 
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

    QSpacerItem *topLeftSpacerLeft1 = new QSpacerItem(64, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);
    QSpacerItem *topLeftSpacerLeft2 = new QSpacerItem(64, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem *topLeftSpacerRight = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    SearchWidget *searchWidget = new SearchWidget();

    QPushButton *button1 = new QPushButton();
    QPixmap normalPixmap("/home/com3dian/Downloads/background.png");
    button1->setIcon(QIcon(normalPixmap));
    button1->setStyleSheet(
        "QPushButton {"
        "   background-color: transparent;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   border-image: url(/home/com3dian/Downloads/noun-remove-1075149.svg);"
        "}"
        );
    button1->setIconSize(QSize(32, 32));

    // QPushButton *button1 = new QPushButton("Button 1", this);
    button2 = new QPushButton("Button 2", this);

    topLeftLayout->addItem(topLeftSpacerLeft1);
    topLeftLayout->addItem(topLeftSpacerLeft2);
    topLeftLayout->addWidget(searchWidget);
    topLeftLayout->addWidget(button1);
    topLeftLayout->addWidget(button2);
    topLeftLayout->addItem(topLeftSpacerRight);

    QWidget *topLeftWidget = new QWidget(this);
    topLeftWidget->setLayout(topLeftLayout);

    QMargins margins = topLeftWidget->contentsMargins();
    margins.setTop(8); // Set the desired top margin
    topLeftWidget->setContentsMargins(margins);

    setupTextEdit(content);
    setupScrollBar();
    syncScrollBar();

    QSpacerItem *bottomLeftSpacer = new QSpacerItem(64, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);

    // add stupid fucking word count
    QLayout *spaceAndCounterLayout = new QVBoxLayout();
    QWidget *spaceAndCounterWidget = new QWidget(this);

    spaceAndCounterLayout->setSpacing(0);
    spaceAndCounterWidget->setContentsMargins(0, 0, 0, 0);

    QSpacerItem *bottomRightSpacer = new QSpacerItem(64, 20, QSizePolicy::Expanding, QSizePolicy::Maximum);
    spaceAndCounterLayout->addItem(bottomRightSpacer);
    // QSpacerItem *verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    // spaceAndCounterLayout->addItem(verticalSpacer);
    spaceAndCounterLayout->addWidget(wordCountLabel);
    spaceAndCounterWidget->setLayout(spaceAndCounterLayout);

    bottomLeftLayout->addItem(bottomLeftSpacer);
    bottomLeftLayout->addWidget(textEdit);
    bottomLeftLayout->addWidget(spaceAndCounterWidget);

    leftLayout->addWidget(topLeftWidget);
    leftLayout->addLayout(bottomLeftLayout);

    globalLayout->addLayout(leftLayout);
    globalLayout->addWidget(vScrollBar);

    setLayout(globalLayout);
    connect(button2, &QPushButton::clicked, this, &FictionViewTab::activateSniperMode);
    connect(textEdit, &FictionTextEdit::onSave, this, &FictionViewTab::saveContent);
    connect(textEdit, &FictionTextEdit::textChanged, this, &FictionViewTab::editContent);
    connect(textEdit, &FictionTextEdit::onFictionEditSearch, searchWidget, &SearchWidget::handleSearch);
    connect(textEdit, &FictionTextEdit::focusGained, searchWidget, &SearchWidget::loseAttention);
    connect(searchWidget, &SearchWidget::onSearch, textEdit, &FictionTextEdit::search);
    connect(searchWidget, &SearchWidget::onClear, textEdit, &FictionTextEdit::clearSearch);
    connect(searchWidget, &SearchWidget::onSearchPrev, textEdit, &FictionTextEdit::searchPrev);
    connect(textEdit, &FictionTextEdit::textChanged, this, &FictionViewTab::updateWordcount);
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
    textEdit->setMinimumWidth(800); // Adjust the minimum width as needed
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
    disconnect(button2, &QPushButton::clicked, this, &FictionViewTab::activateSniperMode);
    connect(button2, &QPushButton::clicked, this, &FictionViewTab::deactivateSniperMode);
}

void FictionViewTab::deactivateSniperMode() {
    textEdit->deactivateSniperMode();
    qDebug() << "DeactivateHighlightMode";
    disconnect(button2, &QPushButton::clicked, this, &FictionViewTab::deactivateSniperMode);
    connect(button2, &QPushButton::clicked, this, &FictionViewTab::activateSniperMode);
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