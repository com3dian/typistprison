#include "menubutton.h"

MenuButton::MenuButton(const QString &leftText, const QString &rightText, QWidget *parent)
    : QPushButton(parent) 
{
    setStyleSheet("QPushButton { border: none; padding: 0px; }");

    QLabel *leftLabel = new QLabel(leftText, this);
    QLabel *rightLabel = new QLabel(rightText, this);

    leftLabel->setStyleSheet("color: #BDBDBD;");
    rightLabel->setStyleSheet("color: transparent;");

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
    findChild<QLabel *>()->setStyleSheet("color: #FFFFFF;");
    findChildren<QLabel *>().last()->setStyleSheet("color: #656565;");
    QPushButton::enterEvent(event);
}

void MenuButton::leaveEvent(QEvent *event) {
    findChild<QLabel *>()->setStyleSheet("color: #BDBDBD;");
    findChildren<QLabel *>().last()->setStyleSheet("color: transparent;");
    QPushButton::leaveEvent(event);
}
