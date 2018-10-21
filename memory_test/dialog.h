#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class QComboBox;
class QLineEdit;

#include <QtWidgets>
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void browse(); 
    void on_pushButtonOk_clicked();

private:
    Ui::Dialog *ui;

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

    QComboBox *createComboBox(QComboBox *box, const QStringList &list = QStringList());
    bool verifyInputParameters();
    bool checkLotNumber();
    bool confirmQuit();
    void findFilesInDirectory(QString pathToDirectory);
    void selectValueInBox(QComboBox *box, QString inputValue);
    void showStatusMessage(QString message);
    void checkInputLotNumber(QString lotNumber);

public:
    // Functions helps get parameters of UI in other windows
    QString getPathToConfig();
    QString getLot();
    QString getMemoryType();
    int getCountChannels();
    int getCountTargets();
    int getLunTarget();
    int getQty();

};

#endif // DIALOG_H
