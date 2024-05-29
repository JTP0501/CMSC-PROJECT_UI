#ifndef TASKEDITDIALOG_H
#define TASKEDITDIALOG_H

#include <QDialog> // Include necessary Qt header files
#include "customstringlistmodel.h" // Include the header file where Task is defined

class QLineEdit; // Forward declaration of QLineEdit class
class QDoubleSpinBox; // Forward declaration of QDoubleSpinBox class

class TaskEditDialog : public QDialog // Define TaskEditDialog class inheriting from QDialog
{
    Q_OBJECT // Add Q_OBJECT macro to enable signals and slots

public:
    TaskEditDialog(QWidget *parent = nullptr); // Constructor declaration
    void setTask(const Task &task); // Function to set the task data in the dialog
    Task getTask() const; // Function to get the task data in the dialog

signals:
    void taskEdited(const Task &task); // Signal emitted when the task is edited and saved

private slots:
    void saveTask(); // Slot to handle saving the edited task

private:
    QLineEdit *m_taskNameLineEdit; // Pointer to QLineEdit for task name input
    QLineEdit *m_semesterLineEdit; // Pointer to QLineEdit for semester input
    QLineEdit *m_courseLineEdit; // Pointer to QLineEdit for course input
    QLineEdit *m_weightLineEdit; // Pointer to QLineEdit for weight input
    QDoubleSpinBox *m_totalScoreSpinBox; // Pointer to QDoubleSpinBox for total score input

    Task m_task;  // Declare m_task as a member variable
};

#endif // TASKEDITDIALOG_H
