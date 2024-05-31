#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QLineEdit>

class Console : public QWidget
{
    Q_OBJECT

public:
    explicit Console(QWidget *parent = nullptr);

private slots:
    void onSendCommand();
    void processCommand(const QString &command);

private:
    void createSubjectFiles();
    QTextEdit* m_consoleOutput;
    QLineEdit* m_consoleInput;
    QString csvFilePath;
};

#endif // CONSOLE_H
