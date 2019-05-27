#include "xyseries.h"
#include <QtCharts/QXYSeries>

XYSeries::XYSeries(QXYSeries *series, QObject *parent)
    : QIODevice (parent), series(series)
{
}

qint64 XYSeries::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);

    return -1;
}


qint64 XYSeries::writeData(const char *data, qint64 maxSize)
{
    const int16_t* true_data = reinterpret_cast<const int16_t*>(data);
    qint64 true_maxSize = maxSize/2;

    static const int resolution = 1;
    if(buffer.isEmpty())
    {
        buffer.reserve(sampleCount);
        for(int i=0;i<sampleCount; ++i)
        {
            buffer.append(QPointF(i, 0));
        }
    }

    int start = 0;
    const int availableSamples = int(true_maxSize) / resolution;
    if(availableSamples < sampleCount)
    {
        start = sampleCount - availableSamples;
        for(int s=0; s<start; ++s)
        {
            buffer[s].setY(buffer.at(s + availableSamples).y());

        }
    }

    for(int s =start; s<sampleCount; ++s, true_data += resolution)
    {
        buffer[s].setY(qreal(*true_data));
    }
    series->replace(buffer);
    return (sampleCount-start)*resolution;
}
