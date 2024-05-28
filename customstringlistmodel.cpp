#include "customstringlistmodel.h"
#include <QMimeData>

CustomStringListModel::CustomStringListModel(QObject* parent)
    : QStringListModel(parent)
{
}

Qt::ItemFlags CustomStringListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QStringListModel::flags(index);
    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions CustomStringListModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QStringList CustomStringListModel::mimeTypes() const
{
    return QStringList() << "text/plain";
}

QMimeData* CustomStringListModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QStringList texts;

    foreach (const QModelIndex &index, indexes)
    {
        if (index.isValid())
        {
            texts << data(index, Qt::DisplayRole).toString();
        }
    }

    mimeData->setText(texts.join("\n"));
    return mimeData;
}

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

    QStringList newItems = data->text().split(QLatin1Char('\n'), Qt::SkipEmptyParts);
    insertRows(row, newItems.count());

    for (const QString &item : newItems)
    {
        QModelIndex idx = index(row, 0);
        setData(idx, item);
        ++row;
    }

    return true;
}
