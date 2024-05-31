#include "console.h"
#include <QVBoxLayout>
#include <QLabel>

Console::Console(QWidget *parent) : QWidget(parent)
{
    // Create a vertical layout for the console widget
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Create a label for the console
    QLabel* consoleLabel = new QLabel("Console", this);
    consoleLabel->setStyleSheet("font-size: 15pt;");
    layout->addWidget(consoleLabel);

    // Create a text edit area for console output
    m_consoleOutput = new QTextEdit(this);
    m_consoleOutput->setReadOnly(true); // Set to read-only
    layout->addWidget(m_consoleOutput);

    // Create a line edit for console input
    m_consoleInput = new QLineEdit(this);
    // Connect the returnPressed signal of the line edit to the onSendCommand slot
    connect(m_consoleInput, &QLineEdit::returnPressed, this, &Console::onSendCommand);
    layout->addWidget(m_consoleInput);
}

void Console::onSendCommand()
{
    QString command = m_consoleInput->text().trimmed(); // Get the trimmed text from the line edit
    if (!command.isEmpty())
    {
        // Append user command to console output
        m_consoleOutput->append("User: " + command);
        // Process the command here
    }
    m_consoleInput->clear();
}
