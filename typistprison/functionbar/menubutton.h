#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QEnterEvent>

class MenuButton : public QPushButton {
    Q_OBJECT

public:
    explicit MenuButton(const QString &leftText, const QString &rightText, QWidget *parent = nullptr);

    void setTexts(const QString &leftText, const QString &rightText);

signals:
    void mouseClick();  // Custom signal

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};

#endif // MENUBUTTON_H
