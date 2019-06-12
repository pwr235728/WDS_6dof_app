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


#include "scenemodifier.h"

#include <QGuiApplication>

#include <Qt3DRender/qcamera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qcameralens.h>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QCommandLinkButton>
#include <QtGui/QScreen>

#include <Qt3DInput/QInputAspect>

#include <Qt3DExtras/qtorusmesh.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qtexture.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qpointlight.h>

#include <Qt3DCore/qtransform.h>
#include <Qt3DCore/qaspectengine.h>

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DExtras/qforwardrenderer.h>

#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSerialPort &port, QWidget *parent = nullptr);
    void view3d_init();
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

    SceneModifier *modifier;

private slots:
    void readSerial();
};

#endif // MAINWINDOW_H
