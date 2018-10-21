#include <QtWidgets>
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Memory test"));

    // Set info text
    ui->lineLotNumber->setPlaceholderText("You can use UP, low or mIX case");

    // Add initial parameters into combobox
    createComboBox(ui->comboBoxMemory, memoryType);

    // Performs actions when Browse button is clicked
    connect(ui->pushButtonBrowse, &QAbstractButton::clicked, this, &Dialog::browse);

    // Perfomrs actions when OK button is clicked
    connect(ui->pushButtonOk, SIGNAL(pressed ()), this, SLOT(on_pushButtonOk_clicked()));

    // Perfomrs actions when Cancel button is clicked
    connect(ui->pushButtonCancel, SIGNAL(pressed ()), this, SLOT(close()));

}

Dialog::~Dialog()
{
    delete ui;
}

QComboBox *Dialog::createComboBox(QComboBox *comboBox, const QStringList &list)
{
    /*!
        Function creates a box with multiple values and shows their
    */

    comboBox->addItems(list);
    comboBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    return comboBox;
}

void Dialog::browse()
{
    /*!
        Helps set path to a folder
    */

    // Create variable to store input path
    QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files")));

    // Show path to input directory if path is not empty
    if (!directory.isEmpty()) {
        ui->lineConfig->setText(directory); // Show path in GUI
        findFilesInDirectory(directory);
    }
}

void Dialog::findFilesInDirectory(QString pathToDirectory)
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
    ui->comboBoxConfig->clear();

    // Add data to box
    for (qint8 i = 0; i < filteredFiles.size(); i++)

        // Set files for combobox with required extensions
        ui->comboBoxConfig->addItem(filteredFiles[i]);
}

void Dialog::selectValueInBox(QComboBox *box, QString inputValue)
{
    /*!
        Updates values in a box
    */

    // Select value in the box
    qint8 index = box->findText(inputValue, Qt::MatchContains);
    box->setCurrentIndex(index);
    box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

bool Dialog::checkLotNumber()
{
    /*!
        Checks input parameter has mapping to predefined template
    */

    // Get IC name from GUI
    QString inputValue =ui->lineLotNumber->text();

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
        // Set default values into UI
        ui->lineCountChannels->setText(countChannels[0]);
        ui->lineCountTargets->setText(countTargets[0]);
        ui->lineLunTarget->setText(LunOnTargets[0]);

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
        selectValueInBox(ui->comboBoxMemory, memory);
        selectValueInBox(ui->comboBoxConfig, confFile);

        return true;
    }
    else
    {
        // If no match, warning message will be shown
        showStatusMessage("Incorrect lot number");
        return false;
     }
}

bool Dialog::verifyInputParameters()
{
    /*!
        Verifies all input parameters from UI
    */

    // Get values from UI
    QString uiConfigPath    = ui->lineConfig ->text();
    QString uiLotNumber     = ui->lineLotNumber->text();
    QString uiMemoryType    = ui->comboBoxMemory->currentText();
    QString uiCountChannel  = ui->lineCountChannels->text();
    QString uiCountTargets  = ui->lineCountTargets->text();
    QString uiLunTarget     = ui->lineLunTarget->text();
    QString uiConfigName    = ui->comboBoxConfig->currentText();
    QString uiSamples       = ui->lineQty->text();

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

    if (!path && !lot && !memory && !channes && ! targets && !lun && !config && (countSamples > 0) && file)
        return true;
    else
        return false;
}

void Dialog::showStatusMessage(QString message)
{
    /*!
        Function shows warning message
     */
    QMessageBox::warning(this, "Warning!", message);
}

void Dialog::checkInputLotNumber (QString lotNumber)
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

void Dialog::on_pushButtonOk_clicked()
{
    /*!
     Checks all requiref parameters are right and
     close window
     */

    bool lot, parameters, quit;

    // Check lot number is correct
    lot = checkLotNumber();
    if (lot)

        // Check all paramteres
        parameters = verifyInputParameters();
    {
        // Close window if all parameters are ok
        if (parameters)
        {
            quit = confirmQuit();
            if (quit)
                close();
        }
    }
}

QString Dialog::getPathToConfig()
{
    /*!
     Gets path to file configuration, its name and returns full path
    */

    // Get path configuration
    QString path = ui->lineConfig->text();

    // Get file name
    QString name = ui->comboBoxConfig->currentText();

    // Define full path to configuration
    QString fullPath = path + "\\" + name;

    return fullPath;

}

QString Dialog::getLot()
{
    /*!
      Function gets lot number from UI and returns it
    */

    // Get value from UI
    QString lot = ui->lineLotNumber->text();

    return lot;
}

QString Dialog::getMemoryType()
{
    /*!
      Gets memory type and returns it
    */

    // Get value from UI
    QString stringMemoryType = ui->comboBoxMemory->currentText();

    return stringMemoryType;

}

int Dialog::getCountChannels()
{
    /*!
      Gets and returns count of channels
    */

    // Get value from UI
    QString stringCountChannels = ui->lineCountChannels->text();

    // Convert string to number
    int channels = stringCountChannels.toInt();

    return channels;
}

int Dialog::getCountTargets()
{
    /*!
      Gets and returns count of targets
    */

    // Get value from UI
    QString stringCountTargets = ui->lineCountTargets->text();

    // Convert string to number
    int targets = stringCountTargets.toInt();

    return targets;

}

int Dialog::getLunTarget()
{
    /*!
      Return value lun on target
    */

    // Get value from UI
    QString stringLunTarget = ui->lineLunTarget->text();

    // Convert string to number
    int luns = stringLunTarget.toInt();

    return luns;
}

int Dialog::getQty()
{
    /*!
    Gets and returns QTY
    */

    // Get value from UI
    QString stringQty = ui->lineQty->text();

    // Convert string to number
    int Qty = stringQty.toInt();

    return Qty;

}

bool Dialog::confirmQuit()
{
    /*!
    Creates window for confirmation of closing
   */

  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Окно подтверждения", "Параметры указаны верно?", QMessageBox::Yes|QMessageBox::No);
  if (reply == QMessageBox::Yes)
      return true;
  else
      return false;
}
