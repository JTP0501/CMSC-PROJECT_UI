#include "customlistview.h"
#include <QMimeData>
#include <QDropEvent>
#include <QStringListModel>

CustomListView::CustomListView(QWidget *parent) : QListView(parent) {
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}

void CustomListView::startDrag(Qt::DropActions supportedActions) {
    QModelIndexList indexes = selectedIndexes();
    if (indexes.size() != 1)
        return;

    QMimeData *mimeData = new QMimeData;
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    stream << indexes.at(0).row();
    mimeData->setData("application/x-qabstractitemmodeldatalist", encodedData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->exec(Qt::MoveAction);
}

void CustomListView::dropEvent(QDropEvent *event) {
    if (!event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
        return;

    QByteArray encodedData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    QStringListModel *model = qobject_cast<QStringListModel *>(this->model());
    if (!model)
        return;

    int row;
    if (event->source() == this && event->possibleActions() & Qt::MoveAction) {
        stream >> row;
    } else {
        QModelIndex index = indexAt(event->pos());
        row = index.isValid() ? index.row() : model->rowCount();
    }

    QString droppedText;
    stream >> droppedText;

    model->insertRow(row);
    model->setData(model->index(row), droppedText);
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

Qt::DropActions CustomListView::supportedDropActions() const {
    return Qt::MoveAction;
}

