#include "console.h" // Include the header file for the Console class

#include <QVBoxLayout> // Include necessary Qt header files
#include <QLabel>

// Constructor
Console::Console(QWidget *parent) : QWidget(parent)
{
    // Create a vertical layout for the console widget
    QVBoxLayout* pConsoleLayout = new QVBoxLayout(this);

    // Create a label for the console
    QLabel* plblConsole = new QLabel("Console", this);
    plblConsole->setStyleSheet("font-size: 15pt;"); // Set style for the label
    pConsoleLayout->addWidget(plblConsole); // Add the label to the layout

    // Create a text edit area for console output
    m_consoleOutput = new QTextEdit(this);
    m_consoleOutput->setReadOnly(true); // Set the text edit area to read-only
    pConsoleLayout->addWidget(m_consoleOutput); // Add the text edit area to the layout

    // Create a line edit for console input
    m_consoleInput = new QLineEdit(this);
    // Connect the returnPressed signal of the line edit to the onSendCommand slot
    connect(m_consoleInput, &QLineEdit::returnPressed, this, &Console::onSendCommand);
    pConsoleLayout->addWidget(m_consoleInput); // Add the line edit to the layout
}

// Slot to handle command input
void Console::onSendCommand()
{
    QString command = m_consoleInput->text(); // Get the text from the line edit
    m_consoleOutput->append("User: " + command); // Append the command to the console output
    // Process the command here
    m_consoleInput->clear(); // Clear the input line edit after processing the command
}
