#include "window/window.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char* argv[]) {
    QApplication a{argc, argv};
    // QApplication::setStyle("Fusion");

    QTranslator translator{};
    const QStringList uiLanguages{QLocale::system().uiLanguages()};
    for (const QString& locale : uiLanguages) {
        const QString baseName{"drawly_" + QLocale(locale).name()};
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w{};
    w.show();
    return a.exec();
}
