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

    QVector3D gyro;
    gyro.setX(GYRO_TO_DPS(imu_data.gyro.x));
    gyro.setY(GYRO_TO_DPS(imu_data.gyro.y));
    gyro.setZ(GYRO_TO_DPS(imu_data.gyro.z));


    // Gyroscope quaternion and integration
    QVector3D gyro_norm = gyro.normalized();
    QQuaternion Qw_dt = QQuaternion::fromAxisAndAngle(gyro_norm, dt*gyro.length());
    Qw = Qw*Qw_dt;

    // Accelerometer vector into world frame
    QQuaternion Qa_b = QQuaternion(0, acc);
    QQuaternion Qa_w =(Qw* Qa_b)* Qw.inverted();
    auto ac = Qw.rotatedVector(acc);

    // Tilt correction
    QVector3D tilt_vector = ac/ac.length();
    QVector3D tilt_normal = QVector3D::crossProduct(tilt_vector, QVector3D(0.0, 0.0, 1.0));
    float angle = qRadiansToDegrees(qAsin(tilt_vector.z()));

    // complementary filter
    // Gyro correction quaternion
    QQuaternion Qc = QQuaternion::fromAxisAndAngle(tilt_normal, (1.0-a)*(angle));
    Qw = (Qc*Qw);

    if(isnan(Qw.x()))
    {
        Qw = QQuaternion(1, 0,0,0);
    }

    //qInfo() << Qw << (Qc*Qw);;
}
