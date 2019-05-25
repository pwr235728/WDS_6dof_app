#include "selectport_dialog.h"
#include <QSerialPortInfo>
#include <QGridLayout>
#include <QDebug>

SelectPort_Dialog::SelectPort_Dialog(QWidget *parent)
    : QDialog (parent),
      connect_Button(new QPushButton(tr("Connect"))),
      serialPort_Label(new QLabel(tr("Serial port:"))),
      serialPort_ComboBox(new QComboBox),
      description_Label(new QLabel(tr("Description:"))),
      descriptionValue_Label(new QLabel(tr("-")))
{
    connect(serialPort_ComboBox, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            this, static_cast<void(SelectPort_Dialog::*)(const QString&)>(&SelectPort_Dialog::portChanged));

    connect(connect_Button, &QPushButton::clicked, this, &SelectPort_Dialog::accept);

    auto layout = new QGridLayout();
    layout->setColumnMinimumWidth(0, 300);
    layout->addWidget(serialPort_Label);
    layout->addWidget(serialPort_ComboBox);
    layout->addWidget(description_Label);
    layout->addWidget(descriptionValue_Label);
    layout->addWidget(connect_Button);
    setLayout(layout);
    setWindowTitle(tr("Select serial port"));



    const auto infos = QSerialPortInfo::availablePorts();
    for( const QSerialPortInfo &info :  infos)
    {
        serialPort_ComboBox->addItem(info.portName());
    }
}


 QString SelectPort_Dialog::GetSelectedPort()
 {
    return selectedPort;
 }

 void SelectPort_Dialog::portChanged(const QString &str)
 {
     auto infos = QSerialPortInfo::availablePorts();
     for( const QSerialPortInfo &info :  infos)
     {
         if(info.portName() == str)
         {
            descriptionValue_Label->setText(info.description());
            selectedPort = info.portName();
            break;
         }
     }
 }

