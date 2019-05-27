#include "imuparser.h"

ImuFrame::ImuFrame()
    : id(0), length(0)
{
}
uchar ImuFrame::GetHeaderSum()
{
    return (id + length);
}
uchar ImuFrame::GetDataSum()
{
    uchar sum = 0;
    for(int i=0; i<length && i<256; i++)
    {
        sum += data[i];
    }
    return sum;
}
ImuParser::ImuParser()
{

}

ParserState ImuParser::ReadFrame(QByteArray &bytes, ImuFrame &out)
{
    ImuFrame frame;

    State state = STATE_WAIT;
    uchar data_index = 0;

    for(int readIndex=0; readIndex<bytes.length(); readIndex++)
    {

        uchar byte =  uchar(bytes[readIndex]);
        switch(state)
        {
        case STATE_WAIT:{
            if(byte == STATRT_BYTE){
                state = STATE_GOT_MARKER;
            }else
            {
                //qInfo("[ImmuParser::ReadFrame()]: PARSER_ERROR on STATE_WAIT");
            }
        } break;

        case STATE_GOT_MARKER:{
            frame.id = byte;
            state = STATE_GOT_ID;

        } break;

        case STATE_GOT_ID:{
            frame.length = byte;
            state = STATE_GOT_LEN;
        } break;

        case STATE_GOT_LEN:{
            if(byte == frame.GetHeaderSum())
            {
                state = (frame.length == 0 ? STATE_GOT_DATA : STATE_GOT_HEADER);
            }else{
                // some bananas happened

                //qInfo("[ImmuParser::ReadFrame()]: PARSER_ERROR on STATE_GOT_LEN");
                state = STATE_WAIT;
            }

        } break;

        case STATE_GOT_HEADER:{
            frame.data[data_index++] = byte;
            if(data_index == frame.length)
            {
                state = STATE_GOT_DATA;
            }else
            {            
                //qInfo("[ImmuParser::ReadFrame()]: PARSER_ERROR on STATE_GOT_DATA");
            }
        } break;

        case STATE_GOT_DATA:{
            state = STATE_WAIT;

            if(byte == frame.GetDataSum())
            {
                out = frame;

                bytes.remove(0, readIndex+1);
                return PARSER_COMPLETE;
            }else
            {
                //qInfo("[ImmuParser::ReadFrame()]: PARSER_ERROR (frame corrupted)");

                bytes.remove(0, readIndex+1); // remove corrupted data
                return PARSER_ERROR;
            }
        } break;
        }
    }

    //qInfo("[ImmuParser::ReadFrame()]: PARSER_ERROR (incomplete)");
    return PARSER_ERROR;
}


ParserState ImuParser::GetImuData(ImuFrame &frame, ImuData &out)
{
    if(frame.id == ID_IMU_DATA)
    {
        out.acc.x = int16_t(uint16_t(frame.data[1] << 8) + uint16_t(frame.data[0]));
        out.acc.y = int16_t(uint16_t(frame.data[3] << 8) + uint16_t(frame.data[2]));
        out.acc.z = int16_t(uint16_t(frame.data[5] << 8) + uint16_t(frame.data[4]));
        out.tmp = int16_t(frame.data[7] << 8) + int16_t(frame.data[6]); // /340 + 36.53
        out.tmp = out.tmp / 34 + 365;
        out.gyro.x = int16_t(frame.data[9] << 8) + int16_t(frame.data[8]);
        out.gyro.y = int16_t(frame.data[11] << 8) + int16_t(frame.data[10]);
        out.gyro.z = int16_t(frame.data[13] << 8) + int16_t(frame.data[12]);

        return PARSER_COMPLETE;
    }

    return PARSER_ERROR;
}


