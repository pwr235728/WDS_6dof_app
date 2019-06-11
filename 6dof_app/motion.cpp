#include "motion.h"

Motion::Motion()
{

}

void Motion::AddData(ImuData imu_data)
{
    QVector3D data_acc;
    data_acc.setX(ACC_TO_G(imu_data.acc.x));
    data_acc.setX(ACC_TO_G(imu_data.acc.y));
    data_acc.setX(ACC_TO_G(imu_data.acc.z));

    QVector3D data_gyro;
    data_gyro.setX(GYRO_TO_DPS(imu_data.gyro.x));
    data_gyro.setX(GYRO_TO_DPS(imu_data.gyro.y));
    data_gyro.setX(GYRO_TO_DPS(imu_data.gyro.z));

    data_acc.push_back(data_acc);
    data_gyro.push_back(data_gyro);
}
