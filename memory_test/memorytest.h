#ifndef MEMORYTEST_H
#define MEMORYTEST_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;

QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);

private slots:
    void browse();
    void getLotNumber();
    void getInputParameters();

private:
    QComboBox *createComboBox(const QStringList &list = QStringList());
    QLineEdit *showLine();

    void findFilesInDirectory(QString pathToDirectory);
    void selectValueInBox(QComboBox *box, QString inputValue);
    void showStatusMessage(QString message);
    void checkInputLotNumber(QString lotNumber);

    QComboBox *fileComboBox;
    QComboBox *configComboBox;

    QLineEdit *lineTestCase;
    QLineEdit *lineType;    
    QLineEdit *lineCountChannels;
    QLineEdit *lineCountTargets;
    QLineEdit *lineLunTarget;
    QLineEdit *lineConfig;
    QLineEdit *lineSamples;

};
#endif // MEMORYTEST_H
