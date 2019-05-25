#ifndef SELECTPORT_DIALOG_H
#define SELECTPORT_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QString>
#include <QPushButton>

class SelectPort_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPort_Dialog(QWidget *parent = nullptr);

    QString GetSelectedPort();

private:

    QPushButton *connect_Button = nullptr;

    QString selectedPort = "";

    QLabel *serialPort_Label = nullptr;
    QComboBox *serialPort_ComboBox = nullptr;

    // port informations:
    QLabel *description_Label = nullptr;
    QLabel *descriptionValue_Label = nullptr;

public slots:
    void portChanged(const QString& str);

};

#endif // SELECTPORT_DIALOG_H
