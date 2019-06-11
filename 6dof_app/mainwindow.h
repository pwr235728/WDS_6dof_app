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
#include "motion.h"

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

    QLineSeries* acc_series[3];
    XYSeries* acc_device[3];

    QLineSeries* gyro_series[3];
    XYSeries *gyro_device[3];

    QLineSeries* orientation_series[3];
    XYSeries *orientation_device[3];

    Motion *motion = nullptr;

    QVector<int16_t> acc[3];    // [X; Y; Z]
    QVector<int16_t> gyro[3];   // [X; Y; Z]
    QVector<int16_t> orientation[3];   // [X; Y; Z]


private slots:
    void readSerial();
};

#endif // MAINWINDOW_H
