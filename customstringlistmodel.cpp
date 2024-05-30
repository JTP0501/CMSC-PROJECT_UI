#include "customstringlistmodel.h" // Include the header file for CustomStringListModel
#include <QMimeData> // Include QMimeData for handling MIME data

// Constructor
CustomStringListModel::CustomStringListModel(QObject* parent)
    : QStringListModel(parent)
{
}

// Override flags function to specify item flags
Qt::ItemFlags CustomStringListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStringListModel::flags(index); // Get default item flags
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags; // Enable drag and drop for valid items
    else
        return Qt::ItemIsDropEnabled | defaultFlags; // Enable drop only for empty areas
}

// Specify supported drop actions
Qt::DropActions CustomStringListModel::supportedDropActions() const
{
    return Qt::MoveAction; // Support only move action for drops
}

// Specify MIME types supported by the model
QStringList CustomStringListModel::mimeTypes() const
{
    return QStringList() << "text/plain"; // Support plain text MIME type
}

// Create MIME data for the given indexes
QMimeData* CustomStringListModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData(); // Create a new MIME data object
    QStringList texts;

    foreach (const QModelIndex &index, indexes)
    {
        if (index.isValid())
        {
            texts << data(index, Qt::DisplayRole).toString(); // Get the text data from the selected indexes
        }
    }

    mimeData->setText(texts.join("\n")); // Set the text data in MIME data object
    return mimeData;
}

// Handle dropped MIME data
bool CustomStringListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    if (!data || !data->hasText())
        return false;

    if (row == -1)
    {
        if (parent.isValid())
            row = parent.row();
        else
            row = rowCount();
    }

    QStringList newItems = data->text().split(QLatin1Char('\n'), Qt::SkipEmptyParts); // Split text data into items
    insertRows(row, newItems.count()); // Insert new rows for the dropped items

    for (const QString &item : newItems)
    {
        QModelIndex idx = index(row, 0); // Get index for the new row
        setData(idx, item); // Set the data for the new row
        ++row; // Move to the next row
    }

    return true;
}

void CustomStringListModel::addTask(const Task& task)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Task newTask = task;
    newTask.index = rowCount(); // Set index to the new position (starting from 0)
    m_tasks.append(newTask);
    endInsertRows();
}

void CustomStringListModel::removeTask(int row)
{
    if (row < 0 || row >= m_tasks.count()) return;

    beginRemoveRows(QModelIndex(), row, row);
    m_tasks.removeAt(row);
    endRemoveRows();

    // Update the indexes of remaining tasks
    for (int i = row; i < m_tasks.count(); ++i) {
        m_tasks[i].index = i; // Update index to match row (starting from 0)
    }
}