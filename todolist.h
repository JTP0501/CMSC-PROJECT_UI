#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>
#include <QAction>
#include <QEvent>
#include "console.h"
#include "customstringlistmodel.h"


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

protected slots:
    void onAdd();
    void onRemove();
    void onEdit();

private:
    QListView* m_pwOngoing = nullptr;
    QListView* m_pwWaitlisted = nullptr;

    Console* m_console;

    QAction* m_pActAdd = nullptr;
    QAction* m_pActRemove = nullptr;
    QAction* m_pActEdit = nullptr;
};

#endif // TODOLIST_H
