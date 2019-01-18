#include "mainwindow.h"
#include "settings.h"
#include "turnstile.h"

#include "version.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("zec-qt-wallet-org");
    QCoreApplication::setApplicationName("zec-qt-wallet");

    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf('_'));   // Get the language code
    qDebug() << "Loading locale " << locale;
    
    QTranslator translator;
    translator.load(QString(":/translations/res/zec_qt_wallet_") + locale);
    a.installTranslator(&translator);

    QIcon icon(":/icons/res/icon.ico");
    QApplication::setWindowIcon(icon);

    #ifdef Q_OS_LINUX
        QFontDatabase::addApplicationFont(":/fonts/res/Ubuntu-R.ttf");
        qApp->setFont(QFont("Ubuntu", 11, QFont::Normal, false));
    #endif

    // QRandomGenerator generates a secure random number, which we use to seed.
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    unsigned int seed = QRandomGenerator::securelySeeded().generate();
#else
    // This will be used only during debugging for compatibility reasons 
    unsigned int seed = std::time(0);
#endif
    std::srand(seed);

    Settings::init();

    // Set up libsodium
    if (sodium_init() < 0) {
        /* panic! the library couldn't be initialized, it is not safe to use */
        qDebug() << "libsodium is not initialized!";
    }

    if (argc >= 2 && QString::fromStdString(argv[1]) == "--no-embedded") {
        Settings::getInstance()->setUseEmbedded(false);
    } else {
        Settings::getInstance()->setUseEmbedded(true);
    }

    MainWindow w;
    w.setWindowTitle("zec-qt-wallet v" + QString(APP_VERSION));
    w.show();
    
    return QApplication::exec();
}
