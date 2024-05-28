#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>

class Console : public QWidget
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);

private slots:
    void onSendCommand();

private:
    QTextEdit* m_consoleOutput;
    QLineEdit* m_consoleInput;
};

#endif // CONSOLE_H
