#ifndef MOTION_H
#define MOTION_H

#include "imuparser.h"
#include <QVector>
#include <QVector3D>
#include <limits>


template <typename T>
constexpr float normalize (T value) {
  return value < 0
    ? -static_cast<float>(value) / std::numeric_limits<T>::min()
    :  static_cast<float>(value) / std::numeric_limits<T>::max()
    ;
}

/* ACC sens: 4G         */
/* GYRO sens: 1000s     */
/* Sample rate: 260Hz   */

// Degrees per seconds
#define GYRO_SENS 1000.0f
#define GYRO_TO_DPS(x) (normalize(x) * GYRO_SENS)

// Gs
#define ACC_SENS 4.0f
#define ACC_TO_G(x) (normalize(x) * ACC_SENS)

#define IMU_SampleRate 250.0f
#define IMU_SampleTime 1.0f/IMU_SampleRate

/* time constant greater than timescale of typical accelerometer noise */
#define TimeConstant 1.0f // 1s,


class Motion
{
public:
    Motion();

    void AddData(ImuData imu_data);

    QVector3D orientation = QVector3D(0,0,0); // filtered

private:
    const float dt = IMU_SampleTime;
    const float a = TimeConstant/(TimeConstant + dt);

    QVector<QVector3D> data_acc;
    QVector<QVector3D> data_gyro;

    QVector3D velocity = QVector3D(0,0,0);
    QVector3D position = QVector3D(0,0,0);

    QVector3D _accAngle = QVector3D(0,0,0);
    QVector3D _gyroAngle = QVector3D(0,0,0);

};

#endif // MOTION_H
