#ifndef PRISONERFICTIONTEXTEDIT_H
#define PRISONERFICTIONTEXTEDIT_H

#include "fictiontextedit.h"

class PrisonerFictionTextEdit : public FictionTextEdit {
    Q_OBJECT

public:
    explicit PrisonerFictionTextEdit(QWidget *parent = nullptr);

signals:
    void onEscape();

protected:
    // Override existing methods if needed
    void keyPressEvent(QKeyEvent *event) override;

private:
    bool isPrisoner;

    // Additional functionality or overrides
    void activateSniperMode() override;
    void deactivateSniperMode() override;

};

#endif // PRISONERFICTIONTEXTEDIT_H
