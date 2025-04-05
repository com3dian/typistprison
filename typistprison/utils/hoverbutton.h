#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QEnterEvent>

class HoverButton : public QPushButton {
    Q_OBJECT
public:
    explicit HoverButton(const QString &text, const QIcon &hoverIcon, QWidget *parent = nullptr)
        : QPushButton(text, parent), hoverIcon(hoverIcon)
    {
        normalIcon = QIcon(":/icons/emptyicon.png");
        setIcon(normalIcon);
        setIconSize(QSize(16, 16));
        setStyleSheet(
            "QPushButton {"
            "    padding: 2px 14px 2px 14px;"
            "    color: transparent;"
            "    background-color: transparent;"
            "    border: 1px solid transparent;"
            "    border-radius: 4px;"
            "    text-align: left;"
            "}"
        );
    }

protected:
    // Override using QEnterEvent instead of QEvent
    void enterEvent(QEnterEvent *event) override {
        QPushButton::enterEvent(event);
        setIcon(hoverIcon);
        setStyleSheet(
            "QPushButton {"
            "    padding: 2px 14px 2px 14px;"
            "    color: #FFFFFF;"
            "    background-color: transparent;"
            "    border: 1px solid #BDBDBD;"
            "    border-radius: 4px;"
            "    text-align: left;"
            "}"
        );
    }

    // leaveEvent remains unchanged (QEvent is fine)
    void leaveEvent(QEvent *event) override {
        QPushButton::leaveEvent(event);
        setIcon(normalIcon);
        setStyleSheet(
            "QPushButton {"
            "    padding: 2px 14px 2px 14px;"
            "    color: transparent;"
            "    background-color: transparent;"
            "    border: 1px solid transparent;"
            "    border-radius: 4px;"
            "    text-align: left;"
            "}"
        );
    }

private:
    QIcon normalIcon;
    QIcon hoverIcon;
};

#endif // HOVERBUTTON_H