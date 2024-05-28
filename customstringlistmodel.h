#ifndef CUSTOMSTRINGLISTMODEL_H
#define CUSTOMSTRINGLISTMODEL_H

#include <QStringListModel>

class CustomStringListModel : public QStringListModel
{
    Q_OBJECT

public:
    explicit CustomStringListModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
};

#endif // CUSTOMSTRINGLISTMODEL_H
