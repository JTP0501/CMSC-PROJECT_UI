#ifndef TASKEDITDIALOG_H
#define TASKEDITDIALOG_H

#include <QDialog>
#include "todolist.h" // Include the header file where Task is defined

class QLineEdit;
class QDoubleSpinBox;

class TaskEditDialog : public QDialog
{
    Q_OBJECT

public:
    TaskEditDialog(QWidget *parent = nullptr);
    void setTaskEdit(const Task &task);
    Task getTaskEdit() const;

private slots:
    void saveTaskEdit();

private:
    void setupEditUI();

    QLineEdit *m_taskNameLineEdit;
    QLineEdit *m_courseLineEdit;
    QLineEdit *m_weightLineEdit;
    QDoubleSpinBox *m_totalScoreSpinBox;
};

class TaskAddDialog : public QDialog
{
    Q_OBJECT

public:
    TaskAddDialog(QWidget *parent = nullptr);
    void setTaskAdd(const Task &task);
    Task getTaskAdd() const;

private slots:
    void saveTaskAdd();

private:
    void setupAddUI();

    QLineEdit *m_taskNameLineEdit;
    QLineEdit *m_courseLineEdit;
    QLineEdit *m_weightLineEdit;
    QDoubleSpinBox *m_totalScoreSpinBox;
};

#endif // TASKEDITDIALOG_H
