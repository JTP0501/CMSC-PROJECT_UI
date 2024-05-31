#include "customstringlistmodel.h"
#include <QMimeData>

CustomStringListModel::CustomStringListModel(QObject* parent)
    : QStringListModel(parent)
{
}

Qt::ItemFlags CustomStringListModel::flags(const QModelIndex &index) const
{
    // Default item flags with additional drag and drop support
    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions CustomStringListModel::supportedDropActions() const
{
    return Qt::MoveAction; // Support move action for drag and drop
}

QStringList CustomStringListModel::mimeTypes() const
{
    return QStringList() << "text/plain"; // Support plain text mime type
}

QMimeData* CustomStringListModel::mimeData(const QModelIndexList &indexes) const
{
    // Create mime data from selected indexes
    QMimeData *mimeData = new QMimeData();
    QStringList texts;

    for (const QModelIndex &index : indexes)
    {
        if (index.isValid())
            texts << data(index, Qt::DisplayRole).toString();
    }

    mimeData->setText(texts.join("\n"));
    return mimeData;
}

bool CustomStringListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction || !data || !data->hasText())
        return false;

    if (row == -1)
        row = rowCount();

    // Split the dropped text into new items
    QStringList newItems = data->text().split('\n', Qt::SkipEmptyParts);
    insertRows(row, newItems.count(), parent);

    // Insert the new items into the model
    for (const QString &item : newItems)
    {
        setData(index(row, 0, parent), item);
        ++row;
    }

    return true;
}

void CustomStringListModel::addTask(const Task& task)
{
    // Begin inserting rows into the model
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Append the task to the list of tasks
    m_tasks.append(task);

    // End inserting rows into the model
    endInsertRows();
}


void CustomStringListModel::removeTask(int row)
{
    // Check if the row is valid
    if (row < 0 || row >= m_tasks.count())
        return;

    // Begin removing rows from the model
    beginRemoveRows(QModelIndex(), row, row);

    // Remove the task from the list
    m_tasks.removeAt(row);

    // End removing rows from the model
    endRemoveRows();
}
