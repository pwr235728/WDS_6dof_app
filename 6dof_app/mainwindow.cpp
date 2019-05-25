#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QSerialPort &port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialPort = &port;

}

MainWindow::~MainWindow()
{
    delete ui;
}
