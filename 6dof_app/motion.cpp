#include "motion.h"
#include <QtMath>
#include <QDebug>
Motion::Motion()
{

}

void Motion::AddData(ImuData imu_data)
{
    QVector3D acc;
    acc.setX(ACC_TO_G(imu_data.acc.x));
    acc.setY(ACC_TO_G(imu_data.acc.y));
    acc.setZ(ACC_TO_G(imu_data.acc.z));

    acc.normalize();

    QVector3D gyro;
    gyro.setX(GYRO_TO_DPS(imu_data.gyro.x));
    gyro.setY(GYRO_TO_DPS(imu_data.gyro.y));
    gyro.setZ(GYRO_TO_DPS(imu_data.gyro.z));

    //data_acc.push_back(acc);
   // data_gyro.push_back(gyro);


    _gyroAngle.setX(orientation.x() + gyro.x()*dt);
    _gyroAngle.setY(orientation.y() + gyro.y()*dt);
    _gyroAngle.setZ(orientation.z() + gyro.z()*dt);


    // X/sqrt(Y^2 + Z^2)
    _accAngle.setX(qAsin(acc.y()));
    _accAngle.setY(qAsin(acc.x()));
    _accAngle.setZ(qTan(qSqrt(acc.x()*acc.x() + acc.y()*acc.y())/acc.z()));

    _accAngle.setX(qRadiansToDegrees(_accAngle.x()));
    _accAngle.setY(qRadiansToDegrees(_accAngle.y()));
    _accAngle.setZ(qRadiansToDegrees(_accAngle.z()));

    orientation.setX(a*_gyroAngle.x() + (1.0f - a)*_accAngle.x());
    orientation.setY(a*_gyroAngle.y() + (1.0f - a)*-_accAngle.y());
    orientation.setZ(a*_gyroAngle.z() + (1.0f - a)*-_accAngle.y());

    qInfo() << "new data: " << _accAngle << gyro << "\n";
}
