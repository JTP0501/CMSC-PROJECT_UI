#include "console.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QTextStream>
#include <QDebug> // Include QDebug for debug output
#include <iostream> // Include for std::cerr and std::endl
#include <fstream> // Include for file operations
#include <sstream> // Include for string stream operations
#include <QApplication>
#include <QFontDatabase>
#include <QLabel>

// Function to split a string by a delimiter
std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

// Structure to represent a subject
struct Subject
{
    std::string year;
    std::string semester;
    std::string name;
    int units;
    std::vector<std::pair<std::string, double>> weights;
    std::vector<std::tuple<double, int, int>> grade_conversions;
};

// Function to parse weight components
void parse_weights(const std::string &weight_str, Subject &subject)
{
    std::vector<std::string> weights = split(weight_str, '|');
    for (const auto &weight : weights)
    {
        size_t pos = weight.find_first_of('-');
        if (pos != std::string::npos)
        {
            std::string name = weight.substr(0, pos);
            double weight_value = std::stod(weight.substr(pos + 1));
            subject.weights.push_back({name, weight_value});
        }
    }
}

// Function to parse grade conversions
void parse_grade_conversions(const std::string &conversion_str, Subject &subject)
{
    std::vector<std::string> conversions = split(conversion_str, '|');
    for (const auto &conversion : conversions)
    {
        size_t pos1 = conversion.find_first_of(':');
        size_t pos2 = conversion.find_first_of('-');
        if (pos1 != std::string::npos && pos2 != std::string::npos)
        {
            try
            {
                double grade = std::stod(conversion.substr(0, pos1));
                double min_score = std::stod(conversion.substr(pos1 + 1, pos2 - pos1 - 1));
                double max_score = std::stod(conversion.substr(pos2 + 1));
                subject.grade_conversions.push_back({grade, min_score, max_score});
            }
            catch (const std::invalid_argument &e)
            {
                std::cerr << "Invalid grade conversion value: " << conversion << std::endl;
            }
        }
    }
}

// Function to parse the CSV file and return a vector of subjects
std::vector<Subject> parse_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<Subject> subjects;
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return subjects;
    }

    std::string line;
    getline(file, line); // Skip header line
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        // Split the line into fields
        std::vector<std::string> fields = split(line, ',');

        // Check if the line has the correct number of fields
        if (fields.size() != 6)
        {
            std::cerr << "Invalid format in line: " << line << std::endl;
            continue;
        }

        // Split the year range into start and end years
        std::vector<std::string> year_range = split(fields[0], '-');
        if (year_range.size() != 2)
        {
            std::cerr << "Invalid year range format in line: " << line << std::endl;
            continue;
        }

        // Create a subject object
        Subject subject;
        subject.year = fields[0]; // Store the original year range
        subject.semester = fields[1];
        subject.name = fields[2];
        subject.units = std::stoi(fields[3]);

        // Parse weight components
        parse_weights(fields[4], subject);

        // Parse grade conversions
        parse_grade_conversions(fields[5], subject);

        subjects.push_back(subject);
    }

    file.close();
    return subjects;
}

// Constructor for the Console class
Console::Console(QWidget *parent) : QWidget(parent)
{
    // Load fonts
    QFontDatabase::addApplicationFont(":/Fonts/8514fix.fon");
    QFontDatabase::addApplicationFont(":/Fonts/VT323-Regular.ttf");


    // Create a vertical layout for the console widget
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Create a label for the console
    QLabel* consoleLabel = new QLabel("Console", this);
    consoleLabel->setStyleSheet("font-family: 'VT323'; font-size: 25pt; font-weight: bold; color: black;");
    layout->addWidget(consoleLabel);

    // Create a text edit area for console output
    m_consoleOutput = new QTextEdit(this);
    m_consoleOutput->setStyleSheet("border: 1px solid black; background-color: #322C2B; color: white;");
    m_consoleOutput->setReadOnly(true); // Set to read-only
    m_consoleOutput->setFont(QFont("Fixedsys", 13));
    layout->addWidget(m_consoleOutput);

    // Create a line edit for console input
    m_consoleInput = new QLineEdit(this);
    m_consoleInput->setStyleSheet("border: 1px solid black; background-color: #322C2B; color: white;");
    m_consoleInput->setFont(QFont("Fixedsys", 13));
    // Connect the returnPressed signal of the line edit to the onSendCommand slot
    connect(m_consoleInput, &QLineEdit::returnPressed, this, &Console::onSendCommand);
    layout->addWidget(m_consoleInput);

    // Call the function to create the subject_files folder and CSV file
    createSubjectFiles();

    m_consoleOutput->append("--------------------------------------------------------\n\n");
    m_consoleOutput->append("add$ <Y,S,N,U,W,G> - Adds a subject to the system\n");
    m_consoleOutput->append("e.g. 2023-2024,1st,CMSC 11,3,MACHINE PROBLEMS-40|QUIZZES-10|LONG EXAMS-30|FINAL PROJECT-20,1.00:96-100|1.25:91-95|1.50:86-90|1.75:81-85|2.00:76-80|2.25:72-75|2.50:68-71|2.75:64-67|3.00:60-63|5.00:0-59\n");
    m_consoleOutput->append("rm$ <subject_name> - Removes a subject from the system\n");
    m_consoleOutput->append("parse - Parses all the subjects and displays in console\n");
    m_consoleOutput->append("parse$ <subject_name> - Adds a subject to the system\n");
    m_consoleOutput->append("clear - Clears the console.\n");
    m_consoleOutput->append("tasks$ <subject_name> - Displays all the tasks taken in of that subject.\n");
    m_consoleOutput->append("grades$ <subject_name> - Displays the grade breakdown of that subject.\n");
    m_consoleOutput->append("--------------------------------------------------------\n");
}

// Slot to handle command input from the user
void Console::onSendCommand()
{
    QString command = m_consoleInput->text().trimmed(); // Get the trimmed text from the line edit
    if (!command.isEmpty())
    {
        // Append user command to console output with grey color
        m_consoleOutput->append("<font color=\"grey\">User: " + command + "</font>");
        // Process the command here
        processCommand(command);
    }
    m_consoleInput->clear();
}

// Function to process user commands
void Console::processCommand(const QString &command)
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString csvFilePath = desktopPath + "/theta_files/subject_files/subjects.csv";

    // Process different commands based on their first character
    switch (command.at(0).toLatin1()) // Using toLatin1() to convert QChar to char
    {
    case 'a':
    {
        // Command to add a new subject
        if (command.startsWith("add$ "))
        {
            QStringList parts = command.mid(5).split(",");
            if (parts.size() == 6)
            {
                QString name = parts[2].trimmed(); // Get the name of the subject

                // Check if the subject already exists in the CSV file
                QFile checkFile(csvFilePath);
                if (checkFile.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream in(&checkFile);
                    in.readLine(); // Skip the header line
                    while (!in.atEnd())
                    {
                        QString line = in.readLine();
                        QStringList fields = line.split(",");
                        if (fields.size() >= 3 && fields[2].trimmed().compare(name, Qt::CaseInsensitive) == 0)
                        {
                            m_consoleOutput->append("Subject already exists: " + name);
                            checkFile.close();
                            return; // Exit the function without adding the subject
                        }
                    }
                    checkFile.close();
                }

                // Subject doesn't exist, proceed to add it
                QString year = parts[0];
                QString semester = parts[1];
                QString units = parts[3];
                QString weightComponents = parts[4];
                QString gradeConversions = parts[5];

                QString csvLine = year + "," + semester + "," + name + "," + units + "," + weightComponents + "," + gradeConversions + "\n";

                qDebug() << "CSV Line:" << csvLine;
                qDebug() << "CSV File Path:" << csvFilePath;

                QFile csvFile(csvFilePath);
                if (csvFile.open(QIODevice::Append | QIODevice::Text))
                {
                    QTextStream out(&csvFile);
                    out << csvLine;
                    csvFile.close();
                    m_consoleOutput->append("Added subject to CSV: " + csvLine.trimmed());

                    // Create a folder for the subject
                    QString subjectFolder = desktopPath + "/theta_files/subject_files/" + name;
                    qDebug() << "Subject Folder Path:" << subjectFolder;

                    if (!QDir().exists(subjectFolder))
                    {
                        if (QDir().mkpath(subjectFolder))
                        {
                            // Create a file for the list of tasks
                            QString tasksFilePath = subjectFolder + "/" + name + "_tasks.txt";
                            qDebug() << "Tasks File Path:" << tasksFilePath;

                            QFile tasksFile(tasksFilePath);
                            if (tasksFile.open(QIODevice::WriteOnly | QIODevice::Text))
                            {
                                QTextStream taskOut(&tasksFile);
                                taskOut << "List of Tasks for " + name + ":\n";
                                tasksFile.close();
                            }
                            else
                            {
                                m_consoleOutput->append("Failed to create tasks file for subject: " + name);
                            }

                            // Create a file for the grade breakdown
                            QString gradesFilePath = subjectFolder + "/" + name + "_grades.txt";
                            qDebug() << "Grades File Path:" << gradesFilePath;

                            QFile gradesFile(gradesFilePath);
                            if (gradesFile.open(QIODevice::WriteOnly | QIODevice::Text))
                            {
                                QTextStream gradesOut(&gradesFile);
                                gradesOut << "Grade Breakdown for " + name + ":\n";
                                for (const auto &weight : parts[4].split('|'))
                                {
                                    gradesOut << weight.split('-')[0] << ": 0\n"; // Initial grade set to 0
                                }
                                gradesFile.close();
                            }
                            else
                            {
                                m_consoleOutput->append("Failed to create grades file for subject: " + name);
                            }

                            // Create a file for weight components and their values, followed by grade conversions
                            QString weightGradeFilePath = subjectFolder + "/" + name + "_weight_grade.txt";
                            qDebug() << "Weight and Grade File Path:" << weightGradeFilePath;

                            QFile weightGradeFile(weightGradeFilePath);
                            if (weightGradeFile.open(QIODevice::WriteOnly | QIODevice::Text))
                            {
                                QTextStream weightGradeOut(&weightGradeFile);
                                weightGradeOut << "Weight Components and Values:\n";
                                weightGradeOut << parts[4] << "\n"; // Write weight components and their values
                                weightGradeOut << "Grade Conversions:\n";
                                weightGradeOut << parts[5] << "\n"; // Write grade conversions
                                weightGradeFile.close();
                            }
                            else
                            {
                                m_consoleOutput->append("Failed to create weight and grade file for subject: " + name);
                            }
                        }
                        else
                        {
                            m_consoleOutput->append("Failed to create folder for subject: " + name);
                        }
                    }
                    else
                    {
                        m_consoleOutput->append("Folder already exists for subject: " + name);
                    }
                }
                else
                {
                    m_consoleOutput->append("Failed to open CSV file for writing: " + csvFilePath);
                }
            }
            else
            {
                m_consoleOutput->append("Invalid command format. Expected format: add$ year,semester,name,units,weight_components,grade_conversions");
            }
        }
        break;
    }
    case 'r':
    {
        // Command to remove a subject
        if (command.startsWith("rm$ "))
        {
            QString subjectName = command.mid(4).trimmed(); // Adjusted to remove "rm$ "
            if (!subjectName.isEmpty())
            {
                QFile csvFile(csvFilePath);
                if (csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream in(&csvFile);
                    QStringList lines;
                    bool found = false;
                    while (!in.atEnd())
                    {
                        QString line = in.readLine();
                        if (!line.contains(subjectName, Qt::CaseInsensitive))
                        {
                            lines.append(line);
                        }
                        else
                        {
                            found = true;
                        }
                    }
                    csvFile.close();

                    if (found)
                    {
                        if (csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
                        {
                            QTextStream out(&csvFile);
                            for (const QString &line : lines)
                            {
                                out << line << "\n";
                            }
                            csvFile.close();
                            m_consoleOutput->append("Removed Subject: " + subjectName);

                            // Remove the subject folder
                            QString subjectFolder = desktopPath + "/theta_files/subject_files/" + subjectName;
                            qDebug() << "Subject Folder Path for Removal:" << subjectFolder;
                            QDir dir(subjectFolder);
                            if (dir.exists()) {
                                // Remove the directory and its contents recursively
                                if (dir.removeRecursively()) {
                                    qDebug() << "Subject folder removed successfully.";
                                } else {
                                    qDebug() << "Failed to remove subject folder.";
                                }
                            }
                        }
                        else
                        {
                            m_consoleOutput->append("Failed to open file for writing: " + csvFilePath);
                        }
                    }
                    else
                    {
                        m_consoleOutput->append("Subject not found: " + subjectName);
                    }
                }
                else
                {
                    m_consoleOutput->append("Failed to open file for reading: " + csvFilePath);
                }
            }
            else
            {
                m_consoleOutput->append("Invalid command format. Expected format: rm$ Name");
            }
        }
        break;
    }

    case 'p':
    {
        // Command to print all subjects from the file
        if (command == "parse")
        {
            QFile csvFile(csvFilePath);
            if (csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&csvFile);
                QString firstLine = in.readLine(); // Read the header
                if (in.atEnd() || in.readLine().isEmpty()) // Check if there's only the header or if there's no additional content
                {
                    m_consoleOutput->append("File is empty.");
                }
                else
                {
                    // Parse the CSV file and display subjects
                    std::string csvFilePathStr = csvFilePath.toStdString();
                    std::vector<Subject> subjects = parse_file(csvFilePathStr);
                    for (const auto &subject : subjects)
                    {
                        // Display subject information in the console
                        QString subjectInfo = "Year: " + QString::fromStdString(subject.year) + "\n" +
                                              "Semester: " + QString::fromStdString(subject.semester) + "\n" +
                                              "Subject: " + QString::fromStdString(subject.name) + "\n" +
                                              "Units: " + QString::number(subject.units) + "\n" +
                                              "Weights:\n";

                        for (const auto &weight : subject.weights)
                        {
                            subjectInfo += "  " + QString::fromStdString(weight.first) + " - " + QString::number(weight.second) + "%\n";
                        }

                        subjectInfo += "Grade Conversions:\n";

                        for (const auto &conversion : subject.grade_conversions)
                        {
                            subjectInfo += "  " + QString::number(std::get<0>(conversion), 'f', 2) + " : " +
                                           QString::number(std::get<1>(conversion), 'f', 2) + " - " +
                                           QString::number(std::get<2>(conversion), 'f', 2) + "\n";
                        }

                        m_consoleOutput->append(subjectInfo);
                    }
                }
                csvFile.close();
            }
            else
            {
                m_consoleOutput->append("Failed to open file for reading: " + csvFilePath);
            }
        }
        // Command to parse a specific subject
        else if (command.startsWith("parse$ "))
        {
            QString subjectName = command.mid(7).trimmed(); // Get the subject name
            bool subjectFound = false;

            QFile csvFile(csvFilePath);
            if (csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&csvFile);
                QString firstLine = in.readLine(); // Read the header

                while (!in.atEnd())
                {
                    QString line = in.readLine();
                    QStringList parts = line.split(",");
                    if (parts.size() == 6 && parts[2].trimmed().compare(subjectName, Qt::CaseInsensitive) == 0)
                    {
                        // Found the subject, parse and display it
                        Subject subject;
                        subject.year = parts[0].trimmed().toStdString();
                        subject.semester = parts[1].trimmed().toStdString();
                        subject.name = parts[2].trimmed().toStdString();
                        subject.units = parts[3].trimmed().toInt();
                        parse_weights(parts[4].trimmed().toStdString(), subject);
                        parse_grade_conversions(parts[5].trimmed().toStdString(), subject);

                        // Display subject information in the console
                        QString subjectInfo = "Year: " + QString::fromStdString(subject.year) + "\n" +
                                              "Semester: " + QString::fromStdString(subject.semester) + "\n" +
                                              "Subject: " + QString::fromStdString(subject.name) + "\n" +
                                              "Units: " + QString::number(subject.units) + "\n" +
                                              "Weights:\n";

                        for (const auto &weight : subject.weights)
                        {
                            subjectInfo += "  " + QString::fromStdString(weight.first) + " - " + QString::number(weight.second) + "%\n";
                        }

                        subjectInfo += "Grade Conversions:\n";

                        for (const auto &conversion : subject.grade_conversions)
                        {
                            subjectInfo += "  " + QString::number(std::get<0>(conversion), 'f', 2) + " : " +
                                           QString::number(std::get<1>(conversion), 'f', 2) + " - " +
                                           QString::number(std::get<2>(conversion), 'f', 2) + "\n";
                        }

                        m_consoleOutput->append(subjectInfo);
                        subjectFound = true;
                        break;
                    }
                }

                csvFile.close();

                if (!subjectFound)
                {
                    m_consoleOutput->append("Subject not found: " + subjectName);
                }
            }
            else
            {
                m_consoleOutput->append("Failed to open file for reading: " + csvFilePath);
            }
        }
        break;
    }

    case 'c':
    {
        // Command to clear the console
        if (command == "clear")
        {
            // Clear the console
            m_consoleOutput->clear();
        }
        break;
    }

    case 'g':
    {
        // Command to display grade breakdown of a specific subject
        if (command.startsWith("grades$ "))
        {
            QString subjectName = command.mid(7).trimmed(); // Get the subject name
            QString subjectFolder = desktopPath + "/theta_files/subject_files/" + subjectName;
            QString gradesFilePath = subjectFolder + "/" + subjectName + "_grades.txt";

            QFile gradesFile(gradesFilePath);
            if (gradesFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&gradesFile);
                QString gradesData = in.readAll();
                gradesFile.close();

                if (!gradesData.isEmpty())
                {
                    m_consoleOutput->append(gradesData);
                }
                else
                {
                    m_consoleOutput->append("Subject doesn't exist.");
                }
            }
            else
            {
                m_consoleOutput->append("Subject doesn't exist.");
            }
        }
        break;
    }

    case 't':
    {
        // Command to display list of tasks of a specific subject
        if (command.startsWith("tasks$ "))
        {
            QString subjectName = command.mid(6).trimmed(); // Get the subject name
            QString subjectFolder = desktopPath + "/theta_files/subject_files/" + subjectName;
            QString tasksFilePath = subjectFolder + "/" + subjectName + "_tasks.txt";

            QFile tasksFile(tasksFilePath);
            if (tasksFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&tasksFile);
                QString tasksData = in.readAll();
                tasksFile.close();

                if (tasksData.trimmed().split("\n").size() <= 1) // Check if tasksData is empty
                {
                    m_consoleOutput->append("List of tasks is empty."); // Print message if it's empty
                }
                else
                {
                    m_consoleOutput->append(tasksData);
                }
            }
            else
            {
                m_consoleOutput->append("Subject doesn't exist.");
            }
        }
        break;
    }

    default:
        // Unknown Command
        m_consoleOutput->append("Unknown command. Please use 'add$ ' to add a subject, 'remove$ ' to remove a subject, 'parse_file' to parse the file, or 'clear' to clear the console.");
        break;
    }
}

// Function to create the necessary subject files and folders
void Console::createSubjectFiles()
{
    qDebug() << "Starting createSubjectFiles()";

    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    qDebug() << "Desktop path:" << desktopPath;

    QString thetaFolderPath = desktopPath + "/theta_files";
    qDebug() << "Theta folder path:" << thetaFolderPath;

    QString subjectFolderPath = thetaFolderPath + "/subject_files";
    qDebug() << "Subject folder path:" << subjectFolderPath;

    // Create the theta_files folder if it doesn't exist
    if (!QDir().exists(thetaFolderPath))
    {
        qDebug() << "Theta folder does not exist. Creating...";
        if (QDir().mkpath(thetaFolderPath))
        {
            qDebug() << "Theta folder created successfully.";
        }
        else
        {
            qDebug() << "Failed to create theta folder.";
            m_consoleOutput->append("Failed to create folder: " + thetaFolderPath);
            return;
        }
    }
    else
    {
        qDebug() << "Theta folder already exists.";
    }

    // Create the subject_files folder if it doesn't exist
    if (!QDir().exists(subjectFolderPath))
    {
        qDebug() << "Subject folder does not exist. Creating...";
        if (QDir().mkpath(subjectFolderPath))
        {
            qDebug() << "Subject folder created successfully.";
        }
        else
        {
            qDebug() << "Failed to create subject folder.";
            m_consoleOutput->append("Failed to create folder: " + subjectFolderPath);
            return;
        }
    }
    else
    {
        qDebug() << "Subject folder already exists.";
    }

    // Path to the CSV file
    csvFilePath = subjectFolderPath + "/subjects.csv";
    qDebug() << "CSV file path:" << csvFilePath;

    // Check if the CSV file already exists
    if (!QFile::exists(csvFilePath))
    {
        qDebug() << "CSV file does not exist. Creating...";
        QFile csvFile(csvFilePath);
        if (csvFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&csvFile);
            // Write the header to the CSV file
            out << "Year,Semester,Name,Units,WeightComponents,GradeConversions\n";
            csvFile.close();
            qDebug() << "CSV file created and header written successfully.";
        }
        else
        {
            qDebug() << "Failed to create CSV file.";
            m_consoleOutput->append("Failed to create file: " + csvFilePath);
        }
    }
    else
    {
        qDebug() << "CSV file already exists.";
    }

    qDebug() << "Finished createSubjectFiles()";
}
