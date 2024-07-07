#include "searchWidget.h"
#include <QPalette>
#include <QKeyEvent>

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent), isHovered(false), isOnSearch(false)
{
    lineEdit = new QLineEdit(this);
    lineEdit->setMinimumWidth(400);
    lineEdit->setStyleSheet("background-color: transparent; border: none;");

    button = new QPushButton("Search", this);
    bottomLine = new QWidget(this);
    bottomLine->setFixedHeight(1);
    bottomLine->setStyleSheet("background-color: transparent;");

    connect(lineEdit, &QLineEdit::returnPressed, this, [this]() { handleSearch(lineEdit->text()); });
    connect(button, &QPushButton::clicked, this, [this]() { handleSearch(lineEdit->text()); });

    hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    hLayout->addWidget(lineEdit);
    hLayout->addWidget(button);

    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    vLayout->addLayout(hLayout);
    vLayout->addWidget(bottomLine);

    setLayout(vLayout);

    // Install event filter to detect hover events
    installEventFilter(this);
    lineEdit->installEventFilter(this);

    connect(lineEdit, &QLineEdit::editingFinished, this, &SearchWidget::updateBottomLine);
}

void SearchWidget::handleSearch(const QString &text)
{
    if (!text.isEmpty()) {
        emit onSearch(text);
        lineEdit->setText(text);

        QPalette palette = lineEdit->palette();
        QColor customColor("#F17C67");
        palette.setColor(QPalette::Highlight, customColor);
        palette.setColor(QPalette::HighlightedText, lineEdit->palette().color(QPalette::Text));
        lineEdit->setPalette(palette);
        lineEdit->selectAll();

        button->setText("Clear");
        disconnect(button, &QPushButton::clicked, this, nullptr);
        connect(button, &QPushButton::clicked, this, &SearchWidget::handleClear);
        connect(lineEdit, &QLineEdit::textChanged, this, &SearchWidget::handleReSearch);

        isOnSearch = true;
    } else {
        lineEdit->setFocus();
    }
    updateBottomLine();
}

void SearchWidget::handleClear()
{
    lineEdit->clear();
    emit onClear();
    button->setText("Search");
    disconnect(button, &QPushButton::clicked, this, nullptr);
    connect(button, &QPushButton::clicked, this, [this]() { handleSearch(lineEdit->text()); });
    isOnSearch = false;
    updateBottomLine();
}

void SearchWidget::handleReSearch()
{
    button->setText("Search");
    disconnect(button, &QPushButton::clicked, this, nullptr);
    connect(button, &QPushButton::clicked, this, [this]() { handleSearch(lineEdit->text()); });
    isOnSearch = true;
    updateBottomLine();
}

bool SearchWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == lineEdit) {
        if (event->type() == QEvent::FocusOut) {
            updateBottomLine();
        }
    }
    if (event->type() == QEvent::Enter) {
        isHovered = true;
        updateBottomLine();
    } else if (event->type() == QEvent::Leave) {
        isHovered = false;
        updateBottomLine();
    }
    return QWidget::eventFilter(obj, event);
}

void SearchWidget::updateBottomLine()
{
    if (isOnSearch) {
        bottomLine->setStyleSheet("background-color: #FFA896;");
    }
    else if (isHovered || !lineEdit->text().isEmpty() || lineEdit->hasFocus()) {
        bottomLine->setStyleSheet("background-color: #454F61;");
    } else {
        bottomLine->setStyleSheet("background-color: transparent;");
    }
}
