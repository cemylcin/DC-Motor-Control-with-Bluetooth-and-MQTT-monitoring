#ifndef CURRENTMONITORFIRSTWINDOW_H
#define CURRENTMONITORFIRSTWINDOW_H


#include <QMainWindow>





#include <QDebug>
#include <QLabel>
#include <limits>
#include <QSlider>
#include <QThread>
#include <QTime>
#include <QMqttClient>


#include "bluetoothInitialization.h"
#include "currentchartwidget.h"
#include "mqttOperations.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class currentMonitorFirstWindow;
}
QT_END_NAMESPACE

class currentMonitorFirstWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit currentMonitorFirstWindow(QWidget *parent = nullptr);
    ~currentMonitorFirstWindow();

private slots:
    void connectBluetoothDevice();
    void addDeviceToComboBox(const QString &name, const QString &address);
    void onDeviceConnected();
    void onDeviceDisconnected();
    void parseBluetoothData(QByteArray data);
    void onpwmSlider_valueChanged(int value) ;
    void reverseButton_clicked();
    void forwardButton_clicked();
    void stopButton_clicked();
    void sendCommandsButton_clicked();




    /// mqtt functions
    void mqttConnectToBrokerButton() ;
    void mqttDisconnectFromBrokerButton();
    void publishMessagesButton() ;
    void subscribeToTopicButton() ;
    void isConnected();
    void isDisconnected() ;
    void handleMqttMessage(const QString &message, const QString &topic);












private:
    Ui::currentMonitorFirstWindow *ui;
    BluetoothManager *bluetooth;
    QList<QString> discoveredAddresses;
    Plotter *plotter ;
    MQTTManager *mqtt ;


    void updateSelectionPreview() ;
    void handleCurrentData(float current);
public:
    QString selectedDirection = "STOP";
    int selectedPWM = 0;

};

#endif // CURRENTMONITORFIRSTWINDOW_H
