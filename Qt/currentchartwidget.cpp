#include "currentchartwidget.h"

Plotter::Plotter(QWidget *parent) :
    QWidget(parent)
{
    series = new QLineSeries();

    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Current over Time");

    axisX = new QDateTimeAxis();
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Time");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setRange(0.05, 0.12);  // Başlangıç aralığı (50mA – 120mA)
    axisY->setTitleText("Current (A)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    this->setLayout(layout);

    minValue = 9999.0;
    maxValue = -9999.0;
}

void Plotter::addData(double value)
{
    qDebug() << "[PLOTTER] AddData() çalıştı - Value:" << value;

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    QPointF point(currentTime, value);

    dataList.append(point);
    series->append(point);

    qDebug() << "[PLOTTER] Timestamp (ms):" << currentTime
             << "| Series Count:" << series->count();

    // Min/Max güncelle
    if (value < minValue) minValue = value;
    if (value > maxValue) maxValue = value;

    // Min/Max yazısı
    if (infoLabel) {
        infoLabel->setText(QString("Min: %1 A | Max: %2 A")
                               .arg(minValue, 0, 'f', 3)
                               .arg(maxValue, 0, 'f', 3));
    }

    // Y ekseni aralığını dinamik olarak ayarla
    axisY->setRange(minValue - 0.01, maxValue + 0.01);  // örn: 0.06 – 0.11

    // X ekseni: son 10 saniyeyi göster
    if (series->count() > 1) {
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        axisX->setRange(QDateTime::fromMSecsSinceEpoch(now - 10000),
                        QDateTime::fromMSecsSinceEpoch(now));
    }

    chart->update();
}

void Plotter::setInfoLabel(QLabel *label)
{
    infoLabel = label;
}

Plotter::~Plotter() {}
