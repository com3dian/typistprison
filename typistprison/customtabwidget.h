#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QTextEdit>
#include <QTabBar>
#include <QStylePainter>
#include <QPainterPath>
#include <QStyleOptionTab>


class CustomTabBar : public QTabBar {
// protected:
    // void paintEvent(QPaintEvent* event) override {
    //     QStylePainter painter(this);
    //     for (int i = 0; i < count(); ++i) {
    //         QStyleOptionTab option;
    //         initStyleOption(&option, i);

    //         option.rect = option.rect.adjusted(0, 0, 0, -1); // Remove the bottom border

    //         // Set custom shape for the tab with only upper corners rounded
    //         QRect rect = option.rect;
    //         QPainterPath path;
    //         path.moveTo(rect.bottomLeft());
    //         path.lineTo(rect.topLeft() - QPointF(0, 4));
    //         path.arcTo(rect.topLeft().x(), rect.topLeft().y(), 8, 8, 180, -90);
    //         path.lineTo(rect.topRight() - QPointF(4, 0));
    //         path.arcTo(rect.topRight().x() - 8, rect.topRight().y(), 8, 8, 90, -90);
    //         path.lineTo(rect.bottomRight());
    //         // path.lineTo(rect.bottomLeft());

    //         painter.setClipPath(path);

    //         painter.drawControl(QStyle::CE_TabBarTabShape, option);
    //         painter.drawControl(QStyle::CE_TabBarTabLabel, option);
    //         }
    // }
};

class CustomTabWidget : public QTabWidget {
    Q_OBJECT

public:
    CustomTabWidget(QWidget *parent = nullptr);
    void createNewTab(const QString &content, const QString &tabName);
    void switchToFictionView();
    // void switchToEditorView();

private:
    void setupTabWidget();
    void setupTabBar();
    void setupStyles();
    void closeWindowIfNoTabs(int index);
    void applyFictionViewStyles(QTextEdit *textEdit);
    void applyEditorViewStyles(QTextEdit *textEdit);

signals:
    void lastTabClosed();

};

#endif // CUSTOMTABWIDGET_H
