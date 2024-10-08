#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <QFontDatabase>
#include <QString>

class FontManager
{
public:
    static FontManager& instance();
    
    QString getNotoSansLightFamily() const { return notoSansLightFamily; }
    QString getNotoSansRegularFamily() const { return notoSansRegularFamily; }
    QString getNotoSansDemiLightFamily() const { return notoSansDemiLightFamily; }
    QString getNotoSansMonoFamily() const { return notoSansMonoFamily; }
    QString getNotoSansSerifFamily() const { return notoSansSerifFamily; }

private:
    FontManager();
    void loadFonts();

    QString notoSansLightFamily;
    QString notoSansRegularFamily;
    QString notoSansDemiLightFamily;
    QString notoSansMonoFamily;
    QString notoSansSerifFamily;
};

#endif // FONTMANAGER_H
