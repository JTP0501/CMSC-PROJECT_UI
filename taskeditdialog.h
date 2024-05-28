#ifndef TASKEDITDIALOG_H
#define TASKEDITDIALOG_H

#include <QDialog>
#include "customstringlistmodel.h" // Include the header file where Task is defined

class QLineEdit;
class QDoubleSpinBox;

class TaskEditDialog : public QDialog
{
    Q_OBJECT

public:
    TaskEditDialog(QWidget *parent = nullptr);
    void setTask(const Task &task);

signals:
    void taskEdited(const Task &task);

private slots:
    void saveTask();

private:
    QLineEdit *m_taskNameLineEdit;
    QLineEdit *m_semesterLineEdit;
    QLineEdit *m_courseLineEdit;
    QLineEdit *m_weightLineEdit;
    QDoubleSpinBox *m_totalScoreSpinBox;
};

#endif // TASKEDITDIALOG_H
