// customeventfilter.cpp
#include "customeventfilter.h"
#include <QDebug>
#include <QListView>

CustomEventFilter::CustomEventFilter(QListView* ongoingListView, QListView* waitlistedListView, QObject *parent)
    : QObject(parent), m_pwOngoing(ongoingListView), m_pwWaitlisted(waitlistedListView)
{
    // Constructor implementation
}

bool CustomEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::HoverEnter)
    {
        qDebug() << "HoverEnter event intercepted by event filter.";

        // Assuming 'obj' corresponds to the task being hovered over
        if (QListView* listView = qobject_cast<QListView*>(obj))
        {
            int completeStatus = 0; // Default complete status
            if (listView == m_pwWaitlisted)
            {
                qDebug() << "Task hovered over in Waitlisted view.";
                completeStatus = 1;
            }
            else if (listView == m_pwOngoing)
            {
                qDebug() << "Task hovered over in Ongoing view.";
            }
            else
            {
                // Unknown list view, do nothing
                return false;
            }

            // Get the task name from the model index
            QModelIndex index = listView->indexAt(listView->mapFromGlobal(QCursor::pos()));
            if (index.isValid())
            {
                QString taskName = index.data(Qt::DisplayRole).toString();

                // Set the task's completion status
                setTaskCompleteStatus(taskName, completeStatus);

                // Reload tasks based on the completion status
                loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(listView->model()), completeStatus);
            }
        }
    }

    // Call the base class implementation
    return QObject::eventFilter(obj, event);
}
