#ifndef BLUETOOTHINITIALIZATION_H
#define BLUETOOTHINITIALIZATION_H

#include <QObject>
#include <QBluetoothSocket>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothUuid>

class BluetoothManager : public QObject
{
    Q_OBJECT

public:
    explicit BluetoothManager(QObject *parent = nullptr);
    ~BluetoothManager();

    void beginDeviceScan();                        // Cihazları ara
    void connectToAddress(const QString &address); // Cihaza bağlan
    void disconnectFromDevice();                   // Bağlantıyı kes
    void transmitData(const QByteArray &data);     // Veri gönder
    bool isDeviceConnected() const;                // Bağlı mı?

signals:
    void deviceFound(QString name, QString address);      // Yeni cihaz bulundu
    void deviceConnected();                               // Başarıyla bağlanıldı
    void deviceDisconnected();                            // Bağlantı koptu
    void messageReceived(QByteArray data);                // Veri alındı

private slots:
    void handleDeviceDiscovery(const QBluetoothDeviceInfo &device);  // Tarama sonucu
    void handleConnectionSuccess();                      // Bağlantı başarılı
    void handleConnectionLost();                         // Bağlantı kesildi
    void handleIncomingData();                           // Veri geldi

private:
    QBluetoothSocket *socket;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QList<QString> discoveredAddresses;
};

#endif // BLUETOOTHINITIALIZATION_H
