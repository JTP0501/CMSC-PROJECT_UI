#include "todolist.h"

#include <QLabel>
#include <QToolBar>
#include <QBoxLayout>
#include <QInputDialog>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "customstringlistmodel.h"
#include "taskeditdialog.h"

CToDoList::CToDoList()
{
    // Create the main widget
    QWidget* pWidget = new QWidget(this);
    pWidget->setStyleSheet("background-color: #ECF0F1");
    setCentralWidget(pWidget);

    // Set window title
    setWindowTitle("Theta(θ): Task Handling and Evaluation Tracking Application");

    // Create the main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pWidget->setLayout(pMainLayout);

    // Header layout to hold the title and make it center aligned
    QHBoxLayout* pHeaderLayout = new QHBoxLayout();
    pMainLayout->addLayout(pHeaderLayout);

    // Title label
    QLabel* pwTitle = new QLabel("To Do List", this);
    pHeaderLayout->addWidget(pwTitle, 0, Qt::AlignCenter);
    pwTitle->setAlignment(Qt::AlignCenter);
    pwTitle->setStyleSheet("font-size: 30pt; margin: 10%");

    // Horizontal layout for labels
    QHBoxLayout* pHLayoutLabels = new QHBoxLayout();
    pMainLayout->addLayout(pHLayoutLabels);

    // Labels for ongoing and waitlisted tasks
    QLabel* plblOngoing = new QLabel("Ongoing", this);
    plblOngoing->setStyleSheet("font-size: 15pt;");
    pHLayoutLabels->addWidget(plblOngoing);

    QLabel* plblWaitlisted = new QLabel("Waitlisted", this);
    plblWaitlisted->setStyleSheet("font-size: 15pt;");
    pHLayoutLabels->addWidget(plblWaitlisted);

    // Main horizontal layout to hold the lists
    QHBoxLayout* pHLayout = new QHBoxLayout();
    pMainLayout->addLayout(pHLayout);

    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation); // Get desktop path
    QString folderName = "theta_files"; // Define folder name
    QString folderPath = desktopPath + "/" + folderName + "/"; // Define folder path

    // Check if folder exists
    if (!QDir().exists(folderPath))
    {
        // Create the folder if it doesn't exist
        QDir().mkpath(folderPath);
    }

    // File paths for ongoing and waitlisted tasks (within the theta_files folder)
    ongoingFilePath = folderPath + "ongoing_tasks.bin";
    waitlistedFilePath = folderPath + "waitlisted_tasks.bin";

    qDebug() << "Theta Files Folder Path:" << folderPath;

    // Check if the files exist
    if (!QFile::exists(ongoingFilePath))
    {
        // Create the file if it doesn't exist
        QFile file(ongoingFilePath);
        if (file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Created file:" << ongoingFilePath;
            file.close();
        }
        else
        {
            qDebug() << "Failed to create file:" << ongoingFilePath;
        }
    }

    if (!QFile::exists(waitlistedFilePath))
    {
        // Create the file if it doesn't exist
        QFile file(waitlistedFilePath);
        if (file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Created file:" << waitlistedFilePath;
            file.close();
        }
        else
        {
            qDebug() << "Failed to create file:" << waitlistedFilePath;
        }
    }

    // List views for ongoing and waitlisted tasks
    m_pwOngoing = new QListView(this);
    m_pwOngoing->setDragEnabled(true);
    m_pwOngoing->setAcceptDrops(true);
    m_pwOngoing->setDropIndicatorShown(true);
    m_pwOngoing->setDefaultDropAction(Qt::MoveAction);
    pHLayout->addWidget(m_pwOngoing);

    m_pwWaitlisted = new QListView(this);
    m_pwWaitlisted->setDragEnabled(true);
    m_pwWaitlisted->setAcceptDrops(true);
    m_pwWaitlisted->setDropIndicatorShown(true);
    m_pwWaitlisted->setDefaultDropAction(Qt::MoveAction);
    pHLayout->addWidget(m_pwWaitlisted);

    // Use CustomStringListModel for both list views
    m_pwOngoing->setModel(new CustomStringListModel);
    m_pwWaitlisted->setModel(new CustomStringListModel);

    // Set stylesheets for list views
    m_pwOngoing->setStyleSheet(
        "QListView { font-size: 15pt; font-weight: bold; }"
        "QListView::item { background-color: #E74C3C; padding: 5%;"
        "border: 1px solid #C0392B; border-radius: 10px; margin-top: 2.5px; margin-bottom: 2.5px; }"
        "QListView::item::hover { background-color: #D44233 }");

    m_pwWaitlisted->setStyleSheet(
        "QListView { font-size: 15pt; font-weight: bold; }"
        "QListView::item { background-color: #2ECC71; padding: 5%;"
        "border: 1px solid #27AE60; border-radius: 10px; margin-top: 2.5px; margin-bottom: 2.5px; }"
        "QListView::item::hover { background-color: #25A85D }");

    // Create a toolbar
    QToolBar* pToolBar = new QToolBar(this);
    addToolBar(pToolBar);

    // Add actions to the toolbar for add, remove, and edit operations
    m_pActAdd = new QAction(this);
    m_pActAdd->setIcon(QIcon(":/1add_btn.png"));
    connect(m_pActAdd, &QAction::triggered, this, &CToDoList::onAdd);

    m_pActRemove = new QAction(this);
    m_pActRemove->setIcon(QIcon(":/2trash_btn.png"));
    connect(m_pActRemove, &QAction::triggered, this, &CToDoList::onRemove);

    m_pActEdit = new QAction(this);
    m_pActEdit->setIcon(QIcon(":/3edit_btn.png"));
    connect(m_pActEdit, &QAction::triggered, this, &CToDoList::onEdit);

    pToolBar->addAction(m_pActAdd);
    pToolBar->addAction(m_pActRemove);
    pToolBar->addAction(m_pActEdit);

    // Add Console
    m_console = new Console(this);
    pMainLayout->addWidget(m_console);

    // Clear ongoing tasks file
    // clearTasks(ongoingFilePath);

    // Load ongoing and waitlisted tasks from files
    loadTasks(ongoingFilePath, m_pwOngoing);
    loadTasks(waitlistedFilePath, m_pwWaitlisted);

}

// Slot to add a new task
void CToDoList::onAdd()
{
    // Prompt the user for the task name
    QString taskName = QInputDialog::getText(this, "Add Task", "Enter task name:");

    // Check if the user entered a valid task name
    if (!taskName.isEmpty()) {
        // Retrieve the model from the ongoing list view
        CustomStringListModel* model = qobject_cast<CustomStringListModel*>(m_pwOngoing->model());
        if (model) {
            // Create a new task and populate it with default values
            Task newTask;
            newTask.taskName = taskName;
            newTask.semester = "...";
            newTask.course = "...";
            newTask.weight = "...";
            newTask.totalScore = 0;
            newTask.score = -1;
            newTask.complete = false;
            newTask.index = model->rowCount(); // Set index to current row count (starting from 0)

            // Add the task to the model
            model->addTask(newTask);

            // Append the task data to the ongoing tasks file
            qDebug() << "I'm appending";
            appendTasks(ongoingFilePath, newTask);
        }
    }
}

// Slot to remove a task
void CToDoList::onRemove()
{
    QModelIndex index = m_pwOngoing->currentIndex();
    if (index.isValid()) {
        // Retrieve the model from the ongoing list view
        CustomStringListModel* model = qobject_cast<CustomStringListModel*>(m_pwOngoing->model());
        if (model) {
            // Remove the task from the model
            model->removeTask(index.row());

            // Update indexes in the file
            updateTaskIndexes(ongoingFilePath);
        }
    }
}

// Slot to edit a task
void CToDoList::onEdit()
{
    QModelIndex index = m_pwOngoing->currentIndex();
    if (index.isValid())
    {
        // Get the current task's data
        QString taskName = m_pwOngoing->model()->data(index, Qt::DisplayRole).toString();
        Task currentTask = m_pwOngoing->model()->data(index, Qt::UserRole).value<Task>();

        // Open a dialog to edit the task
        TaskEditDialog editDialog(this);
        editDialog.setTask(currentTask);

        // Execute the dialog
        int result = editDialog.exec();
        if (result == QDialog::Accepted) {
            // Get the edited task data from the dialog
            Task editedTask = editDialog.getTask();

            // Update the task name if it has been changed
            if (editedTask.taskName != taskName) {
                // Remove the old task with the original name
                // Add the edited task with the new name
            }

            // Save the edited task back to your data structure or file
            // Update the task with the edited values
            // m_pwOngoing->model()->setData(index, QVariant::fromValue(editedTask), Qt::UserRole);
        }
    }
}

void CToDoList::appendTasks(const QString& fileName, const Task& task)
{
    QFile file(fileName);
    if (file.open(QIODevice::Append)) { // Open file in Append mode to add new tasks
        QDataStream out(&file);

        // Write task data to the file
        out << task.taskName << task.semester << task.course << task.weight << task.totalScore << task.score << task.complete << task.index;

        file.close();
    }
}

void CToDoList::loadTasks(const QString& fileName, QListView* listView)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);

        // Read and populate tasks until the end of file
        while (!in.atEnd()) {
            Task loadedTask;
            in >> loadedTask.taskName >> loadedTask.semester >> loadedTask.course >> loadedTask.weight >> loadedTask.totalScore >> loadedTask.score >> loadedTask.complete >> loadedTask.index;

            // Check if the loaded task name is empty
            if (!loadedTask.taskName.isEmpty()) {
                // Insert loaded task into the list view's model
                insertTaskIntoModel(loadedTask, listView);
            } else {
                qDebug() << "Empty task name encountered while loading tasks from file:" << fileName;
            }
        }

        file.close();
    }
}

void CToDoList::insertTaskIntoModel(const Task& task, QListView* listView)
{
    // Check if the list view has a model set
    if (!listView->model())
    {
        // If no model is set, create a new CustomStringListModel and set it as the model for the list view
        CustomStringListModel* model = new CustomStringListModel(listView);
        listView->setModel(model);
    }

    // Retrieve the model from the list view
    CustomStringListModel* model = qobject_cast<CustomStringListModel*>(listView->model());

    // Add the task to the model
    if (model)
        model->addTask(task);
}

void CToDoList::clearTasks(const QString& fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        // Truncate the file to clear its content
        file.close();
        qDebug() << "Cleared tasks in file:" << fileName;
    } else {
        qDebug() << "Failed to clear tasks in file:" << fileName;
    }
}

void CToDoList::updateTaskIndexes(const QString& fileName)
{
    CustomStringListModel* model = qobject_cast<CustomStringListModel*>(m_pwOngoing->model());
    if (!model) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QDataStream out(&file);

        // Update the index of each task based on its position in the model
        for (int row = 0; row < model->rowCount(); ++row) {
            QModelIndex index = model->index(row, 0);
            Task task = model->data(index, Qt::UserRole).value<Task>();
            task.index = row; // Update index to current row (starting from 0)

            // Write updated task data to the file
            out << task.taskName << task.semester << task.course << task.weight << task.totalScore << task.score << task.complete << task.index;
        }

        file.close();
    }
}

/*
Task CToDoList::findTaskByName(const QString& fileName, const QString& taskName){
    Task foundTask;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);

        // Read and compare task names until the end of file
        while (!in.atEnd()) {
            Task loadedTask;
            in >> loadedTask.taskName >> loadedTask.semester >> loadedTask.course >> loadedTask.weight >> loadedTask.totalScore >> loadedTask.score >> loadedTask.complete >> loadedTask.index;

            // If the task name matches, assign it to foundTask and break the loop
            if (loadedTask.taskName == taskName) {
                foundTask = loadedTask;
                break;
            }
        }

        file.close();
    }
    return foundTask;
}

*/