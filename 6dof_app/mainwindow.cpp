#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xyseries.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QGridLayout>

#include <QVector3D>

MainWindow::MainWindow(QSerialPort &port, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tabs(new QTabWidget()),
    serialPort(&port),
    terminal(new Terminal()),
    receivedData(new QByteArray()),
    motion(new Motion())
{
    acc_series[0] = new QLineSeries();
    acc_series[1] = new QLineSeries();
    acc_series[2] = new QLineSeries();

    gyro_series[0] = new QLineSeries();
    gyro_series[1] = new QLineSeries();
    gyro_series[2] = new QLineSeries();

    QChartView* acc_chartView[3];
    QChartView* gyro_chartView[3];

    QString acc_title[3];
    acc_title[0] = "Accelerometer X axis";
    acc_title[1] = "Accelerometer Y axis";
    acc_title[2] = "Accelerometer Z axis";

    QString gyro_title[3];
    gyro_title[0] = "Gyroscope X axis";
    gyro_title[1] = "Gyroscope Y axis";
    gyro_title[2] = "Gyroscope Z axis";
    // Accelerometer charts
    for(int i=0;i<3;i++)
    {
        auto acc_chart = new QChart();
        acc_chartView[i] = new QChartView(acc_chart);
        acc_chartView[i]->setMinimumSize(400,100);
        acc_chart->addSeries(acc_series[i]);
        auto acc_axisX = new QValueAxis();
        acc_axisX->setRange(0, XYSeries::sampleCount);
        acc_axisX->setLabelFormat("%g");
        acc_axisX->setTitleText("Samples");
        auto acc_axisY = new QValueAxis();
        acc_axisY->setRange(-32768, 32767);
        acc_axisY->setTitleText("Raw value");

        acc_chart->addAxis(acc_axisX,Qt::AlignBottom);
        acc_series[i]->attachAxis(acc_axisX);
        acc_chart->addAxis(acc_axisY,Qt::AlignLeft);
        acc_series[i]->attachAxis(acc_axisY);
        //acc_chart->setAxisX(acc_axisX, acc_series[i]);
        //acc_chart->setAxisY(acc_axisY, acc_series[i]);

        acc_chart->legend()->hide();
        acc_chart->setTitle(acc_title[i]);

        acc_device[i] = new XYSeries(acc_series[i]);
        acc_device[i]->open(QIODevice::WriteOnly);
    }

    // Gyro charts
    for(int i=0;i<3;i++)
    {
        auto gyro_chart = new QChart();
        gyro_chartView[i] = new QChartView(gyro_chart);
        gyro_chartView[i]->setMinimumSize(400,100);
        gyro_chart->addSeries(gyro_series[i]);
        auto gyro_axisX = new QValueAxis();
        gyro_axisX->setRange(0, XYSeries::sampleCount);
        gyro_axisX->setLabelFormat("%g");
        gyro_axisX->setTitleText("Samples");
        auto gyro_axisY = new QValueAxis();
        gyro_axisY->setRange(-32768, 32767);
        gyro_axisY->setTitleText("Raw value");

        gyro_chart->addAxis(gyro_axisX,Qt::AlignBottom);
        gyro_series[i]->attachAxis(gyro_axisX);
        gyro_chart->addAxis(gyro_axisY,Qt::AlignLeft);
        gyro_series[i]->attachAxis(gyro_axisY);
        //gyro_chart->setAxisX(gyro_axisX,gyro_series[i]);
        //gyro_chart->setAxisY(gyro_axisY,gyro_series[i]);

        gyro_chart->legend()->hide();
        gyro_chart->setTitle(gyro_title[i]);

        gyro_device[i] = new XYSeries(gyro_series[i]);
        gyro_device[i]->open(QIODevice::WriteOnly);
    }


    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->addWidget(acc_chartView[0], 0, 0);
    gridLayout->addWidget(acc_chartView[1], 1, 0);
    gridLayout->addWidget(acc_chartView[2], 2, 0);

    gridLayout->addWidget(gyro_chartView[0], 0, 1);
    gridLayout->addWidget(gyro_chartView[1], 1, 1);
    gridLayout->addWidget(gyro_chartView[2], 2, 1);


    ui->setupUi(this);

    QWidget *w = new QWidget();
    w->setLayout(gridLayout);
    tabs->addTab(w, "Charts");
    tabs->addTab(terminal, "Terminal");

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

            motion->AddData(imuData);

            acc[0].append(imuData.acc.x);
            acc[1].append(imuData.acc.y);
            acc[2].append(imuData.acc.z);

            gyro[0].append(imuData.gyro.x);
            gyro[1].append(imuData.gyro.y);
            gyro[2].append(imuData.gyro.z);

            static const int len = 20;
            if(acc[0].length() > len )            {

                acc_device[0]->write(QByteArray(reinterpret_cast<const char*>(acc[0].data()), int(sizeof(int16_t))*acc[0].size()));
                acc_device[1]->write(QByteArray(reinterpret_cast<const char*>(acc[1].data()), int(sizeof(int16_t))*acc[1].size()));
                acc_device[2]->write(QByteArray(reinterpret_cast<const char*>(acc[2].data()), int(sizeof(int16_t))*acc[2].size()));

                gyro_device[0]->write(QByteArray(reinterpret_cast<const char*>(gyro[0].data()), int(sizeof(int16_t))*gyro[0].size()));
                gyro_device[1]->write(QByteArray(reinterpret_cast<const char*>(gyro[1].data()), int(sizeof(int16_t))*gyro[1].size()));
                gyro_device[2]->write(QByteArray(reinterpret_cast<const char*>(gyro[2].data()), int(sizeof(int16_t))*gyro[2].size()));

                acc[0].remove(0, len );
                acc[1].remove(0, len );
                acc[2].remove(0, len );

                gyro[0].remove(0, len );
                gyro[1].remove(0, len );
                gyro[2].remove(0, len );
            }
        }else
        {
            //qInfo("[MainWindow::readSerial()]: frame is not a ImuData");
        }
    }
    //qInfo("[MainWindow::readSerial()]: PARSER_INCOMPLETE");
}
