#include "menubutton.h"

MenuButton::MenuButton(const QString &leftText, const QString &rightText, QWidget *parent)
    : QPushButton(parent) 
{
    setStyleSheet("QPushButton { border: none; padding: 0px; background-color: transparent; }");

    QLabel *leftLabel = new QLabel(leftText, this);
    QLabel *rightLabel = new QLabel(rightText, this);

    leftLabel->setStyleSheet("color: #BDBDBD; background-color: transparent;");
    rightLabel->setStyleSheet("color: transparent; background-color: transparent;");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(leftLabel);
    layout->addStretch();
    layout->addWidget(rightLabel);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);
    setAttribute(Qt::WA_Hover);

    // Connect QPushButton's clicked signal to emit our custom mouseClick signal
    connect(this, &QPushButton::clicked, this, &MenuButton::mouseClick);
}

void MenuButton::setTexts(const QString &leftText, const QString &rightText) {
    findChild<QLabel *>()->setText(leftText);
    findChildren<QLabel *>().last()->setText(rightText);
}

void MenuButton::enterEvent(QEnterEvent *event) {
    findChild<QLabel *>()->setStyleSheet("color: #FFFFFF; background-color: transparent;");
    findChildren<QLabel *>().last()->setStyleSheet("color: #656565; background-color: transparent;");
    QPushButton::enterEvent(event);
    emit hovered();
}

void MenuButton::leaveEvent(QEvent *event) {
    findChild<QLabel *>()->setStyleSheet("color: #BDBDBD; background-color: transparent;");
    findChildren<QLabel *>().last()->setStyleSheet("color: transparent; background-color: transparent;");
    QPushButton::leaveEvent(event);
    emit notHovered();
}

void MenuButton::showEvent(QShowEvent *event) {
    QPushButton::showEvent(event);
    
    // Mac-specific fix: Force initial label styling to ensure proper text rendering
    #ifdef Q_OS_MAC
    QLabel *leftLabel = findChild<QLabel *>();
    QLabel *rightLabel = findChildren<QLabel *>().last();
    if (leftLabel) {
        leftLabel->setStyleSheet("color: #BDBDBD; background-color: transparent;");
        leftLabel->update();
    }
    if (rightLabel) {
        rightLabel->setStyleSheet("color: transparent; background-color: transparent;");
        rightLabel->update();
    }
    #endif
}
