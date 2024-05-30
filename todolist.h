#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QListView>
#include <QAction>
#include <QString>
#include "console.h" // Ensure you have the correct path to Console class

// Forward declaration of Task
struct Task;

// CToDoList class definition
class CToDoList : public QMainWindow
{
    Q_OBJECT

public:
    CToDoList();

private slots:
    void onAdd();
    void onRemove();
    void onEdit();

private:
    void appendTasks(const QString& fileName, const Task& task);
    void loadTasks(const QString& fileName, QListView* listView);
    void insertTaskIntoModel(const Task& task, QListView* listView);
    void clearTasks(const QString& fileName);
    void updateTaskIndexes(const QString& fileName);

    QListView* m_pwOngoing; // List view for ongoing tasks
    QListView* m_pwWaitlisted; // List view for waitlisted tasks

    QAction* m_pActAdd; // Action to add tasks
    QAction* m_pActRemove; // Action to remove tasks
    QAction* m_pActEdit; // Action to edit tasks

    Console* m_console; // Console widget

    QString ongoingFilePath; // Path to the ongoing tasks file
    QString waitlistedFilePath; // Path to the waitlisted tasks file
};

#endif // TODOLIST_H
