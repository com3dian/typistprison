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
    void applyBlockFormatting(QTextBlock &block, bool isFirstBlock = false);
    void changeFontSize(int delta);

    int globalFontSize;

    int getVisibleCenterY();
    int getBlockCenterY(const QTextBlock &block);
    QTextBlock findBlockClosestToCenter();
};

#endif // FICTIONTEXTEDIT_H
