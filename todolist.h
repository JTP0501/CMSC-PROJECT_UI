//todolist.h

#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>
#include "console.h"

struct Task {
    QString taskName;
    QString course;
    QString weight;
    double totalScore;
    double score;
    int complete; // Changed bool to int to store completion status as 0 or 1

    Task() : totalScore(0.0), score(-1.0), complete(0) {}
};

class CToDoList : public QMainWindow
{
    Q_OBJECT

public:
    CToDoList(QWidget *parent = nullptr); // Constructor declaration with default argument
    void loadTasksFromFile(const QString& filePath, QStringListModel* model, bool completeStatus);
    void addTaskToFile(const Task& task, const QString& filePath);
    void loadTasksOnStartup();
    Task searchTaskByName(const QString& taskName, const QString& filePath);
    void removeTaskByName(const QString& taskName, const QString& filePath);
    QString verifyTaskName(const QString& taskName, const QString& filePath);
    void setTaskCompleteStatus(const QString& taskName, int completeStatus);
    void updateTaskInFile(const Task& task, const QString& filePath);
    QString getTaskNameAtCursor(QListView* listView);
    QString constructSubjectFilePath(const Task& task);
    bool writeTaskToSubjectFile(const Task& task, const QString& subjectFilePath);

private slots:
    void onAdd();
    void onRemove();
    void onEdit();
    void onRefresh();
    void onOngoingHovered();
    void onWaitlistedHovered();

private:
    QListView* m_pwOngoing = nullptr;
    QListView* m_pwWaitlisted = nullptr;

    QAction* m_pActAdd;
    QAction* m_pActRemove;
    QAction* m_pActEdit;
    QAction* m_pActRefresh;

    Console* m_console;

    QString tasksFilePath;
};

#endif // TODOLIST_H
