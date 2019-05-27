#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTabWidget>

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

#include "terminal.h"
#include "imuparser.h"
#include "xyseries.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSerialPort &port, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabs = nullptr;

    QSerialPort *serialPort = nullptr;
    Terminal *terminal = nullptr;

    QByteArray *receivedData = nullptr;

    QLineSeries *series = nullptr;
    XYSeries *xy_device = nullptr;

    QByteArray x_arr;

private slots:
    void readSerial();
};

#endif // MAINWINDOW_H
