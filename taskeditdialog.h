#ifndef TASKEDITDIALOG_H
#define TASKEDITDIALOG_H

#include <QDialog>
#include <QLabel>
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
    void convertToUpperCaseEdit();

private slots:
    void saveTaskEdit();

private:
    void setupEditUI();

    QLineEdit *m_taskNameLineEdit;
    QLineEdit *m_courseLineEdit;
    QLineEdit *m_weightLineEdit;
    QDoubleSpinBox *m_totalScoreSpinBox;

};

class TaskEditScoreDialog : public QDialog
{
    Q_OBJECT

public:
    TaskEditScoreDialog(QWidget *parent = nullptr);

    void setTotalScore(double totalScore);
    double getScore() const;
    void setScore(double score);

private slots:
    void saveTaskEdit();

private:
    void setupScoreUI();

    QDoubleSpinBox *m_scoreSpinBox;
    QLabel *m_totalScoreLabel;  // QLabel to display total score
    double m_totalScore;
};

class TaskAddDialog : public QDialog
{
    Q_OBJECT

public:
    TaskAddDialog(QWidget *parent = nullptr);
    void setTaskAdd(const Task &task);
    Task getTaskAdd() const;
    void convertToUpperCaseAdd();

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
