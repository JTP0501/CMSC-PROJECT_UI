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
                int min_score = std::stoi(conversion.substr(pos1 + 1, pos2 - pos1 - 1));
                int max_score = std::stoi(conversion.substr(pos2 + 1));
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

        // Parse the year range
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

Console::Console(QWidget *parent) : QWidget(parent)
{
    // Create a vertical layout for the console widget
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Create a label for the console
    QLabel* consoleLabel = new QLabel("Console", this);
    consoleLabel->setStyleSheet("font-size: 15pt;");
    layout->addWidget(consoleLabel);

    // Create a text edit area for console output
    m_consoleOutput = new QTextEdit(this);
    m_consoleOutput->setReadOnly(true); // Set to read-only
    layout->addWidget(m_consoleOutput);

    // Create a line edit for console input
    m_consoleInput = new QLineEdit(this);
    // Connect the returnPressed signal of the line edit to the onSendCommand slot
    connect(m_consoleInput, &QLineEdit::returnPressed, this, &Console::onSendCommand);
    layout->addWidget(m_consoleInput);

    // Call the function to create the subject_files folder and CSV file
    createSubjectFiles();
}

void Console::onSendCommand()
{
    QString command = m_consoleInput->text().trimmed(); // Get the trimmed text from the line edit
    if (!command.isEmpty())
    {
        // Append user command to console output
        m_consoleOutput->append("User: " + command);
        // Process the command here
        processCommand(command);
    }
    m_consoleInput->clear();
}

void Console::processCommand(const QString &command)
{
    if (command.startsWith("add$ "))
    {
        QStringList parts = command.mid(5).split(",");
        if (parts.size() == 6)
        {
            QString csvLine = parts.join(",") + "\n";
            QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            QString csvFilePath = desktopPath + "/theta_files/subject_files/subjects.csv";

            QFile csvFile(csvFilePath);
            if (csvFile.open(QIODevice::Append | QIODevice::Text))
            {
                QTextStream out(&csvFile);
                out << csvLine;
                csvFile.close();
                m_consoleOutput->append("Added subject to CSV: " + csvLine.trimmed());
            }
            else
            {
                m_consoleOutput->append("Failed to open file for writing: " + csvFilePath);
            }
        }
        else
        {
            m_consoleOutput->append("Invalid command format. Expected format: add$ Year,Semester,Name,Units,WeightComponents,GradeConversions");
        }
    }
    else if (command.startsWith("rm$ "))
    {
        QString subjectName = command.mid(4).trimmed(); // Adjusted to remove "rm$ "
        if (!subjectName.isEmpty())
        {
            QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
            QString csvFilePath = desktopPath + "/theta_files/subject_files/subjects.csv";

            QFile csvFile(csvFilePath);
            if (csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QStringList lines;
                QTextStream in(&csvFile);
                bool found = false;

                while (!in.atEnd())
                {
                    QString line = in.readLine();
                    QStringList fields = line.split(',');
                    if (fields.size() >= 3 && fields[2].trimmed() != subjectName) // Ensure that subject name matches exactly
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
                        m_consoleOutput->append("Removed subject: " + subjectName);
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
    else if (command == "pf")
    {
        QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString csvFilePath = desktopPath + "/theta_files/subject_files/subjects.csv";

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
                        subjectInfo += "  " + QString::number(std::get<0>(conversion)) + " : " +
                                       QString::number(std::get<1>(conversion)) + " - " +
                                       QString::number(std::get<2>(conversion)) + "\n";
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
    else if (command == "clear")
    {
        // Clear the console
        m_consoleOutput->clear();
    }
    else
    {
        // Unknown Command
        m_consoleOutput->append("Unknown command. Please use 'add$ ' to add a subject, 'remove$ ' to remove a subject, 'parse_file' to parse the file, or 'clear' to clear the console.");
    }
}

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

