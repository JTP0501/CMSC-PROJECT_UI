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
    task.complete;
    task.score = 0;
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

TaskEditScoreDialog::TaskEditScoreDialog(QWidget *parent)
    : QDialog(parent), m_scoreSpinBox(new QDoubleSpinBox(this)), m_totalScoreLabel(new QLabel(this)), m_totalScore(0.0)
{
    setupScoreUI();
}

void TaskEditScoreDialog::setTotalScore(double totalScore)
{
    m_totalScore = totalScore;
    m_totalScoreLabel->setText(QString("Total Score: %1").arg(totalScore));
}

double TaskEditScoreDialog::getScore() const
{
    return m_scoreSpinBox->value();
}

void TaskEditScoreDialog::saveTaskEdit()
{
    // Save the score value
    double editedScore = m_scoreSpinBox->value();
    setScore(editedScore); // Update the score member variable
    accept(); // Accept the changes and close the dialog
}

void TaskEditScoreDialog::setScore(double score)
{
    m_scoreSpinBox->setValue(score);
}

void TaskEditScoreDialog::setupScoreUI()
{
    // Setup the dialog layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Add the score input field
    mainLayout->addWidget(new QLabel("Score:", this));
    m_scoreSpinBox->setRange(0.0, 200.0);
    mainLayout->addWidget(m_scoreSpinBox);

    // Add the total score label
    mainLayout->addWidget(m_totalScoreLabel);

    // Add buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &TaskEditScoreDialog::saveTaskEdit);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &TaskEditScoreDialog::reject);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
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
    task.score = 0;

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
