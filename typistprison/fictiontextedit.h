#ifndef FICTIONTEXTEDIT_H
#define FICTIONTEXTEDIT_H

#include <QTextEdit>
#include <QList>
#include <QTextBlock>
#include <string>
#include "searchHighlighter.h"

class FictionTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit FictionTextEdit(QWidget *parent = nullptr);
    void load(const QString& text);
    void setTopMargin(int margin);
    void activateHighlightMode();
    void deactivateHighlightMode();
    void search(const QString &searchString);
    void clearSearch();

signals:
    void onFictionEditSearch(const QString &text);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void insertFromMimeData(const QMimeData *source) override;
    SearchHighlighter *highlighter;

private:
    void applyBlockFormatting(QTextBlock &block, bool isCheck = true);
    void updateFocusBlock();
    void changeGlobalTextColor(const QColor &color);
    int getVisibleCenterY();
    std::string checkVisibleCenterBlock(const QTextBlock &block);
    QTextBlock findBlockClosestToCenter();

    void changeFontSize(int delta);
    QTextBlock previousCenteredBlock;
    QTextBlock newCenteredBlock;
    bool isHighlightMode;

    int globalFontSize;


};

#endif // FICTIONTEXTEDIT_H
