#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget> // Include QWidget for the base class
#include <QTextEdit> // Include QTextEdit for console output
#include <QLineEdit> // Include QLineEdit for console input

class Console : public QWidget // Define Console class inheriting from QWidget
{
    Q_OBJECT // Add Q_OBJECT macro to enable signals and slots

public:
    explicit Console(QWidget *parent = nullptr); // Constructor declaration

private slots:
    void onSendCommand(); // Slot to handle sending a command

private:
    QTextEdit* m_consoleOutput; // QTextEdit for displaying console output
    QLineEdit* m_consoleInput; // QLineEdit for user input in the console
};

#endif // CONSOLE_H
