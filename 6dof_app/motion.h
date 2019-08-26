#ifndef MOTION_H
#define MOTION_H

#include "imuparser.h"
#include <QVector>
#include <QVector3D>
#include <limits>

#include <QQuaternion>

//! Funkcja zamienia liczbe dowolnego typu na zakres -1.0f do 1.0f typu float
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
#define GYRO_TO_DPS(x) ((normalize(x) * GYRO_SENS))

// Gs
#define ACC_SENS 4.0f
#define ACC_TO_G(x) (normalize(x) * ACC_SENS)

#define IMU_SampleRate 250.0f
#define IMU_SampleTime 1.0f/IMU_SampleRate

/* time constant greater than timescale of typical accelerometer noise */
#define TimeConstant 1.0f // 1s,


/**
 * @brief Klasa Motion
 *
 * Klasa służaca do liczenia oreintacj czujnik IMU
 * na podstawie danych z żyroskopu i akcelerometru.
 */
class Motion
{
public:
    Motion();

    //! Funkcja do wprowadzania kolejnych danych z czujnika.
    void AddData(ImuData imu_data);

    //! Kwaternion opisujący ostatnio obliczoną orientacje sensora.
    QQuaternion Qw = QQuaternion(1, 0, 0, 0);

private:
    //! Okres próbkowania danych w czujniku.
    const float dt = IMU_SampleTime;

    /*! Stała używana w filtrze komplementarnym. Określa wpływ żyroskopu i akcelerometru na obliczenie orientacji.
     *  Dobrana doświadczalnie. */
    const float a =0.999;

    //! Wektor przechowujący dane z akcelerometru.
    QVector<QVector3D> data_acc;

    //! Wektor przechowujacy dane z zyroskopu.
    QVector<QVector3D> data_gyro;


};

#endif // MOTION_H
