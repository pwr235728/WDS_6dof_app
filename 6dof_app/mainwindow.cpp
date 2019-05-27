#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xyseries.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(QSerialPort &port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs(new QTabWidget()),
    serialPort(&port),
    terminal(new Terminal()),
    receivedData(new QByteArray()),
    series(new QLineSeries())
{
    auto chart = new QChart();
    QChartView *chartView = new QChartView(chart);
    chartView->setMinimumSize(800, 600);
    chart->addSeries(series);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, XYSeries::sampleCount);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-10000, 10000);
    axisY->setTitleText("Audio level");
    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);
    chart->legend()->hide();
    chart->setTitle("Data ");

    ui->setupUi(this);
    tabs->addTab(terminal, "Terminal");

    tabs->addTab(chartView, "graph");

    xy_device = new XYSeries(series, this);
    xy_device->open(QIODevice::WriteOnly);

    setCentralWidget(tabs);

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readSerial()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readSerial()
{

    if(serialPort->bytesAvailable()<20)
        return;

    static int count = 0;

    auto bytes = serialPort->readAll();
    receivedData->append(bytes);

    ImuFrame frame;
    ImuData imuData;

    while(ImuParser::ReadFrame(*receivedData, frame) == PARSER_COMPLETE)
    {
        //qInfo("[MainWindow::readSerial()]: PARSER_INCOMPLETE");
        //return;

        if(frame.id != ImuParser::ID_IMU_DATA)
        {
            return;
        }
        if(ImuParser::GetImuData(frame, imuData) == PARSER_COMPLETE)
        {
            //terminal->putData("Complete packet\n");
            char* data = reinterpret_cast<char*>(&imuData.acc.x);

            x_arr.append(data[0]);
            x_arr.append(data[1]);
            if(x_arr.length() > 100)            {

                xy_device->write(x_arr, 100);
                x_arr.remove(0, 100);
            }
        }else
        {
            //qInfo("[MainWindow::readSerial()]: frame is not a ImuData");
        }
    }
    //qInfo("[MainWindow::readSerial()]: PARSER_INCOMPLETE");
}
