#ifndef IMUPARSER_H
#define IMUPARSER_H

#include   <QSerialPort>

struct Vector3
{
    int16_t x;
    int16_t y;
    int16_t z;
};

struct ImuData
{
    Vector3 acc;
    Vector3 gyro;
    float tmp;
};

struct ImuFrame
{
    uchar id;
    uchar length;
    uchar data[256];

    uchar GetHeaderSum();
    uchar GetDataSum();
    ImuFrame();
};

enum ParserState
{
    PARSER_ERROR = 0,
    PARSER_COMPLETE,
};

class ImuParser
{
private:
    enum State{
        STATE_WAIT,
        STATE_GOT_MARKER,
        STATE_GOT_ID,
        STATE_GOT_LEN,
        STATE_GOT_HEADER,
        STATE_GOT_DATA
    };

    ImuParser();

public:
    static const char STATRT_BYTE = '>';
    static const char ID_IMU_DATA = 'i';

    static ParserState ReadFrame(QByteArray &bytes, ImuFrame &out);
    static ParserState GetImuData(ImuFrame &frame, ImuData &out);
};

#endif // IMUPARSER_H
