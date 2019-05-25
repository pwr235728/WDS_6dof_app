#include "mainwindow.h"
#include <QApplication>

#include <QMessageBox>
#include <QDebug>
#include <QSerialPort>
#include <QString>
#include "selectport_dialog.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    
    SelectPort_Dialog selectPort;
    QSerialPort serial;

    do
    {
        if(selectPort.exec() == QDialog::Accepted){
            auto port = selectPort.GetSelectedPort();
            serial.close();
            serial.setPortName(port);
            if(!serial.open(QIODevice::ReadWrite)){
                auto msg = QObject::tr("Can't open %1, error code %2").arg(port).arg(serial.error());
                qDebug() << msg;

                QMessageBox messageBox;
                messageBox.critical(0,"Error", msg);
                messageBox.setFixedSize(500,200);
            }else {
                qDebug() << QObject::tr("Serial port %1, opened successfully").arg(port);
                break;
            }
        }else{
            return QDialog::Rejected;
        }
    }while(true);

    MainWindow mainApp(serial);
    mainApp.show();


    return a.exec();
}
