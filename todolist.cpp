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

    // Load ongoing and waitlisted tasks from files
    loadTasks(ongoingFilePath, m_pwOngoing);
    loadTasks(waitlistedFilePath, m_pwWaitlisted);
}

// Slot to add a new task
void CToDoList::onAdd()
{
    // Insert a new row into the ongoing list view
    m_pwOngoing->model()->insertRow(m_pwOngoing->model()->rowCount());
    // Get the index of the newly inserted row
    QModelIndex oIndex = m_pwOngoing->model()->index(m_pwOngoing->model()->rowCount() - 1, 0);
    // Start editing the new item
    m_pwOngoing->edit(oIndex);

    // Create a new task and populate it with default values
    Task newTask;
    // Get the task data from the newly added row
    newTask.taskName = m_pwOngoing->model()->data(oIndex, Qt::DisplayRole).toString();
    newTask.semester = "..."; // Default value, will be verified with another file (not implemented yet)
    newTask.course = "..."; // To be inputted by the user in onEdit function
    newTask.weight = "..."; // To be inputted by the user in onEdit function
    newTask.totalScore = 0; // To be inputted by the user in onEdit function
    newTask.score = -1; // Default value, indicates no score yet
    newTask.complete = false; // Default value, indicates that user hasn't completed the task yet
    newTask.index = oIndex.row(); // Index tracker for user in the file (storage)
    // Append the task data to the ongoing tasks file
    appendTasks(ongoingFilePath, newTask);
}

// Slot to remove a task
void CToDoList::onRemove()
{
    QModelIndex index = m_pwOngoing->currentIndex();
    if (index.isValid()) {
        m_pwOngoing->model()->removeRow(index.row());
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

        // Create and set up the edit dialog
        TaskEditDialog editDialog(this);
        editDialog.setTask(currentTask);

        // Connect the dialog's taskEdited signal to a slot that updates the task in the model
        connect(&editDialog, &TaskEditDialog::taskEdited, this, [=](const Task &updatedTask) {
            // Update the model data with the modified task
            m_pwOngoing->model()->setData(index, QVariant::fromValue(updatedTask), Qt::UserRole);
        });

        // Show the edit dialog
        int result = editDialog.exec();
        if (result == QDialog::Accepted) {
            // Update the task data in the model if the user accepted the changes
            Task updatedTask = editDialog.getTask();
            editDialog.setTask(updatedTask);
            m_pwOngoing->model()->setData(index, QVariant::fromValue(updatedTask), Qt::UserRole);
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

void CToDoList::readTasks(const QString& fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);

        // Read task data from the file
        Task loadedTask;
        in >> loadedTask.taskName >> loadedTask.semester >> loadedTask.course >> loadedTask.weight >> loadedTask.totalScore >> loadedTask.score >> loadedTask.complete >> loadedTask.index;

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
            // Insert loaded task into the list view's model
            insertTaskIntoModel(loadedTask, listView);
        }

        file.close();
    }
}

void CToDoList::insertTaskIntoModel(const Task& task, QListView* listView)
{
    // Create a new model if one doesn't exist
    if (!listView->model())
        listView->setModel(new CustomStringListModel);

    // Insert the task into the model
    CustomStringListModel* model = qobject_cast<CustomStringListModel*>(listView->model());
    if (model)
        model->addTask(task);
}
/*
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
*/
