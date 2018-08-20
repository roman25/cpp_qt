#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Set test cases
const string openCase = "Open";
const string shortCase = "Short";
const string okCase = "Ok";


// Function for reading input data
void readInputData(string directoryPath, int *counterOpen, int *counterShort, int *counterOk)
{
    // Define input file extension
    const string inputFileExtension = ".txt";

    // Define variables that are help form full file name
    int counter = 1;
    string filePath;

    // Bool variable to read undefined count of files
    bool fileExists = true;

    // Base cycle
    while (fileExists)
    {
        // Set full path to a file
        filePath = directoryPath + to_string(counter) + inputFileExtension;

        // Create input stream
        ifstream readData(filePath);

        // Check if file is exists
        if (readData.is_open())
        {
            // Variable to store input data
            string inputData;

            // Read line
            while(getline(readData, inputData))
            {
                size_t found;

                found=inputData.find(openCase);
                if (found!=string::npos)
                    (*counterOpen)++;

                found=inputData.find(shortCase);
                if (found!=string::npos)
                    (*counterShort)++;

                found=inputData.find(okCase);
                if (found!=string::npos)
                    (*counterOk)++;
            }

            readData.close();
            counter++;
        }
        else
            fileExists = false;
    }
}

// Function to store statistic
void writeStatistic(string directoryPath, int *counterOpen, int *counterShort, int *counterOk)
{
    // Create output stream
    ofstream outfile;

    // Output file parameters
    const string fileName = "_cases_statistic";
    const string outputFileExtension = ".txt";
    const string delimeter = "\t";

    // Forming output file name and creating it
    string fullOutFileName = directoryPath + fileName + outputFileExtension;
    outfile.open(fullOutFileName);

    // Write data
    outfile << openCase << delimeter << *counterOpen << endl;
    outfile << shortCase << delimeter << *counterShort << endl;
    outfile << okCase << delimeter << *counterOk << endl;

    outfile.close();
}

int main()

{
    // Define input parameters
    const string directoryPath = "D:\\projects\\res_OpenShortTest\\";

    // Define variables to collect count of cases
    int counterOpen = 0;
    int counterShort = 0;
    int counterOk = 0;

    readInputData(directoryPath, &counterOpen, &counterShort, &counterOk);
    writeStatistic(directoryPath, &counterOpen, &counterShort, &counterOk);

    cout << "---------- Done ----------" << endl;
    return 0;
}
