// Include necessary headers
#include "taskeditdialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>

// Constructor
TaskEditDialog::TaskEditDialog(QWidget *parent)
    : QDialog(parent)
{
    // Set window title
    setWindowTitle("Edit Task");

    // Create form layout for input fields
    QFormLayout *layout = new QFormLayout(this);

    // Create input fields for task details
    m_taskNameLineEdit = new QLineEdit(this);
    m_semesterLineEdit = new QLineEdit(this);
    m_courseLineEdit = new QLineEdit(this);
    m_weightLineEdit = new QLineEdit(this);
    m_totalScoreSpinBox = new QDoubleSpinBox(this);
    // No minimum or maximum values set
    // This allows any value to be entered

    // Add input fields to the layout
    layout->addRow("Task Name:", m_taskNameLineEdit);
    layout->addRow("Semester:", m_semesterLineEdit);
    layout->addRow("Course:", m_courseLineEdit);
    layout->addRow("Weight Component:", m_weightLineEdit);
    layout->addRow("Total Score:", m_totalScoreSpinBox);

    // Create buttons for saving or canceling changes
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, Qt::Horizontal, this);

    // Connect the button box signals to slots
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TaskEditDialog::saveTask);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Add the button box to the layout
    layout->addWidget(buttonBox);
}

// Method to set initial values for input fields
void TaskEditDialog::setTask(const Task &task)
{
    m_taskNameLineEdit->setText(task.taskName);
    m_semesterLineEdit->setText(task.semester);
    m_courseLineEdit->setText(task.course);
    m_weightLineEdit->setText(task.weight);
    m_totalScoreSpinBox->setValue(task.totalScore);
}

// Slot to save the edited task
void TaskEditDialog::saveTask()
{
    // Create a Task object and populate it with values from input fields
    Task task;
    task.taskName = m_taskNameLineEdit->text();
    task.semester = m_semesterLineEdit->text();
    task.course = m_courseLineEdit->text();
    task.weight = m_weightLineEdit->text();
    task.totalScore = m_totalScoreSpinBox->value();

    // Emit the taskEdited signal with the updated task data
    emit taskEdited(task);

    // Close the dialog
    accept();
}
