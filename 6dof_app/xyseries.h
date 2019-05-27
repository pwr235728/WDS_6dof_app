#ifndef XYSERIES_H
#define XYSERIES_H

#include <QIODevice>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class XYSeries : public QIODevice
{
    Q_OBJECT

public:
    explicit XYSeries(QXYSeries *series, QObject *parent = nullptr);

    static const int sampleCount = 500;

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    QXYSeries *series;
    QVector<QPointF> buffer;
};

#endif // XYSERIES_H
