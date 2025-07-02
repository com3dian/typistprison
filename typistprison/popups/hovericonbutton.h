#ifndef HOVERICONBUTTON_H
#define HOVERICONBUTTON_H

#include <QPushButton>
#include <QIcon>
#include <QString>

class HoverIconButton : public QPushButton {
    Q_OBJECT

public:
    explicit HoverIconButton(const QString &text = "", QWidget *parent = nullptr);
    void setIcons(const QString &normalIconPath, const QString &hoverIconPath);
    void setIconSize(const QSize &size);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QIcon normalIcon;
    QIcon hoverIcon;
    QSize iconSize;
};

#endif // HOVERICONBUTTON_H