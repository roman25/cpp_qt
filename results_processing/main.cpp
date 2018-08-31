#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Set required test cases
const string openCase   = "Open";
const string shortCase  = "Short";

// Set parameters to parse and store data
const string fileName               = "_cases_statistic";
const string outputFileExtension    = ".txt";
const string inputFileExtension     = ".txt";
string delimiter                    = "\t";
size_t delimeterPosition            = 0;

void collectPinNames (string directoryPath, vector <string> *pins)
{
    /* Function collects name of contacts an integrated circuit */

    // Set full path to a file to collect contact names
    string filePath = directoryPath + "1" + inputFileExtension;

    // Create input stream
    string inputData;
    ifstream readData(filePath);

    string pinName;

    // Iterate for every row in the stream
    while(getline(readData, inputData))
    {
        delimeterPosition = inputData.find(delimiter);
        pinName = inputData.substr(0, delimeterPosition);

        // Skip unused rows
        if (pinName != "" && pinName != "////MKM4//// ")
            (*pins).push_back(pinName);
    }

    // Close input file
    readData.close();
}

void collectStatistic(string directoryPath, vector <string> *pins, vector <int> *counterOpen, vector <int> *counterShort)
{
    /* Function for reading input data */

    // Define variable that are help form full file name
    int counter = 1;

    // Bool variable to read undefined count of files
    bool fileExists = true;

    // Base cycle to read input file
    while (fileExists)
    {
        // Set full path to a file
        string filePath = directoryPath + to_string(counter) + inputFileExtension;

        // Create input stream
        ifstream readData(filePath);

        // Check if file is exists
        if (readData.is_open())
        {
            // Variable to store input data
            string inputData;
            string pinName;

            // Read a row from input
            while(getline(readData, inputData))
            {
                // Mapping between contact name and its test case
                // Defined as one to one relation
                for (int i= 0; i < (*pins).size(); i++)
                {
                    delimeterPosition = inputData.find(delimiter);

                    std::size_t foundopenCase   = inputData.find(openCase);
                    std::size_t foundshortCase  = inputData.find(shortCase);
                    pinName = inputData.substr(0, delimeterPosition);
                    string temp = (*pins)[i];
                    if (pinName == temp)
                    {
                        if (foundopenCase!=std::string::npos)
                            ((*counterOpen)[i])++;

                        if (foundshortCase!=std::string::npos)
                            ((*counterShort)[i])++;

                    }
                }

            }
          counter++;
          readData.close();
        }
        else
           fileExists = false;
    }
}

void writeStatistic(string directoryPath, vector <string> *pins, vector <int> *counterOpen, vector <int> *counterShort)
{
    /* Function stores statistic */

    // Create output stream
    ofstream outfile;
    string header;
    string delimeter;

    // Choose delimeter for output file
    if (outputFileExtension == ".txt")
        delimeter = "\t";
    else if (outputFileExtension == ".csv")
        delimeter = ",";

    // Define header
    header = "Pin name" + delimeter + shortCase + delimeter + openCase + "\n";

    // Forming output file name and creating it
    string fullOutFileName = directoryPath + fileName + outputFileExtension;
    outfile.open(fullOutFileName);

    // Write data
    outfile << header;
    for (int i = 0; i < (*pins).size(); i++)
        outfile << (*pins)[i] << delimeter << (*counterShort)[i] << delimeter << (*counterOpen)[i] << endl;

    outfile.close();
}

int main()

{
    // Define input path
    const string directoryPath = "D:\\projects\\res_OpenShortTest\\";

    // Define vectors to collect count of cases
    vector <string> pins;
    collectPinNames(directoryPath, &pins);

    int vectorSize = pins.size();
    vector <int> counterOpen(vectorSize, 0);
    vector <int> counterShort(vectorSize, 0);

    collectStatistic(directoryPath, &pins, &counterOpen, &counterShort);
    writeStatistic(directoryPath, &pins, &counterOpen, &counterShort);

    cout << "---------- Done ----------" << endl;

    return 0;
}
