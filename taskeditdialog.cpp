#include "taskeditdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDoubleSpinBox>

TaskEditDialog::TaskEditDialog(QWidget *parent)
    : QDialog(parent),
    m_taskNameLineEdit(new QLineEdit(this)),
    m_courseLineEdit(new QLineEdit(this)),
    m_weightLineEdit(new QLineEdit(this)),
    m_totalScoreSpinBox(new QDoubleSpinBox(this))
{
    setupEditUI();
}

void TaskEditDialog::setupEditUI()
{
    // Setup the dialog layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Add input fields
    mainLayout->addWidget(new QLabel("Task Name:", this));
    mainLayout->addWidget(m_taskNameLineEdit);

    mainLayout->addWidget(new QLabel("Course:", this));
    mainLayout->addWidget(m_courseLineEdit);

    mainLayout->addWidget(new QLabel("Weight Component:", this));
    mainLayout->addWidget(m_weightLineEdit);

    mainLayout->addWidget(new QLabel("Total Score:", this));
    m_totalScoreSpinBox->setRange(0.0, 200.0);
    mainLayout->addWidget(m_totalScoreSpinBox);

    // Add buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TaskEditDialog::saveTaskEdit);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TaskEditDialog::reject);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void TaskEditDialog::setTaskEdit(const Task &task)
{
    m_taskNameLineEdit->setText(task.taskName);
    m_courseLineEdit->setText(task.course);
    m_weightLineEdit->setText(task.weight);
    m_totalScoreSpinBox->setValue(task.totalScore);
}

Task TaskEditDialog::getTaskEdit() const
{
    Task task;
    task.taskName = m_taskNameLineEdit->text();
    task.course = m_courseLineEdit->text();
    task.weight = m_weightLineEdit->text();
    task.totalScore = m_totalScoreSpinBox->value();
    task.complete = 0; // Default value
    return task;
}

void TaskEditDialog::saveTaskEdit()
{
    if (m_courseLineEdit->text().isEmpty() || m_weightLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Please fill in all required fields.");
    }
    else
    {
        accept();
    }
}

TaskAddDialog::TaskAddDialog(QWidget *parent)
    : QDialog(parent),
    m_taskNameLineEdit(new QLineEdit(this)),
    m_courseLineEdit(new QLineEdit(this)),
    m_weightLineEdit(new QLineEdit(this)),
    m_totalScoreSpinBox(new QDoubleSpinBox(this))
{
    setupAddUI();
}

void TaskAddDialog::setupAddUI()
{
    // Setup the dialog layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Add input fields
    mainLayout->addWidget(new QLabel("Task Name:", this));
    mainLayout->addWidget(m_taskNameLineEdit);

    mainLayout->addWidget(new QLabel("Course:", this));
    mainLayout->addWidget(m_courseLineEdit);

    mainLayout->addWidget(new QLabel("Weight Component:", this));
    mainLayout->addWidget(m_weightLineEdit);

    mainLayout->addWidget(new QLabel("Total Score:", this));
    m_totalScoreSpinBox->setRange(0.0, 200.0);
    mainLayout->addWidget(m_totalScoreSpinBox);

    // Add buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TaskAddDialog::saveTaskAdd);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TaskAddDialog::reject);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void TaskAddDialog::setTaskAdd(const Task &task)
{
    m_taskNameLineEdit->setText(task.taskName);
    m_courseLineEdit->setText(task.course);
    m_weightLineEdit->setText(task.weight);
    m_totalScoreSpinBox->setValue(task.totalScore);
}

Task TaskAddDialog::getTaskAdd() const
{
    Task task;
    task.taskName = m_taskNameLineEdit->text();
    task.course = m_courseLineEdit->text();
    task.weight = m_weightLineEdit->text();
    task.totalScore = m_totalScoreSpinBox->value();
    task.complete = 0; // Default value
    return task;
}

void TaskAddDialog::saveTaskAdd()
{
    if (m_taskNameLineEdit->text().isEmpty() || m_courseLineEdit->text().isEmpty() || m_weightLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Please fill in all required fields.");
    }
    else
    {
        accept();
    }
}
