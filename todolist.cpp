#include "todolist.h"

#include <QLabel>
#include <QToolBar>
#include <QBoxLayout>
#include <QInputDialog>
#include "customstringlistmodel.h"
#include "taskeditdialog.h"

CToDoList::CToDoList()
{
    QWidget* pWidget = new QWidget(this);
    pWidget->setStyleSheet("background-color: #ECF0F1");
    setCentralWidget(pWidget);

    setWindowTitle("Theta(θ): Task Handling and Evaluation Tracking Application");

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pWidget->setLayout(pMainLayout);

    // Header layout to hold the title and make it center aligned
    QHBoxLayout* pHeaderLayout = new QHBoxLayout();
    pMainLayout->addLayout(pHeaderLayout);

    QLabel* pwTitle = new QLabel("To Do List", this);
    pHeaderLayout->addWidget(pwTitle, 0, Qt::AlignCenter);
    pwTitle->setAlignment(Qt::AlignCenter);
    pwTitle->setStyleSheet("font-size: 30pt; margin: 10%");

    QHBoxLayout* pHLayoutLabels = new QHBoxLayout();
    pMainLayout->addLayout(pHLayoutLabels);

    QLabel* plblOngoing = new QLabel("Ongoing", this);
    plblOngoing->setStyleSheet("font-size: 15pt;");
    pHLayoutLabels->addWidget(plblOngoing);

    QLabel* plblWaitlisted = new QLabel("Waitlisted", this);
    plblWaitlisted->setStyleSheet("font-size: 15pt;");
    pHLayoutLabels->addWidget(plblWaitlisted);

    // Main horizontal layout to hold the lists
    QHBoxLayout* pHLayout = new QHBoxLayout();
    pMainLayout->addLayout(pHLayout);

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

    // Use CustomStringListModel
    m_pwOngoing->setModel(new CustomStringListModel);
    m_pwWaitlisted->setModel(new CustomStringListModel);

    m_pwOngoing->setStyleSheet
        ("QListView { font-size: 15pt; font-weight: bold; }"
         "QListView::item { background-color: #E74C3C; padding: 5%;"
         "border: 1px solid #C0392B; border-radius: 10px; margin-top: 2.5px; margin-bottom: 2.5px; }"
         "QListView::item::hover { background-color: #D44233 }");

    m_pwWaitlisted->setStyleSheet
        ("QListView { font-size: 15pt; font-weight: bold; }"
         "QListView::item { background-color: #2ECC71; padding: 5%;"
         "border: 1px solid #27AE60; border-radius: 10px; margin-top: 2.5px; margin-bottom: 2.5px; }"
         "QListView::item::hover { background-color: #25A85D }");

    QToolBar* pToolBar = new QToolBar(this);
    addToolBar(pToolBar);

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
}

// SLOTS
void CToDoList::onAdd()
{
    m_pwOngoing->model()->insertRow(m_pwOngoing->model()->rowCount());
    QModelIndex oIndex = m_pwOngoing->model()->index(m_pwOngoing->model()->rowCount() - 1, 0);
    m_pwOngoing->edit(oIndex);
}

void CToDoList::onRemove()
{
    QModelIndex index = m_pwOngoing->currentIndex();
    if (index.isValid()) {
        m_pwOngoing->model()->removeRow(index.row());
    }
}

void CToDoList::onEdit()
{
    QModelIndex index = m_pwOngoing->currentIndex();
    if (index.isValid())
    {
        // Get the current task's data
        QString taskName = m_pwOngoing->model()->data(index, Qt::DisplayRole).toString();
        Task currentTask = m_pwOngoing->model()->data(index, Qt::UserRole).value<Task>();

        // Create and set up the edit dialog
        TaskEditDialog editDialog(this); // $
        editDialog.setTask(currentTask); // $

        // Connect the dialog's taskEdited signal to a slot that updates the task in the model
        connect(&editDialog, &TaskEditDialog::taskEdited, this, [=](const Task &updatedTask) { // $
            // Update the model data with the modified task
            m_pwOngoing->model()->setData(index, updatedTask.taskName, Qt::DisplayRole);
            m_pwOngoing->model()->setData(index, QVariant::fromValue(updatedTask), Qt::UserRole);
        });

        // Show the edit dialog
        editDialog.exec();
    }
}


