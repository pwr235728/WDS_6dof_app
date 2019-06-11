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

// Degrees per seconds
#define GYRO_SENS 1000.0f
#define GYRO_TO_DPS(x) (normalize(x) * GYRO_SENS)


// Gs
#define ACC_SENS 4.0f
#define ACC_TO_G(x) (normalize(x) * ACC_SENS)
class Motion
{
public:
    Motion();

    void AddData(ImuData imu_data);


private:
    QVector<QVector3D> data_acc;
    QVector<QVector3D> data_gyro;

    QVector3D velocity;
    QVector3D position;
    QVector3D orientation;
};

#endif // MOTION_H
