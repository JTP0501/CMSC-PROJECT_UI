#include "todolist.h"
#include "taskeditdialog.h"

#include <QLabel>
#include <QToolBar>
#include <QBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QHoverEvent>
#include <QMimeData>


CToDoList::CToDoList(QWidget *parent)
    : QMainWindow(parent), tasksFilePath("") // Initialize tasksFilePath in the constructor initialization list
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

    // File paths for tasks (within the theta_files folder)
    tasksFilePath = folderPath + "tasks_file.bin";

    qDebug() << "Theta Files Folder Path:" << folderPath;

    if (!QDir().exists(folderPath))
    {
        // Create the folder if it doesn't exist
        QDir().mkpath(folderPath);
    }

    // Check if the files exist
    if (!QFile::exists(tasksFilePath))
    {
        // Create the file if it doesn't exist
        QFile file(tasksFilePath);
        if (file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Created file:" << tasksFilePath;
            file.close();
        }
        else
        {
            qDebug() << "Failed to create file:" << tasksFilePath;
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

    // Set the models for the QListView instances
    m_pwOngoing->setModel(new QStringListModel());
    m_pwWaitlisted->setModel(new QStringListModel());

    // Set the edit triggers for the list views to prevent editing
    m_pwOngoing->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pwWaitlisted->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Connect doubleClicked signal for both views to onEdit slot
    connect(m_pwOngoing, &QListView::doubleClicked, this, &CToDoList::onEdit);
    connect(m_pwWaitlisted, &QListView::doubleClicked, this, &CToDoList::onEdit);

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

    m_pActRefresh = new QAction(this);
    m_pActRefresh->setIcon(QIcon(":/4refresh_btn.png"));
    connect(m_pActRefresh, &QAction::triggered, this, &CToDoList::onRefresh);

    pToolBar->addAction(m_pActAdd);
    pToolBar->addAction(m_pActRemove);
    pToolBar->addAction(m_pActEdit);
    pToolBar->addAction(m_pActRefresh);

    // Add Console
    m_console = new Console(this);
    pMainLayout->addWidget(m_console);

    loadTasksOnStartup();

    connect(m_pwOngoing, &QListView::entered, this, &CToDoList::onOngoingHovered);
    connect(m_pwWaitlisted, &QListView::entered, this, &CToDoList::onWaitlistedHovered);
}

// TASK MANAGEMENT OPERATIONS

// Slot to add a task
void CToDoList::onAdd()
{
    // Prompt the user to add a task
    TaskAddDialog addDialog(this);
    addDialog.setWindowTitle("Add Task");

    // Executing the dialog and checking if the user clicked "OK"
    if (addDialog.exec() == QDialog::Accepted)
    {
        // Getting the task details from the dialog
        Task newTask = addDialog.getTaskAdd();

        // Verify the task name to ensure it's unique
        QString verifiedTaskName = verifyTaskName(newTask.taskName, tasksFilePath);
        newTask.taskName = verifiedTaskName;

        // Set complete to 0 for new tasks
        newTask.complete = 0;
        newTask.score = -1; // Set default score value to -1

        // Adding the task name to the ongoing list view
        static_cast<QStringListModel*>(m_pwOngoing->model())->insertRow(m_pwOngoing->model()->rowCount());
        QModelIndex newIndex = m_pwOngoing->model()->index(m_pwOngoing->model()->rowCount() - 1, 0);
        static_cast<QStringListModel*>(m_pwOngoing->model())->setData(newIndex, newTask.taskName);

        // Add Task to File
        addTaskToFile(newTask,tasksFilePath);
    }
}

// Slot to remove a task
void CToDoList::onRemove()
{
    QModelIndex index = m_pwOngoing->currentIndex();
    if (index.isValid())
    {
        QString taskName = index.data(Qt::DisplayRole).toString();

        // Remove the task from the file
        removeTaskByName(taskName, tasksFilePath);

        // Reload the tasks from the file
        loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(m_pwOngoing->model()), 1);
        loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(m_pwOngoing->model()), 0);
    }
}

// Slot to edit a task
void CToDoList::onEdit()
{
    QModelIndex index = m_pwOngoing->currentIndex();
    if (!index.isValid()) {
        index = m_pwWaitlisted->currentIndex();
    }

    if (index.isValid()) {
        QString taskName = index.data(Qt::DisplayRole).toString();

        // Search for the task by name in the ongoing tasks file
        Task currentTask = searchTaskByName(taskName, tasksFilePath);
        if (currentTask.taskName.isEmpty()) {
            qDebug() << "Task not found.";
            return;
        }

        if (currentTask.complete) {
            // Show the TaskEditScoreDialog if the task is waitlisted
            TaskEditScoreDialog editScoreDialog(this);
            editScoreDialog.setTotalScore(currentTask.totalScore);
            editScoreDialog.setScore(currentTask.score);

            if (editScoreDialog.exec() == QDialog::Accepted) {
                // Get the edited score from the dialog
                double editedScore = editScoreDialog.getScore();
                currentTask.score = editedScore;

                // Update the task in the file
                updateTaskInFile(currentTask, tasksFilePath);

                // If score is greater than or equal to 0, move the task to the subject folder and file
                if (currentTask.score >= 0) {
                    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
                    QString subjectFolderPath = desktopPath + "/theta_files/subject_files/" + currentTask.course;

                    // Check if the subject folder exists
                    if (!QDir(subjectFolderPath).exists()) {
                        // Subject folder doesn't exist
                        qDebug() << "Subject folder does not exist:" << subjectFolderPath;
                        return;
                    }

                    QString subjectFilePath = subjectFolderPath + "/" + currentTask.course + "_tasks.txt";
                    QFile subjectFile(subjectFilePath);
                    if (subjectFile.open(QIODevice::Append | QIODevice::Text))
                    {
                        QTextStream subjectOut(&subjectFile);

                        // Format: Task Name - Weight - Score / Total Score
                        QString formattedTask = QString("%1 - %2 - %3 / %4")
                                                    .arg(currentTask.taskName)
                                                    .arg(currentTask.weight)
                                                    .arg(currentTask.score)
                                                    .arg(currentTask.totalScore);

                        subjectOut << formattedTask << "\n";
                        subjectFile.close();

                        // Remove the task from the original file
                        removeTaskByName(currentTask.taskName, tasksFilePath);
                        loadTasksOnStartup();

                        // After navigating to the subject folder
                        QString weightGradeFilePath = subjectFolderPath + "/" + currentTask.course + "_weight_grade.txt";
                        QString subjectTasksFilePath = subjectFolderPath + "/" + currentTask.course + "_tasks.txt";

                        // Debug statements to check file paths
                        qDebug() << "Weight Grade File Path:" << weightGradeFilePath;
                        qDebug() << "Tasks File Path:" << subjectTasksFilePath;

                        // Open the tasks file for reading
                        QFile tasksFile(subjectTasksFilePath);
                        if (tasksFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                            QTextStream tasksStream(&tasksFile);

                            // Map to store weight component names and their corresponding scores
                            QMap<QString, double> componentScores;
                            QMap<QString, int> componentTaskCount;

                            // Read tasks file to calculate scores and count tasks per component
                            bool headerSkipped = false;
                            while (!tasksStream.atEnd()) {
                                QString line = tasksStream.readLine();
                                // Skip the header line
                                if (!headerSkipped) {
                                    headerSkipped = true;
                                    continue;
                                }

                                qDebug() << "Task Line:" << line;
                                QStringList taskDetails = line.split(" - ", Qt::SkipEmptyParts);

                                qDebug() << "Task Details:" << taskDetails;

                                if (taskDetails.size() == 3) {
                                    QString componentName = taskDetails[1].trimmed();
                                    QStringList scoreDetails = taskDetails[2].split(" / ", Qt::SkipEmptyParts);

                                    if (scoreDetails.size() == 2) {
                                        double taskScore = scoreDetails[0].toDouble();
                                        double totalScore = scoreDetails[1].toDouble();

                                        qDebug() << "Component Name:" << componentName;
                                        qDebug() << "Task Score:" << taskScore;
                                        qDebug() << "Total Score:" << totalScore;

                                        double ratio = (totalScore != 0) ? (taskScore / totalScore) : 0.0;
                                        qDebug() << "Ratio:" << ratio;

                                        componentScores[componentName] += ratio;
                                        componentTaskCount[componentName]++;
                                    }
                                }
                            }

                            tasksFile.close();

                            // Debug statements to check calculated values
                            qDebug() << "Component Scores:" << componentScores;
                            qDebug() << "Component Task Count:" << componentTaskCount;

                            // Calculate the weighted grade for each component
                            QMap<QString, double> weightedGrades;
                            for (auto it = componentScores.begin(); it != componentScores.end(); ++it) {
                                QString componentName = it.key();
                                double totalWeightedGrade = it.value();

                                // Get the weight value for the component from the weight grades file
                                double weightValue = 0.0;

                                QFile weightGradeFile(weightGradeFilePath);
                                if (weightGradeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                                    QTextStream weightGradeStream(&weightGradeFile);

                                    // Skip the header line
                                    QString header = weightGradeStream.readLine();

                                    // Read weight grades file to find the weight value for the component
                                    while (!weightGradeStream.atEnd()) {
                                        QString line = weightGradeStream.readLine();

                                        // Skip the header line
                                        if (line.startsWith("Weight Components and Values:")) {
                                            continue;
                                        }

                                        int index = line.indexOf(componentName);
                                        if (index != -1) {
                                            // Find the hyphen next to the componentName
                                            int hyphenIndex = line.indexOf('-', index);
                                            if (hyphenIndex != -1) {
                                                // Find the end of the value
                                                int endIndex = line.indexOf('|', hyphenIndex);
                                                if (endIndex == -1) {
                                                    endIndex = line.length();
                                                }
                                                // Extract the value substring and convert it to double
                                                QString valueString = line.mid(hyphenIndex + 1, endIndex - hyphenIndex - 1).trimmed();
                                                weightValue = valueString.toDouble();
                                                qDebug() << "Weight value for" << componentName << ":" << weightValue;
                                                break;
                                            }
                                        }
                                    }

                                    weightGradeFile.close();
                                } else {
                                    qDebug() << "Failed to open weight grade file for reading:" << weightGradeFilePath;
                                }

                                // Calculate the weighted grade for the component
                                double weightedGrade = (weightValue != 0 && componentTaskCount[componentName] != 0) ?
                                                           totalWeightedGrade * (weightValue / componentTaskCount[componentName]) : 0.0;

                                // Store the weighted grade for the component
                                weightedGrades[componentName] = weightedGrade;
                            }

                            // Write the weighted grades to the subject_grades.txt file
                            QString subjectGradesFilePath = subjectFolderPath + "/" + currentTask.course + "_grades.txt";
                            QFile subjectGradesFile(subjectGradesFilePath);
                            if (subjectGradesFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                                QTextStream subjectGradesStream(&subjectGradesFile);

                                // Write the updated grade breakdown to the subject_grades.txt file
                                subjectGradesStream << "Grade Breakdown for " << currentTask.course << ":\n";
                                for (auto it = weightedGrades.begin(); it != weightedGrades.end(); ++it) {
                                    subjectGradesStream << it.key() << ": " << it.value() << "\n";
                                }

                                // Close the subject_grades.txt file
                                subjectGradesFile.close();
                            } else {
                                qDebug() << "Failed to open subject grades file for writing:" << subjectGradesFilePath;
                            }
                        } else {
                            qDebug() << "Failed to open tasks file for reading:" << subjectTasksFilePath;
                        }
                    } else {
                        qDebug() << "Failed to open subject file for writing:" << subjectFilePath;
                    }
                }
            }
        } else {
            // Existing logic for editing tasks that are not waitlisted
            TaskEditDialog editDialog(this);
            editDialog.setWindowTitle("Edit Task");
            editDialog.setTaskEdit(currentTask);

            if (editDialog.exec() == QDialog::Accepted) {
                // Get the edited task from the dialog
                Task editedTask = editDialog.getTaskEdit();

                if (taskName != editedTask.taskName) {
                    QString verifiedTaskName = verifyTaskName(editedTask.taskName, tasksFilePath);
                    editedTask.taskName = verifiedTaskName;
                }

                if (!editedTask.taskName.isEmpty() && !editedTask.course.isEmpty() &&
                    editedTask.totalScore >= 0 && !editedTask.weight.isEmpty()) {
                    static_cast<QStringListModel*>(m_pwOngoing->model())->setData(index, editedTask.taskName);

                    removeTaskByName(taskName, tasksFilePath);
                    addTaskToFile(editedTask, tasksFilePath);

                    loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(m_pwOngoing->model()), 1);
                    loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(m_pwOngoing->model()), 0);
                } else {
                    qDebug() << "Invalid task data. Changes not saved.";
                }
            }
        }
    }
}

// Slot to refresh list of tasks (for accidental replacements)
void CToDoList::onRefresh()
{
    // Reload ongoing tasks (completeStatus = 0)
    loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(m_pwOngoing->model()), 0);

    // Reload waitlisted tasks (completeStatus = 1)
    loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(m_pwWaitlisted->model()), 1);

    // Debugging message
    qDebug() << "Tasks reloaded from files and complete status updated.";
}

// FILE OPERATIONS

void CToDoList::loadTasksFromFile(const QString& filePath, QStringListModel* model, bool completeStatus)
{
    QFile taskFile(filePath);
    if (taskFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&taskFile);
        QStringList tasks;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList taskDetails = line.split(",", Qt::SkipEmptyParts);
            if (!taskDetails.isEmpty() && (taskDetails.size() >= 5 && taskDetails[4].toInt() == (completeStatus ? 1 : 0)))
            {
                QString taskName = taskDetails.first();
                tasks.append(taskName);
            }
        }
        taskFile.close();
        model->setStringList(tasks);
    }
    else
    {
        qDebug() << "Failed to open file for reading:" << filePath;
    }
}

void CToDoList::loadTasksOnStartup()
{
    // Load ongoing tasks (completeStatus = 0)
    loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(m_pwOngoing->model()), 0);

    // Load waitlisted tasks (completeStatus = 1)
    loadTasksFromFile(tasksFilePath, static_cast<QStringListModel*>(m_pwWaitlisted->model()), 1);

    // Debugging message
    qDebug() << "Tasks loaded on startup.";
}

void CToDoList::addTaskToFile(const Task &task, const QString &filePath)
{
    // Open the file in append mode
    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text))
    {
        // Create a QTextStream to write to the file
        QTextStream out(&file);

        // Write the task details to the file in CSV format, including the completion status as an integer
        out << task.taskName << ","
            << task.course << ","
            << task.weight << ","
            << task.totalScore << ","
            << (task.complete ? 1 : 0) << ","
            << task.score << "\n";

        // Close the file
        file.close();
    }
    else
    {
        qDebug() << "Failed to open file for writing:" << filePath;
    }
}

// Utility function to search for a task by name in a file
Task CToDoList::searchTaskByName(const QString& taskName, const QString& filePath)
{
    QFile taskFile(filePath);
    if (taskFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&taskFile);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            qDebug() << "Read line from file:" << line; // Debugging statement
            if (!line.trimmed().isEmpty()) // Process non-empty lines
            {
                QStringList taskDetails = line.split(",", Qt::SkipEmptyParts);
                qDebug() << "Split line into parts:" << taskDetails; // Debugging statement

                // Add this line for debugging
                qDebug() << "taskDetails size:" << taskDetails.size();

                if (taskDetails.size() >= 6 && taskDetails.first() == taskName)
                {
                    Task foundTask;
                    foundTask.taskName = taskDetails.at(0);
                    foundTask.course = taskDetails.at(1);
                    foundTask.weight = taskDetails.at(2);
                    foundTask.totalScore = taskDetails.at(3).toDouble();
                    foundTask.complete = taskDetails.at(4).toInt();
                    foundTask.score = taskDetails.at(5).toDouble();
                    taskFile.close();
                    return foundTask;
                }
            }
        }
        taskFile.close();
    }
    // Return an empty task if not found
    return Task();
}

// Utility function to remove a task by name from a file
void CToDoList::removeTaskByName(const QString& taskName, const QString& filePath)
{
    QFile inputFile(filePath);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QFile outputFile("tempfile");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream in(&inputFile);
            QTextStream out(&outputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if (!line.trimmed().isEmpty()) // Process non-empty lines
                {
                    QStringList taskDetails = line.split(",", Qt::SkipEmptyParts);
                    if (!taskDetails.isEmpty() && taskDetails.first() != taskName)
                    {
                        out << line << '\n';
                    }
                }
            }
            inputFile.close();
            outputFile.close();
            inputFile.remove();
            outputFile.rename(filePath);
        }
    }
}

// Utility function to verify the task name has no duplicates
QString CToDoList::verifyTaskName(const QString& taskName, const QString& filePath)
{
    // Open the file in read mode
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString newTaskName = taskName;
        int symbolCount = 1;

        // Check if the task name already exists in the file
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList taskDetails = line.split(",", Qt::SkipEmptyParts);
            if (!taskDetails.isEmpty() && taskDetails.first() == newTaskName)
            {
                // Append a number in parentheses to differentiate the task name
                newTaskName = taskName + " (" + QString::number(symbolCount++) + ")";
            }
        }

        // Close the file
        file.close();

        // Return the verified task name
        return newTaskName;
    }
    else
    {
        qDebug() << "Failed to open file for reading:" << filePath;
        return taskName; // Return original task name if file cannot be opened
    }
}

void CToDoList::updateTaskInFile(const Task& task, const QString& filePath)
{
    QFile inputFile(filePath);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QFile outputFile("tempfile");
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream in(&inputFile);
            QTextStream out(&outputFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();
                if (!line.trimmed().isEmpty()) // Process non-empty lines
                {
                    QStringList taskDetails = line.split(",", Qt::SkipEmptyParts);
                    if (!taskDetails.isEmpty() && taskDetails.first() == task.taskName)
                    {
                        // Update the complete status
                        taskDetails.replace(4, task.complete ? "1" : "0");
                        // Update the score
                        taskDetails.replace(5, QString::number(task.score));
                        line = taskDetails.join(",");
                    }
                    out << line << '\n';
                }
            }
            inputFile.close();
            outputFile.close();
            inputFile.remove();
            outputFile.rename(filePath);
        }
    }
}

// TASK STATUS MANAGEMENT

void CToDoList::setTaskCompleteStatus(const QString& taskName, int completeStatus)
{
    // Search for the task by name in the appropriate file
    QString filePath = tasksFilePath;
    Task task = searchTaskByName(taskName, filePath);
    if (!task.taskName.isEmpty())
    {
        // Update the task's complete status
        task.complete = completeStatus;
        qDebug() << "Completion Status: " << task.complete << '\n';
        // Rewrite the task to the file
        updateTaskInFile(task, filePath);
    }
    else
    {
        qDebug() << "Task not found in file: " << taskName;
    }
}

void CToDoList::onWaitlistedHovered()
{
    qDebug() << "Hovered over waitlisted tasks.";

    // Get the task name from the index at the cursor position
    QString taskName = getTaskNameAtCursor(m_pwWaitlisted);

    // Search for the task by name and update its complete status to 1
    setTaskCompleteStatus(taskName, 1);

}

void CToDoList::onOngoingHovered()
{
    qDebug() << "Hovered over ongoing tasks.";

    // Get the task name from the index at the cursor position
    QString taskName = getTaskNameAtCursor(m_pwOngoing);

    // Search for the task by name and update its complete status to 0
    setTaskCompleteStatus(taskName, 0);
}

QString CToDoList::getTaskNameAtCursor(QListView* listView)
{
    QModelIndex index = listView->indexAt(listView->mapFromGlobal(QCursor::pos()));
    if (index.isValid())
    {
        // Get the task name from the model index
        return index.data(Qt::DisplayRole).toString();
    }
    return QString();
}
