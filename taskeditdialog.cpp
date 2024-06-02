#include "taskeditdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QApplication>
#include <QFontDatabase>
#include <QLabel>

TaskEditDialog::TaskEditDialog(QWidget *parent)
    : QDialog(parent),
    m_taskNameLineEdit(new QLineEdit(this)),
    m_courseLineEdit(new QLineEdit(this)),
    m_weightLineEdit(new QLineEdit(this)),
    m_totalScoreSpinBox(new QDoubleSpinBox(this))

{
    QFontDatabase::addApplicationFont(":/Fonts/Fixedsys.fon");

    setupEditUI();
    // In TaskEditDialog class
    connect(m_courseLineEdit, &QLineEdit::textChanged, this, &TaskEditDialog::convertToUpperCaseEdit);
    connect(m_weightLineEdit, &QLineEdit::textChanged, this, &TaskEditDialog::convertToUpperCaseEdit);

    // Set the stylesheet
    setStyleSheet("background-color: #97BE5A");
}

void TaskEditDialog::setupEditUI()
{
    // Setup the dialog layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Add input fields
    QLabel *taskNameLabel = new QLabel("Task Name:", this);
    taskNameLabel->setFont(QFont("System", 10));
    taskNameLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(taskNameLabel);
    m_taskNameLineEdit->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';"); // Set black border, font color, and font
    mainLayout->addWidget(m_taskNameLineEdit);

    QLabel *courseLabel = new QLabel("Course:", this);
    courseLabel->setFont(QFont("System", 10));
    courseLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(courseLabel);
    m_courseLineEdit->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';"); // Set black border, font color, and font
    mainLayout->addWidget(m_courseLineEdit);

    QLabel *weightLabel = new QLabel("Weight Component:", this);
    weightLabel->setFont(QFont("System", 10));
    weightLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(weightLabel);
    m_weightLineEdit->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';"); // Set black border, font color, and font
    mainLayout->addWidget(m_weightLineEdit);

    QLabel *totalScoreLabel = new QLabel("Total Score:", this);
    totalScoreLabel->setFont(QFont("System", 10));
    totalScoreLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(totalScoreLabel);
    m_totalScoreSpinBox->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';"); // Set black border, font color, and font
    m_totalScoreSpinBox->setRange(0.0, 200.0);
    mainLayout->addWidget(m_totalScoreSpinBox);

    // Add buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setStyleSheet("color: black; background-color: #FFBF78; font: 10pt 'System';"); // Set black border, font color, and font
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
    //task.complete;
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

    // Set the window title
    setWindowTitle("Add Score");

    // Set the stylesheet
    setStyleSheet("background-color: #97BE5A");
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
    QLabel *scoreLabel = new QLabel("Score:", this);
    scoreLabel->setFont(QFont("System", 10));
    scoreLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(scoreLabel);
    m_scoreSpinBox->setRange(0.0, 200.0);
    m_scoreSpinBox->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';");
    mainLayout->addWidget(m_scoreSpinBox);

    // Add the total score label
    m_totalScoreLabel->setFont(QFont("System", 10));
    m_totalScoreLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(m_totalScoreLabel);

    // Add buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setStyleSheet("color: black; background-color: #FFBF78; font: 10pt 'System';"); // Set black border, font color, and font
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

    // In TaskAddDialog class
    connect(m_courseLineEdit, &QLineEdit::textChanged, this, &TaskAddDialog::convertToUpperCaseAdd);
    connect(m_weightLineEdit, &QLineEdit::textChanged, this, &TaskAddDialog::convertToUpperCaseAdd);

    // Set the stylesheet
    setStyleSheet("background-color: #97BE5A");
}

void TaskAddDialog::setupAddUI()
{
    // Setup the dialog layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Add input fields
    QLabel *taskNameLabel = new QLabel("Task Name:", this);
    taskNameLabel->setFont(QFont("System", 10));
    taskNameLabel->setStyleSheet("color: white");
    mainLayout->addWidget(taskNameLabel);
    m_taskNameLineEdit->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';");
    mainLayout->addWidget(m_taskNameLineEdit);

    QLabel *courseLabel = new QLabel("Course:", this);
    courseLabel->setFont(QFont("System", 10));
    courseLabel->setStyleSheet("color: white");
    mainLayout->addWidget(courseLabel);
    m_courseLineEdit->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';");
    mainLayout->addWidget(m_courseLineEdit);

    QLabel *weightLabel = new QLabel("Weight Component:", this);
    weightLabel->setFont(QFont("System", 10));
    weightLabel->setStyleSheet("color: white");
    mainLayout->addWidget(weightLabel);
    m_weightLineEdit->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';");
    mainLayout->addWidget(m_weightLineEdit);

    QLabel *totalScoreLabel = new QLabel("Total Score:", this);
    totalScoreLabel->setFont(QFont("System", 10));
    totalScoreLabel->setStyleSheet("color: white");
    mainLayout->addWidget(totalScoreLabel);
    m_totalScoreSpinBox->setStyleSheet("border: 1px solid black; color: black; background-color: #F8F6E3; font: 10pt 'System';");
    m_totalScoreSpinBox->setRange(0.0, 200.0);
    mainLayout->addWidget(m_totalScoreSpinBox);

    // Add buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setStyleSheet("color: black; background-color: #FFBF78; font: 10pt 'System';");
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

void TaskEditDialog::convertToUpperCaseEdit()
{
    m_courseLineEdit->setText(m_courseLineEdit->text().toUpper());
    m_weightLineEdit->setText(m_weightLineEdit->text().toUpper());
}

void TaskAddDialog::convertToUpperCaseAdd()
{
    m_courseLineEdit->setText(m_courseLineEdit->text().toUpper());
    m_weightLineEdit->setText(m_weightLineEdit->text().toUpper());
}


