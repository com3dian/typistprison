#include "searchWidget.h"
#include <QPalette>
#include <QKeyEvent>

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent), isHovered(false), isOnSearch(false), isSearchPrev(false), isSearchLatter(false)
{
    lineEdit = new QLineEdit(this);
    lineEdit->setMinimumWidth(256);
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
    // Install event filter to detect 
    lineEdit->installEventFilter(this);

    connect(lineEdit, &QLineEdit::editingFinished, this, &SearchWidget::updateBottomLine);
}

void SearchWidget::handleSearch(const QString &text)
{
    if (!text.isEmpty()) {
        emit onSearch(text);
        qDebug() << "onsearch 1";

        // avoid textchanged signal to be triggered when
        QString prevSearchText = lineEdit->text();

        if (prevSearchText != text) {
            disconnect(lineEdit, &QLineEdit::textChanged, this, &SearchWidget::handleReSearch);
            lineEdit->setText(text);
        }

        QPalette palette = lineEdit->palette();
        palette.setColor(QPalette::Highlight, QColor("#84e0a5"));
        palette.setColor(QPalette::HighlightedText, QColor("#31363F"));
        lineEdit->setPalette(palette);
        lineEdit->selectAll();

        button->setText("Clear");
        disconnect(button, &QPushButton::clicked, this, nullptr);
        connect(button, &QPushButton::clicked, this, &SearchWidget::handleClear);
        qDebug() << "SearchWidget::handleSearch!" ;
        connect(lineEdit, &QLineEdit::textChanged, this, &SearchWidget::handleReSearch); // previously not empty and text changed

        isOnSearch = true;
    }
    // Focus to "search" lineEdit
    lineEdit->setFocus();

    updateBottomLine();
}

void SearchWidget::handleClear()
{
    lineEdit->clear();
    qDebug() << "SearchWidget::handleClear()";
    emit onClear();
    button->setText("Search");
    disconnect(button, &QPushButton::clicked, this, nullptr);
    connect(button, &QPushButton::clicked, this, [this]() { handleSearch(lineEdit->text()); });
    disconnect(lineEdit, &QLineEdit::textChanged, this, nullptr);
    isOnSearch = false;
    updateBottomLine();
}

void SearchWidget::handleReSearch()
{
    button->setText("Search");
    qDebug() << "SearchWidget::handleReSearch()";
    emit onClear();
    disconnect(button, &QPushButton::clicked, this, nullptr);
    connect(button, &QPushButton::clicked, this, [this]() { handleSearch(lineEdit->text()); });
    disconnect(lineEdit, &QLineEdit::textChanged, this, nullptr);
    isOnSearch = false;
    updateBottomLine();
}

bool SearchWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == lineEdit) {
        if (event->type() == QEvent::FocusOut) {
            updateBottomLine();
            return true;  // Event handled
        }

        // Check if the event is a key event
        if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
            // Convert event to QKeyEvent safely
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent) {
                // Get search text
                QString searchText = lineEdit->text();

                if (keyEvent->key() == Qt::Key_Up && isOnSearch) {
                    // Event filter is intalled to both `this` and `lineEdit`
                    // To avoid triggering `onSearchPrev` signal twice
                    isSearchPrev = not isSearchPrev;
                    if (isSearchPrev) {
                        emit onSearchPrev(searchText);
                        qDebug() << "on search up";
                    }
                    return true;  // Event handled

                } else if (keyEvent->key() == Qt::Key_Down && isOnSearch) {
                    isSearchLatter = not isSearchLatter;
                    if (isSearchLatter) {
                        emit onSearch(searchText);
                        qDebug() << "on search down";
                    }
                    return true;  // Event handled
                }
            }
        }
    } else if (obj == this) {
        if (event->type() == QEvent::Enter) {
            isHovered = true;
            updateBottomLine();
            return true;  // Event handled
        } else if (event->type() == QEvent::Leave) {
            isHovered = false;
            updateBottomLine();
            return true;  // Event handled
        }
    }

    // Let the base class process the event if not handled
    return QWidget::eventFilter(obj, event);
}

void SearchWidget::updateBottomLine()
{
    if (isOnSearch) {
        bottomLine->setStyleSheet("background-color: #A8D8B9;");
    }
    else if (isHovered || !lineEdit->text().isEmpty() || lineEdit->hasFocus()) {
        bottomLine->setStyleSheet("background-color: #454F61;");
    } else {
        bottomLine->setStyleSheet("background-color: transparent;");
    }
}

void SearchWidget::loseAttention()
{   
    qDebug() << "SearchWidget::loseAttention;";
    if (isOnSearch) {
        qDebug() << "    on search;";
        return;
    }
    lineEdit->setText("+");
    lineEdit->clear();
}