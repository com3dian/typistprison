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

    // QScreen *screen = QGuiApplication::primaryScreen(); // Use QGuiApplication to get the primary screen
    // qreal scalingFactor = screen->devicePixelRatio(); // Correctly call devicePixelRatio()
    // qDebug() << "scalingFactor: " << scalingFactor;

    app.setWindowIcon(QIcon(":/icons/typrison_icon.png"));

    MainWindow w;

    QString notoSansLightFamily = FontManager::instance().getNotoSansLightFamily();
    QString notoSansDemiLightFamily = FontManager::instance().getNotoSansDemiLightFamily();
    QString notoSansMonoFamily = FontManager::instance().getNotoSansMonoFamily();
    QString notoSansRegularFamily = FontManager::instance().getNotoSansRegularFamily();

    #if defined(Q_OS_WIN)
        qDebug() << "Running on Windows";
        QString styleSheet = "QWidget { font-family: 'Noto Sans CJK SC Light';}"
                             "PlaintextEdit { font-family: 'Noto Mono'; }"
                             "QMarkdownTextEdit { font-family: %1; }"
                             "FictionTextEdit { font-family: %1; }" ;
    #elif defined(Q_OS_MAC)
        qDebug() << "Running on MacOS";
        QString styleSheet = QString(
                             "QWidget { font-family: '%1';}"
                             "PlaintextEdit { font-family: '%2'; }"
                             "QMarkdownTextEdit { font-family: '%3'; }"
                             "FictionTextEdit { font-family: '%4'; }"
                             ).arg(notoSansLightFamily)
                              .arg(notoSansMonoFamily)
                              .arg(notoSansLightFamily)
                              .arg(notoSansLightFamily);

    #elif defined(Q_OS_LINUX)
        qDebug() << "Running on Linux";
        QString styleSheet = QString(
                             "QWidget { font-family: '%1';}"
                             "PlaintextEdit { font-family: '%2'; }"
                             "QMarkdownTextEdit { font-family: '%3'; }"
                             "FictionTextEdit { font-family: '%4'; }"
                             ).arg(notoSansLightFamily)
                              .arg(notoSansMonoFamily)
                              .arg(notoSansLightFamily)
                              .arg(notoSansLightFamily);
        
    #else
        qDebug() << "Unknown Operating System";
    #endif

    // Set the font as the default application font
    app.setStyleSheet(styleSheet);

    w.setWindowIcon(QIcon(":/icons/typrison_icon.png"));
    w.show();
    return app.exec();
}
