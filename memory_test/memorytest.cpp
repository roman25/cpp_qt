#include <QtWidgets>
#include "memorytest.h"

// List of allowed IC type
const QStringList memoryType  = {"V4", "L06B", "L95B"};

// List of allowed file extensions
const QStringList extensions  = {"*.xlsm", "*.xlsx", "*.xls"};

// Paramters of lot name
const QStringList productCode   = {"N"};
const QStringList crystalCode   = {"V", "L", "B"};
const QStringList countStack    = {"1", "2", "4", "8", "16"};
const QStringList countChannels = {"1", "2"};
const QStringList countTargets  = {"1", "2", "4"};
const QStringList LunOnTargets  = {"1", "2"};
const QStringList status        = {"P", "E", "Q", "R"};

Window::Window(QWidget *parent):QWidget(parent)
{
    // Create parent window
    QGridLayout *mainLayout = new QGridLayout(this);

    // Define header and screen size
    setWindowTitle(tr("Memory test"));
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    resize(screenGeometry.width() / 2, screenGeometry.height() / 3);

    // Define temporary list of used ICs. It needs for initialization of combobox
    fileComboBox = createComboBox(memoryType);

    // Create combobox with initial value which informs about some action
    configComboBox = createComboBox({"Need to set path to config files to get list of the files"});

    // Create lines
    lineTestCase        = showLine();
    lineType            = showLine();
    lineCountChannels   = showLine();
    lineCountTargets    = showLine();
    lineLunTarget       = showLine();
    lineConfig          = showLine();
    lineSamples         = showLine();

    // Create buttons for GUI
    QPushButton *validateButton = new QPushButton(tr("Validate"));
    connect(validateButton, &QAbstractButton::clicked, this, &Window::getLotNumber);

    QPushButton *browseButton = new QPushButton(tr("Browse"));
    connect(browseButton, &QAbstractButton::clicked, this, &Window::browse);

    QPushButton *launchTest = new QPushButton(tr("Run"));
    connect(launchTest, &QAbstractButton::clicked, this, &Window::getInputParameters);

    // Add graphic elements into the parent window
    mainLayout->addWidget(new QLabel("Set a path to the config"), 0, 0);
    mainLayout->addWidget(lineConfig, 0, 1);
    mainLayout->addWidget(browseButton, 0, 2);

    mainLayout->addWidget(new QLabel("Lot number"), 1, 0);
    mainLayout->addWidget(lineTestCase, 1, 1);
    mainLayout->addWidget(validateButton, 1, 2);
    lineTestCase->setPlaceholderText("You can use UP, low or mIX case");

    mainLayout->addWidget(new QLabel("Type of memory"), 2, 0);
    mainLayout->addWidget(fileComboBox, 2, 1);

    mainLayout->addWidget(new QLabel("Count of Channels"), 3, 0);
    mainLayout->addWidget(lineCountChannels, 3, 1);

    mainLayout->addWidget(new QLabel("Count of targets"), 4, 0);
    mainLayout->addWidget(lineCountTargets, 4, 1);

    mainLayout->addWidget(new QLabel("Lun on target"), 5, 0);
    mainLayout->addWidget(lineLunTarget, 5, 1);

    mainLayout->addWidget(new QLabel("Config File"), 6, 0);
    mainLayout->addWidget(configComboBox, 6, 1);

    mainLayout->addWidget(new QLabel("QTY"), 7, 0);
    mainLayout->addWidget(lineSamples, 7, 1);

    mainLayout->addWidget(launchTest, 8, 0);
}

QComboBox *Window::createComboBox(const QStringList &list)
{
    /*!
        Function creates a box with multiple values and shows their
    */

    QComboBox *comboBox = new QComboBox;
    comboBox->addItems(list);
    comboBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    return comboBox;
}

QLineEdit *Window::showLine()
{
    /*!
        Creates and returns line for typing
    */

    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setFocus();
    return lineEdit;
}

void Window::browse()
{
    /*!
        Helps set path to a folder
    */

    // Create variable to store input path
    QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files")));

    // Show path to input directory if path is not empty
    if (!directory.isEmpty()) {
        lineConfig->setText(directory); // Show path in GUI
        findFilesInDirectory(directory);
    }
}

void Window::findFilesInDirectory(QString pathToDirectory)
{
    /*!
        Filters files in a directory based on file extension
    */

    // Storage for filtered files
    QStringList filteredFiles = {""};

    // Path to directory
    QDir folder(pathToDirectory);

    // Filter files in folder using predefined extensions
    folder.setNameFilters(QStringList() << extensions[0] << extensions[1] << extensions[2]);
    filteredFiles = folder.entryList();

    // Clear all values in box
    configComboBox->clear();

    // Add data to box
    for (qint8 i = 0; i < filteredFiles.size(); i++)

        // Set files for combobox with required extensions
        configComboBox->addItem(filteredFiles[i]);
}

void Window::selectValueInBox(QComboBox *box, QString inputValue)
{
    /*!
        Updates values in a box
    */

    // Select value in the box
    qint8 index = box->findText(inputValue, Qt::MatchContains);
    box->setCurrentIndex(index);
    box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

void Window::getLotNumber()
{
    /*!
        Checks input parameter has mapping to predefined template
    */

    // Get IC name from GUI
    QString inputValue = this->lineTestCase->text();

    // Change text case to upper
    inputValue = inputValue.toUpper();

    // Define variables for forming number of lot
    QString lotNumber       = "[";
    QString parametersOfLot = "[0-9._]{4,8}"; // Allows use digits, dot and underscore (not more than 8 symbols)
    QString endOfLine       = "$";

    // Verify values of lot number
    checkInputLotNumber(inputValue);

    // Generate template for product code
    for (quint8 i = 0; i < productCode.size(); i++)
        lotNumber +=productCode[i];
    lotNumber += "][";

    // Generate template for crystal code
    for (quint8 i = 0; i < crystalCode.size(); i++)
        lotNumber +=crystalCode[i];
    lotNumber += "][";

    // Generate template for count of stack code. There is 1 or 2-digits number
    lotNumber += "0-9]{1,2}[";

    // Generate template for status
    for (quint8 i = 0; i < status.size(); i++)
        lotNumber +=status[i];
    lotNumber += "]";

    // Create output string
    lotNumber = lotNumber + parametersOfLot + endOfLine;

    // Verify input name is consistent with required template
    QRegularExpression re(lotNumber);
    QRegularExpressionMatch match = re.match(inputValue);

    // Create bool variable that defines consistent between template and input from UI
    bool hasMatch = match.hasMatch();
    if (hasMatch)
    {
        // Check count of stack is 1 or 2 digits number
        QString tempStack = "";
        tempStack = inputValue[2];
        if (inputValue[3].isDigit())
            tempStack += inputValue[3];

        // Processing count of stack from UI. Set channels, targets, luns based on the input
        QString channels = "";
        QString targets  = "";
        QString luns     = "";

        if (tempStack == countStack[0])
        {
            channels    = "1";
            targets     = "1";
            luns        = "1";
        }
        else if (tempStack == countStack[1])
        {
            channels    = "1";
            targets     = "2";
            luns        = "1";
        }
        else if (tempStack == countStack[2])
        {
            channels    = "1";
            targets     = "2";
            luns        = "2";
        }
        else if (tempStack == countStack[3])
        {
            channels    = "1";
            targets     = "4";
            luns        = "2";
        }
        else if (tempStack == countStack[4])
        {
            channels    = "2";
            targets     = "4";
            luns        = "2";
        }

        // Set values into UI based on conditions above
        lineCountChannels->setText(channels);
        lineCountTargets->setText(targets);
        lineLunTarget->setText(luns);

        // Update comboboxes widgets
        // Value in combobox is based on lot number from UI
        // Index [1] in input is memory type always
        QString memory      = "";
        QString confFile    = "";
        if (inputValue[1] == crystalCode[0])
        {
            memory      = memoryType[0];
            confFile    = memoryType[0];
        }
        else if (inputValue[1] == crystalCode[1])
        {
            memory      = memoryType[1];
            confFile    = memoryType[1];
        }
        else if (inputValue[1] == crystalCode[2])
        {
            memory      = memoryType[2];
            confFile    = memoryType[2];
        }
        selectValueInBox(fileComboBox, memory);
        selectValueInBox(configComboBox, confFile);
    }
    else
        // If no match, warning message will be shown
        showStatusMessage("Incorrect lot number");
}

void Window::getInputParameters()
{
    /*!
        Helps get all input parameters from UI and use their for other functions/methods
    */

    // Get values from UI
    QString uiConfigPath    = lineConfig ->text();
    QString uiLotNumber     = lineTestCase->text();
    QString uiMemoryType    = fileComboBox->currentText();
    QString uiCountChannel  = lineCountChannels->text();
    QString uiCountTargets  = lineCountTargets->text();
    QString uiLunTarget     = lineLunTarget->text();
    QString uiConfigName    = configComboBox->currentText();
    QString uiSamples       = lineSamples->text();

    // Verify no empty input parameters
    bool path       = uiConfigPath.isEmpty();
    bool lot        = uiLotNumber.isEmpty();
    bool memory     = uiMemoryType.isEmpty();
    bool channes    = uiCountChannel.isEmpty();
    bool targets    = uiCountTargets.isEmpty();
    bool lun        = uiLunTarget.isEmpty();
    bool config     = uiConfigName.isEmpty();
    bool samples    = uiSamples.isEmpty();

    if (path)
        showStatusMessage("Config path is empty");
    if (lot)
        showStatusMessage("Lot number is empty");
    if (memory)
        showStatusMessage("Memory type is empty");
    if (channes)
        showStatusMessage("Count of channels is empty");
    if (targets)
        showStatusMessage("Count of targets is empty");
    if (lun)
        showStatusMessage("Lun on targets is empty");
    if (config)
        showStatusMessage("Config name is empty");

    // Check count of samples > 0
    qint8 countSamples;
    countSamples = uiSamples.toInt();
    if (samples || (!(countSamples > 0)))
        showStatusMessage("Count samples is empty or smaller than 0");

    // Check path to config file
    QString FullPathToConfig = uiConfigPath + '\\' + uiConfigName;
    bool file = QFileInfo::exists(FullPathToConfig);
    if (!file)
        showStatusMessage("Check path to the file configuration");

    // Any actions can be perform if no issues with input parameters
    if (!path && !lot && !memory && !channes && !targets && !lun && !config && !samples && (countSamples > 0) && file)
    {        
        // Output to console for testing purpose only!
        qInfo() << "Path to config "    << uiConfigPath;
        qInfo() << "LotNumber "         << uiLotNumber;
        qInfo() << "MemoryType "        << uiMemoryType;
        qInfo() << "ConfigDirectory "   << uiConfigPath;
        qInfo() << "CountChannel "      << uiCountChannel;
        qInfo() << "CountTargets "      << uiCountTargets;
        qInfo() << "LunTarget "         << uiLunTarget;
        qInfo() << "Config name "       << uiConfigName;
        qInfo() << "Samples "           << uiSamples;
        qInfo() << "FullPathToConfig "  << FullPathToConfig;
    }
}

void Window::showStatusMessage(QString message)
{
    /*!
        Function shows warning message
     */
    QMessageBox::warning(this, "Warning!", message);
}

void Window::checkInputLotNumber (QString lotNumber)
{
    /*!
        Verifies that input lot number accords predefined lists
     */

    QString message     = "Incorrect input data.\n";
    QString values      = "";
    QString stackValue  = "";
    QString statusValue = "";
    quint8 counter      = 0;
    bool showMessage    = false;

    // *************** Check product code value ***************
    // Iterate for every value of product codes
    for (qint8 i = 0; i < productCode.size(); i++)
    {
        // Collect allowed values
        values = values + "or \"" + productCode[i] + "\" ";

        // Find occurrence of input value in predefined list
        // If we have at least 1 occurrence current cycle is stopped
        if ((lotNumber[0] == productCode[i]))
            counter++;
        if (counter)
            break;
    }

    // If input code is not occurrence in predefined list message will be shown
    if (!counter)
    {
        message = message + "Instead of " + lotNumber[0] + " must be " + values + "\n";
        showMessage = true;
    }

    // *************** Check crystal code value ***************
    // Clear varibales before new using
    counter = 0;
    values  = "";

    // Iterate for every value of crystal codes
    for (qint8 i = 0; i < crystalCode.size(); i++)
    {
        // Collect allowed values
        values = values + "or \"" + crystalCode[i] + "\" ";

        // Find occurrence of input value in predefined list
        // If we have at least 1 occurrence current cycle is stopped
        if ((lotNumber[1] == crystalCode[i]))
            counter++;
        if (counter)
            break;
    }

    // If input code is not occurrence in predefined list message will be shown
    if (!counter)
    {
        message = message + "Instead of " + lotNumber[1] + " must be " + values + "\n";
        showMessage = true;
    }

    // *************** Check count of stack is 2 digits number or not ***************
    if (!(lotNumber[3].isDigit()))
    {
        // This block shows count of stack is 1 digit number
        stackValue  = lotNumber[2];
        statusValue = lotNumber[3];
    }
    else
    {
        // This block shows count of stack is 2 digit number
        stackValue = lotNumber[2];
        stackValue += lotNumber[3];
        statusValue = lotNumber[4];
    }

    // *************** Check count stack value ***************
    // Clear varibales before new using
    counter = 0;
    values  = "";

    // Iterate for every value of count stack
    for (qint8 i = 0; i < countStack.size(); i++)
    {
        // Collect allowed values
        values = values + "or \"" + countStack[i] + "\" ";

        // Find occurrence of input value in predefined list
        // If we have at least 1 occurrence current cycle is stopped

        // countStack    = {"1", "2", "4", "8", "16"};
        if ( stackValue == countStack[i])
            counter++;
        if (counter)
            break;
    }

    // If input code is not occurrence in predefined list message will be shown
    if (!counter)
    {
        message = message + "Instead of " + stackValue + " must be " + values + "\n";
        showMessage = true;
    }

    // *************** Check status value ***************
    // Clear varibales before new using
    counter = 0;
    values  = "";

    // Iterate for every value of status
    for (qint8 i = 0; i < status.size(); i++)
    {
        // Collect allowed values
        values = values + "or \"" + status[i] + "\" ";

        // Find occurrence of input value in predefined list
        // If we have at least 1 occurrence current cycle is stopped
        if (statusValue == status[i])
            counter++;
        if (counter)
            break;
    }

    // If input code is not occurrence in predefined list message will be shown
    if (!counter)
    {
        message = message + "Instead of " + statusValue + " must be " + values + "\n";
        showMessage = true;
    }

    // Show status message
    if (showMessage)
        showStatusMessage(message);
}
