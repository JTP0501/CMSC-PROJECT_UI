#include "todolist.h" // Include the header file for CToDoList class

#include <QApplication> // Include QApplication for managing the application
#include <QLocale> // Include QLocale for handling locales
#include <QTranslator> // Include QTranslator for localization

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Create a QApplication instance

    a.setStyleSheet(
        "QListView { "
        "   background-color: rgba(255, 125, 41, 0.3); "
        "   border: 1px solid black; "  // 1px solid black border
        "} "
        );

    // Translator stuff, do not touch
    QTranslator translator; // Create a translator object for handling translations

    const QStringList uiLanguages = QLocale::system().uiLanguages(); // Get the list of UI languages from the system
    for (const QString &locale : uiLanguages) {
        const QString baseName = "cmsc_thetaproj_" + QLocale(locale).name(); // Construct the base name for translation files
        if (translator.load(":/i18n/" + baseName)) { // Try loading the translation file
            a.installTranslator(&translator); // Install the translator if the translation file is loaded successfully
            break; // Exit the loop once a translation file is found and loaded
        }
    }

    CToDoList app; // Create an instance of the CToDoList class, which represents the main application window
    app.show(); // Show the main application window

    return a.exec(); // Execute the application event loop
}
