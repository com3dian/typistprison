#include "markdownviewtab.h"
#include <QTextBlock>


MarkdownViewTab::MarkdownViewTab(const QString &content, const QString &filePath, QWidget *parent)
    : QWidget(parent),
      textEdit(new QMarkdownTextEdit(this)), 
      vScrollBar(new QScrollBar(Qt::Vertical, this)),
      currentFilePath(filePath)
{
    globalLayout = new QHBoxLayout(this);
    leftLayout = new QVBoxLayout();
    topLeftLayout = new QHBoxLayout();
    bottomLeftLayout = new QHBoxLayout();

    globalLayout->setContentsMargins(0, 0, 0, 0);
    globalLayout->setSpacing(0);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    topLeftLayout->setContentsMargins(0, 0, 0, 0);
    topLeftLayout->setSpacing(16);
    bottomLeftLayout->setContentsMargins(16, 16, 16, 16);
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

    topLeftLayout->addItem(topLeftSpacerLeft1);
    topLeftLayout->addItem(topLeftSpacerLeft2);
    topLeftLayout->addWidget(searchWidget);
    topLeftLayout->addWidget(button1);
    topLeftLayout->addItem(topLeftSpacerRight);

    QWidget *topLeftWidget = new QWidget(this);
    topLeftWidget->setLayout(topLeftLayout);

    QMargins margins = topLeftWidget->contentsMargins();
    margins.setTop(8); // Set the desired top margin
    topLeftWidget->setContentsMargins(margins);

    setupTextEdit(content);
    setupScrollBar();
    syncScrollBar();

    bottomLeftLayout->addWidget(textEdit);

    leftLayout->addWidget(topLeftWidget);
    leftLayout->addLayout(bottomLeftLayout);

    globalLayout->addLayout(leftLayout);
    globalLayout->addWidget(vScrollBar);

    setLayout(globalLayout);
    connect(textEdit, &QMarkdownTextEdit::onMarkdownSearch, searchWidget, &SearchWidget::handleSearch);
    connect(textEdit, &QMarkdownTextEdit::focusGained, searchWidget, &SearchWidget::loseAttention);
    connect(textEdit, &QMarkdownTextEdit::onSave, this, &MarkdownViewTab::saveContent);
    connect(textEdit, &QMarkdownTextEdit::textChanged, this, &MarkdownViewTab::editContent);
    connect(searchWidget, &SearchWidget::onSearch, textEdit, &QMarkdownTextEdit::search);
    connect(searchWidget, &SearchWidget::onClear, textEdit, &QMarkdownTextEdit::clearSearch);
    connect(searchWidget, &SearchWidget::onSearchPrev, textEdit, &QMarkdownTextEdit::searchPrev);
}

void MarkdownViewTab::setupTextEdit(const QString &content) {
    textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit->load(content);
    textEdit->setStyleSheet(
        "QMarkdownTextEdit {"
        "   background-color: #31363F;"
        "   border: none;"
        "}"
    );
    textEdit->setMinimumWidth(400); // Adjust the minimum width as needed
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QTextDocument *doc = textEdit->document();
    QTextFrame *rootFrame = doc->rootFrame();
    QTextFrameFormat format = rootFrame->frameFormat();
    format.setTopMargin(16);
    format.setBottomMargin(16);
    format.setLeftMargin(16);
    format.setRightMargin(16);
    rootFrame->setFrameFormat(format);
}

void MarkdownViewTab::setupScrollBar() {
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
    connect(textEdit->verticalScrollBar(), &QScrollBar::rangeChanged, this, &MarkdownViewTab::syncScrollBar);
}

void MarkdownViewTab::syncScrollBar() {
    QScrollBar* internalScrollBar = textEdit->verticalScrollBar();
    vScrollBar->setRange(internalScrollBar->minimum(), internalScrollBar->maximum());
    vScrollBar->setPageStep(internalScrollBar->pageStep());
    vScrollBar->setValue(internalScrollBar->value());
    vScrollBar->setVisible(internalScrollBar->minimum() != internalScrollBar->maximum());
}

QString MarkdownViewTab::getCurrentFilePath() const {
    return currentFilePath;
}

bool MarkdownViewTab::saveContent() {
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

void MarkdownViewTab::editContent() {
    emit onChangeTabName(QFileInfo(currentFilePath).fileName() + "*");
}