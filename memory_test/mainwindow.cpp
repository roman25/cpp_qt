#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSettings_triggered()
{
    /*!
      creates, opens dialog window, returns values from
      its and set values into main window
     */

    // Create dialog
    dialogWin = new Dialog(this);

    // Get pressed value from dialog
    dialogWin->exec();

       // Get values from dialog window
    QString pathToConfig = dialogWin->getPathToConfig();
    QString lot          = dialogWin->getLot();
    QString memoryType   = dialogWin->getMemoryType();
    int channels         = dialogWin->getCountChannels();
    int targets          = dialogWin->getCountTargets();
    int lunTargets       = dialogWin->getLunTarget();
    int qty              = dialogWin->getQty();

    // Convert strung values to set as labels into UI
    QString stringChannels   = QString::number(channels);
    QString stringTargets    = QString::number(targets);
    QString stringLunTargets = QString::number(lunTargets);
    QString stringQty        = QString::number(qty);

    // Set values on UI
    ui->label->setText(pathToConfig);
    ui->label_2->setText(lot);
    ui->label_3->setText(memoryType);
    ui->label_4->setText(stringChannels);
    ui->label_5->setText(stringTargets);
    ui->label_6->setText(stringLunTargets);
    ui->label_7->setText(stringQty);

}
