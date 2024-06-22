#ifndef FICTIONTEXTEDIT_H
#define FICTIONTEXTEDIT_H

#include <QTextEdit>
#include <QList>
#include <QTextBlock>

class FictionTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit FictionTextEdit(QWidget *parent = nullptr);
    void load(const QString& text);
    void setTopMargin(int margin);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void insertFromMimeData(const QMimeData *source) override;

private:
    void applyBlockFormatting(QTextBlock &block, bool isFirstBlock);
    void updateTextColor();
    int getVisibleCenterY();
    int getBlockCenterY(const QTextBlock &block);
    QTextBlock findBlockClosestToCenter();

    void changeFontSize(int delta);
    QTextBlock previousCenteredBlock;

    int globalFontSize;


};

#endif // FICTIONTEXTEDIT_H
