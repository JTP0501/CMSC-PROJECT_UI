// taskeditdialog.cpp
#include "taskeditdialog.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>

TaskEditDialog::TaskEditDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Edit Task");

    QFormLayout *layout = new QFormLayout(this);
    m_taskNameLineEdit = new QLineEdit(this);
    m_semesterLineEdit = new QLineEdit(this);
    m_courseLineEdit = new QLineEdit(this);
    m_weightLineEdit = new QLineEdit(this);
    m_totalScoreSpinBox = new QDoubleSpinBox(this);
    // No minimum or maximum values set
    // This allows any value to be entered

    layout->addRow("Task Name:", m_taskNameLineEdit);
    layout->addRow("Semester:", m_semesterLineEdit);
    layout->addRow("Course:", m_courseLineEdit);
    layout->addRow("Weight Component:", m_weightLineEdit);
    layout->addRow("Total Score:", m_totalScoreSpinBox);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TaskEditDialog::saveTask);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox);
}

void TaskEditDialog::setTask(const Task &task)
{
    m_taskNameLineEdit->setText(task.taskName);
    m_semesterLineEdit->setText(task.semester);
    m_courseLineEdit->setText(task.course);
    m_weightLineEdit->setText(task.weight);
    m_totalScoreSpinBox->setValue(task.totalScore);
}

void TaskEditDialog::saveTask()
{
    Task task;
    task.taskName = m_taskNameLineEdit->text();
    task.semester = m_semesterLineEdit->text();
    task.course = m_courseLineEdit->text();
    task.weight = m_weightLineEdit->text();
    task.totalScore = m_totalScoreSpinBox->value();

    emit taskEdited(task);
    accept();
}
