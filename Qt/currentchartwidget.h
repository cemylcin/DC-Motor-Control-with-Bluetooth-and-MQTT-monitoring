#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QDateTime>
#include <QPainter>
#include <QBoxLayout>
#include <QLabel>
#include <limits>


QT_BEGIN_NAMESPACE
namespace Ui {
class Plotter;
}
QT_END_NAMESPACE



class Plotter : public QWidget
{
    Q_OBJECT

public:
    explicit Plotter(QWidget *parent = nullptr);
    ~Plotter();

    void addData(double value);
        void setInfoLabel(QLabel *label);
private:

    QLabel *infoLabel = nullptr;
    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::min();
    QList<QPointF> dataList;

    QChart *chart;
    QLineSeries *series;
    QValueAxis *axisY;
    QDateTimeAxis *axisX;
};

#endif // PLOTTER_H

