#include "console.h"

#include <QVBoxLayout>
#include <QLabel>

Console::Console(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* pConsoleLayout = new QVBoxLayout(this);

    QLabel* plblConsole = new QLabel("Console", this);
    plblConsole->setStyleSheet("font-size: 15pt;");
    pConsoleLayout->addWidget(plblConsole);

    m_consoleOutput = new QTextEdit(this);
    m_consoleOutput->setReadOnly(true);
    pConsoleLayout->addWidget(m_consoleOutput);

    m_consoleInput = new QLineEdit(this);
    connect(m_consoleInput, &QLineEdit::returnPressed, this, &Console::onSendCommand);
    pConsoleLayout->addWidget(m_consoleInput);
}

void Console::onSendCommand()
{
    QString command = m_consoleInput->text();
    m_consoleOutput->append("User: " + command);
    // Process the command here
    m_consoleInput->clear();
}
