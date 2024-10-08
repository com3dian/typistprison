#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H
#include <QProxyStyle>
#include <QStyleOptionMenuItem>

class CustomStyle : public QProxyStyle{
public:
    using QProxyStyle::QProxyStyle;

    void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const override
    {
        if(element == QStyle::CE_MenuItem){
            QStyleOptionMenuItem myMenuOption;
            const QStyleOptionMenuItem *menuOption =
                    qstyleoption_cast<const QStyleOptionMenuItem *>(opt);
            if (menuOption) {
                const int width = pixelMetric(PM_SmallIconSize)+6;
                myMenuOption = *menuOption;
                QRect r(myMenuOption.rect);
                r.setLeft(-width);
                myMenuOption.rect = r;
            }
            QProxyStyle::drawControl(element, &myMenuOption, p, w);
            return;
        }


        // set draw control for other element
        QProxyStyle::drawControl(element, opt, p, w);
    }
};

#endif // CUSTOMSTYLE_H
