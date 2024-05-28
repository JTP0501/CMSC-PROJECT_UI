#ifndef CUSTOMSTRINGLISTMODEL_H
#define CUSTOMSTRINGLISTMODEL_H

#include <QStringListModel>
#include <QMimeData>
#include <QList>

// Define a struct to represent a task
struct Task {
    QString taskName;
    QString semester;
    QString course;
    QString weight;
    double totalScore;
    double score;
    bool complete;
};

class CustomStringListModel : public QStringListModel
{
    Q_OBJECT

public:
    explicit CustomStringListModel(QObject *parent = nullptr);

    // Overridden functions
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    // Custom functions to interact with tasks
    void addTask(const Task &task);
    void removeTask(int index);
    Task getTask(int index) const;

private:
    QList<Task> m_tasks;
};

#endif // CUSTOMSTRINGLISTMODEL_H

