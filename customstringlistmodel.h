#ifndef CUSTOMSTRINGLISTMODEL_H
#define CUSTOMSTRINGLISTMODEL_H

#include <QStringListModel> // Include QStringListModel for the base class
#include <QMimeData> // Include QMimeData for handling drag-and-drop operations
#include <QList> // Include QList for storing tasks

// Define a struct to represent a task
struct Task {
    QString taskName;
    QString semester;
    QString course;
    QString weight;
    double totalScore;
    double score;
    bool complete;
    int index;
};

class CustomStringListModel : public QStringListModel // Define CustomStringListModel class inheriting from QStringListModel
{
    Q_OBJECT // Add Q_OBJECT macro to enable signals and slots

public:
    explicit CustomStringListModel(QObject *parent = nullptr); // Constructor declaration

    // Overridden functions for drag-and-drop functionality
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    // Custom functions to interact with tasks
    void addTask(const Task& task); // Function to add a task to the model
    void removeTask(int row);
    Task getTask(int index) const; // Function to retrieve a task from the model

private:
    QList<Task> m_tasks; // List to store tasks
};

#endif // CUSTOMSTRINGLISTMODEL_H
