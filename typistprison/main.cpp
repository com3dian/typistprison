#include "mainwindow.h"
#include "fontmanager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSysInfo>
#include <QFontDatabase>
#include <QDebug>
#include <QScreen>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    // Warning: ‘Qt::AA_EnableHighDpiScaling’ is deprecated: High-DPI scaling is always enabled. 
    // This attribute no longer has any effect.
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "typistprison_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QScreen *screen = QGuiApplication::primaryScreen(); // Use QGuiApplication to get the primary screen
    qreal scalingFactor = screen->devicePixelRatio(); // Correctly call devicePixelRatio()
    qDebug() << "scalingFactor: " << scalingFactor;

    MainWindow w;

    QString notoSansLightFamily = FontManager::instance().getNotoSansLightFamily();
    QString notoSansDemiLightFamily = FontManager::instance().getNotoSansDemiLightFamily();
    QString notoSansMonoFamily = FontManager::instance().getNotoSansMonoFamily();
    QString notoSansRegularFamily = FontManager::instance().getNotoSansRegularFamily();

    // QFont customFont(customFontFamily, 12); // TODO: to be changed to read default font size from 
    // myWidget->setFont(customFont);
    
    // int notoSansLightId = QFontDatabase::addApplicationFont(":/fonts/fonts/NotoSansSC-Light.ttf");
    // int notoSansRegularId = QFontDatabase::addApplicationFont(":/fonts/fonts/NotoSansSC-Regular.ttf");
    // int notoSansDemiLightId = QFontDatabase::addApplicationFont(":/fonts/fonts/NotoSansSC-DemiLight.otf");
    // int notoSansMonoId = QFontDatabase::addApplicationFont(":/fonts/fonts/NotoSansMono-Regular.ttf");
    // int notoSansSerifId = QFontDatabase::addApplicationFont(":/fonts/fonts/NotoSerifTC-Regular.ttf");


    // QString notoSansLightFamily;
    // QString notoSansRegularFamily;
    // QString notoSansDemiLightFamily;
    // QString notoSansMonoFamily;
    // QString notoSansSerifFamily;

    // Retrieve the font family names safely
    // QStringList notoSansLightFamilies = QFontDatabase::applicationFontFamilies(notoSansLightId);
    // if (!notoSansLightFamilies.isEmpty()) {
    //     notoSansLightFamily = notoSansLightFamilies.at(0);
    // } else {
    //     qWarning() << "Failed to load Noto Sans Light font family.";
    // }

    // QStringList notoSansRegularFamilies = QFontDatabase::applicationFontFamilies(notoSansRegularId);
    // if (!notoSansRegularFamilies.isEmpty()) {
    //     notoSansRegularFamily = notoSansRegularFamilies.at(0);
    // } else {
    //     qWarning() << "Failed to load Noto Sans Regular font family.";
    // }

    // QStringList notoSansDemiLightFamilies = QFontDatabase::applicationFontFamilies(notoSansDemiLightId);
    // if (!notoSansDemiLightFamilies.isEmpty()) {
    //     notoSansDemiLightFamily = notoSansDemiLightFamilies.at(0);
    // } else {
    //     qWarning() << "Failed to load Noto Sans DemiLight font family.";
    // }

    // QStringList notoSansMonoFamilies = QFontDatabase::applicationFontFamilies(notoSansMonoId);
    // if (!notoSansMonoFamilies.isEmpty()) {
    //     notoSansMonoFamily = notoSansMonoFamilies.at(0);
    // } else {
    //     qWarning() << "Failed to load Noto Sans Mono font family.";
    // }

    // QStringList notoSansSerifFamilies = QFontDatabase::applicationFontFamilies(notoSansSerifId);
    // if (!notoSansSerifFamilies.isEmpty()) {
    //     notoSansSerifFamily = notoSansSerifFamilies.at(0);
    // } else {
    //     qWarning() << "Failed to load Noto Sans Mono font family.";
    // }

    #if defined(Q_OS_WIN)
        qDebug() << "Running on Windows";
        QString styleSheet = "QWidget { font-family: 'Noto Sans CJK SC Light';}"
                             "PlaintextEdit { font-family: 'Noto Mono'; }"
                             "QMarkdownTextEdit { font-family: %1; }"
                             "FictionTextEdit { font-family: %1; }" ;
    #elif defined(Q_OS_MAC)
        qDebug() << "Running on MacOS";
        QString styleSheet = QString(
                             "QWidget { font-family: '%1'; font-size: %2px;}"
                             "PlaintextEdit { font-family: '%3'; }"
                             "QMarkdownTextEdit { font-family: '%4'; }"
                             "FictionTextEdit { font-family: '%5'; }"
                             ).arg(notoSansLightFamily)
                              .arg(12)
                              .arg(notoSansMonoFamily)
                              .arg(notoSansLightFamily)
                              .arg(notoSansLightFamily);

    #elif defined(Q_OS_LINUX)
        qDebug() << "Running on Linux";
        QString styleSheet = QString(
                             "QWidget { font-family: '%1'; font-size: %2px;}"
                             "PlaintextEdit { font-family: '%3'; }"
                             "QMarkdownTextEdit { font-family: '%4'; }"
                             "FictionTextEdit { font-family: '%5'; }"
                             ).arg(notoSansLightFamily)
                              .arg(12)
                              .arg(notoSansMonoFamily)
                              .arg(notoSansLightFamily)
                              .arg(notoSansLightFamily);
        
    #else
        qDebug() << "Unknown Operating System";
    #endif

    // Set the font as the default application font
    app.setStyleSheet(styleSheet);

    w.show();
    return app.exec();
}
