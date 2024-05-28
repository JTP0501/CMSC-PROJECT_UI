#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow> // Include necessary Qt header files
#include <QListView>
#include <QStringListModel>
#include <QAction>
#include <QEvent>
#include "console.h" // Include header files for Console and CustomStringListModel
#include "customstringlistmodel.h"

// Forward declaration of Ui namespace
QT_BEGIN_NAMESPACE
namespace Ui {
class ToDoList;
}
QT_END_NAMESPACE

class CToDoList : public QMainWindow // Define CToDoList class inheriting from QMainWindow
{
    Q_OBJECT // Add Q_OBJECT macro to enable signals and slots

public:
    CToDoList(); // Constructor declaration

protected slots: // Declare slots for handling events
    void onAdd(); // Slot to handle adding an item
    void onRemove(); // Slot to handle removing an item
    void onEdit(); // Slot to handle editing an item

private:
    QListView* m_pwOngoing = nullptr; // Pointer to the ongoing task list view
    QListView* m_pwWaitlisted = nullptr; // Pointer to the waitlisted task list view

    Console* m_console; // Pointer to the console widget

    QAction* m_pActAdd = nullptr; // Pointer to the action for adding a task
    QAction* m_pActRemove = nullptr; // Pointer to the action for removing a task
    QAction* m_pActEdit = nullptr; // Pointer to the action for editing a task
};

#endif // TODOLIST_H
