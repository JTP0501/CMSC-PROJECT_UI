#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>
#include <QAction>
#include <QEvent>
#include "console.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ToDoList;
}
QT_END_NAMESPACE

class CToDoList : public QMainWindow
{
    Q_OBJECT

public:
    CToDoList();

<<<<<<< Updated upstream
protected slots:
    void onAdd();
    void onRemove();
    void onEdit();
=======
protected slots: // Declare slots for handling events
    void onAdd(); // Slot to handle adding an item
    void onRemove(); // Slot to handle removing an item
    void onEdit(); // Slot to handle editing an item
    void appendTasks(const QString& fileName, const Task& task);
    void loadTasks(const QString& fileName, QListView* listView);
    void insertTaskIntoModel(const Task& task, QListView* listView);
    void clearTasks(const QString& fileName);
    void updateTaskIndexes(const QString& fileName);
    //Task findTaskByName(const QString& fileName, const QString& taskName);
>>>>>>> Stashed changes

private:
    QListView* m_pwOngoing = nullptr;
    QListView* m_pwWaitlisted = nullptr;
    QStringListModel* m_taskModelOngoing = nullptr;
    QStringListModel* m_taskModelWaitlisted = nullptr;

    Console* m_console;

    QAction* m_pActAdd = nullptr;
    QAction* m_pActRemove = nullptr;
    QAction* m_pActEdit = nullptr;

};


#endif // TODOLIST_H
