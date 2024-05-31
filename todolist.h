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
    bool complete;
    QString semester;
    QString year;
};

class CToDoList : public QMainWindow
{
    Q_OBJECT

public:
    CToDoList();
    void loadTasksFromFile(const QString& filePath, QStringListModel* model);
    void addTaskToFile(const Task& task, const QString& filePath);
    void loadTasksOnStartup();
    Task searchTaskByName(const QString& taskName, const QString& filePath);
    void removeTaskByName(const QString& taskName, const QString& filePath);

private slots:
    void onAdd();
    void onRemove();
    void onEdit();
    void onRefresh();

private:
    QListView* m_pwOngoing = nullptr;
    QListView* m_pwWaitlisted = nullptr;

    QAction* m_pActAdd;
    QAction* m_pActRemove;
    QAction* m_pActEdit;
    QAction* m_pActRefresh;

    Console* m_console;

    QString ongoingFilePath;
    QString waitlistedFilePath;

};

#endif // TODOLIST_H
