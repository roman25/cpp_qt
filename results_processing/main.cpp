#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QString>

using namespace std;

// Create struct for storing statistic
struct circuitState
{
    string pinName;
    string pinState;
};

// Set required test cases
const string openCase   = "Open";
const string shortCase  = "Short";
const string okCase     = "Ok";

// Set parameters to parse and store data
const string fileName               = "_cases_statistic";
const string outputFileExtension    = ".txt"; // .csv and .txt are allowed
const string inputFileExtension     = ".txt";
string delimiter                    = "\t";
size_t delimeterPosition            = 0;

void createTemplate (string directoryPath, vector< pair <string, string> > *pinsState)
{
    // Function defines template with pin's state



    /* [RK] - I assume the template should be created another way, isn't it?
     It may be predefined .txt for example */



    // Set full path to a file
    string filePath = directoryPath + to_string(1) + inputFileExtension;

    // Create input stream
    ifstream readData(filePath);

    // Check if file is exists
    if (readData.is_open())
    {
        // Variable to store input data
        string inputData;

        // Read a row from input
        while(getline(readData, inputData))
        {
            delimeterPosition = inputData.find(delimiter);

            std::size_t foundOpenCase   = inputData.find(openCase);
            std::size_t foundShortCase  = inputData.find(shortCase);
            std::size_t foundOkCase  = inputData.find(okCase);
            string pinName = inputData.substr(0, delimeterPosition);

            // Skip unused rows
            if (pinName != "" && pinName != "////MKM4//// ")
            {
                string currentState;

                if (foundOpenCase!=std::string::npos)
                    currentState = openCase;

                if (foundShortCase!=std::string::npos)
                    currentState = shortCase;

                if (foundOkCase!=std::string::npos)
                    currentState = okCase;

                (*pinsState).push_back(pair <string, string> (pinName, currentState));
            }
        }
    readData.close();

    }
}

void collectStatistic(vector <circuitState> vectState, vector< pair <string, string> > *pinsState, vector <int> *counterOpen, vector <int> *counterShort, vector <int> *counterOk)
{
    // Collects statistis of input data

    // Iterate for every value in input data
    for (int i= 0; i < vectState.size(); i++)
    {
        // Get values from input data
        string pinName          = vectState[i].pinName;
        string currentState     = vectState[i].pinState;

        // Get values from template
        string tempName         = (*pinsState)[i].first;
        string tempState        = (*pinsState)[i].second;

        // Compare current pin name and name from template
        if (pinName == tempName)
        {
            // Choose state of a pin
            if (currentState == openCase && currentState != tempState)
                ((*counterOpen)[i])++;

            if (currentState == shortCase && currentState != tempState)
                ((*counterShort)[i])++;

            if (currentState == okCase && currentState != tempState)
                ((*counterOk)[i])++;
         }
      }
}

QString writeStatistic(string directoryPath, vector <pair <string, string> > *pinsState, vector <int> *counterOpen, vector <int> *counterShort, vector <int> *counterOk)
{
    /* Function stores and shows statistic */

    // Create output stream
    ofstream outfile;
    string header;
    string delimeter;

    // Choose delimeter for output file
    if (outputFileExtension == ".txt")
        delimeter = "\t";
    else if (outputFileExtension == ".csv")
        delimeter = ",";

    // Set header for output file
    header = "Pin name" + delimeter + shortCase + delimeter + openCase + delimeter + okCase + "\n";

    // Forming output file name and creating it
    string fullOutFileName = directoryPath + fileName + outputFileExtension;
    outfile.open(fullOutFileName);

    // Write data
    outfile << header;

    string strToWrite = "";
    for (int i = 0; i < (*pinsState).size(); i++)
    {
        // Form output row with statistic
        strToWrite += ((*pinsState)[i].first + delimeter + to_string((*counterShort)[i]) + delimeter + to_string((*counterOpen)[i]) + delimeter + to_string((*counterOk)[i]) + "\n");

    }
    outfile << strToWrite;
    outfile.close();

    strToWrite = header + strToWrite;
    QString returnedString = QString::fromStdString(strToWrite);
    return returnedString;
}

vector <circuitState> convertTextToStruct(string filePath, bool *fileExists)
{
    // Temporary function converts input text data to structure

    circuitState strCircuitState;
    vector <circuitState> vectState;

    // Create input stream
    ifstream readData(filePath);

    // Check if file is exists
    if (readData.is_open())
    {
        // Variable to store input data
        string inputData;

        // Read a row from input
        while(getline(readData, inputData))
        {
            delimeterPosition = inputData.find(delimiter);

            std::size_t foundOpenCase   = inputData.find(openCase);
            std::size_t foundShortCase  = inputData.find(shortCase);
            std::size_t foundOkCase  = inputData.find(okCase);
            string pinName = inputData.substr(0, delimeterPosition);

            // Skip unused rows
            if (pinName != "" && pinName != "////MKM4//// ")
            {
                string pinsState;

                if (foundOpenCase!=std::string::npos)
                    pinsState = openCase;

                if (foundShortCase!=std::string::npos)
                    pinsState = shortCase;

                if (foundOkCase!=std::string::npos)
                    pinsState = okCase;

                // Collect data as vector
                strCircuitState.pinName = pinName;
                strCircuitState.pinState = pinsState;
                vectState.push_back(strCircuitState);
            }
        }
        readData.close();
    }

    else
        (*fileExists) = false;

    return vectState;
}

int main(int argc, char *argv[])
{  
    // Path to txt
    const string directoryPath = "D:\\projects\\res_OpenShortTest\\";

    // Define vector to collect required state of a pin
    vector <pair <string, string> > pinsState;
    createTemplate (directoryPath, &pinsState);

    // Set size of output vector
    int vectorSize = pinsState.size();
    vector <int> counterOpen(vectorSize, 0);
    vector <int> counterShort(vectorSize, 0);
    vector <int> counterOk(vectorSize, 0);

    // Define variable that helps form full file name
    int counter = 2;

    // Bool variable to read undefined count of files
    bool fileExists = true;

    // Base cycle to read input file
    while (fileExists)
    {
        // Set full path to a file
        string filePath = directoryPath + to_string(counter) + inputFileExtension;
        vector <circuitState> vectState = convertTextToStruct(filePath, &fileExists);
        collectStatistic(vectState, &pinsState, &counterOpen, &counterShort, &counterOk);

        // Increment to the nex file name
        counter++;
    }

    QString dataText = writeStatistic(directoryPath, &pinsState, &counterOpen, &counterShort, &counterOk);

    // Create GUI to show results
    QApplication application(argc, argv);
    QMainWindow mainWindow;

    // Set windows size
    int winWidth    = 250;
    int winHeight   = 700;
    mainWindow.setFixedSize(winWidth,winHeight);

    // Create text label and set its size
    QLabel *label = new QLabel(&mainWindow);
    label->setFixedWidth(winWidth);
    label->setFixedHeight(winHeight);

    // Output to window
    label->setText(dataText);
    mainWindow.show();

    return application.exec();

}
