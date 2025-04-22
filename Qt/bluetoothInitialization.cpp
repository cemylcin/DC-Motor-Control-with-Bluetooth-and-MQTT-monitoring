#include "bluetoothInitialization.h"
#include <QDebug>

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject(parent)
{
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothManager::handleDeviceDiscovery);

    connect(socket, &QBluetoothSocket::connected,
            this, &BluetoothManager::handleConnectionSuccess);

    connect(socket, &QBluetoothSocket::disconnected,
            this, &BluetoothManager::handleConnectionLost);

    connect(socket, &QBluetoothSocket::readyRead,
            this, &BluetoothManager::handleIncomingData);
}

BluetoothManager::~BluetoothManager()
{
    if (socket) {
        if (socket->isOpen())
            socket->disconnectFromService();
        socket->deleteLater();
        socket = nullptr;
    }

    if (discoveryAgent) {
        discoveryAgent->stop();
        discoveryAgent->deleteLater();
        discoveryAgent = nullptr;
    }

    qDebug() << "🧹 BluetoothManager nesnesi yok edildi.";
}

void BluetoothManager::beginDeviceScan()
{
    discoveryAgent->start();
    qDebug() << "🔍 Cihaz taraması başlatıldı.";
}

void BluetoothManager::connectToAddress(const QString &addressStr)
{
    QBluetoothAddress address(addressStr);
    QBluetoothUuid uuid(QStringLiteral("00001101-0000-1000-8000-00805F9B34FB"));

    socket->connectToService(address, uuid);
    qDebug() << "🔗 Cihaza bağlanılıyor:" << addressStr;
}

void BluetoothManager::disconnectFromDevice()
{
    if (socket && socket->isOpen()) {
        socket->disconnectFromService();
        qDebug() << "🔌 Bağlantı kesildi.";
    } else {
        qDebug() << "ℹ️ Zaten bağlı değil.";
    }
}

void BluetoothManager::transmitData(const QByteArray &data)
{
    if (socket && socket->isOpen()) {
        socket->write(data);
    } else {
        qDebug() << "❌ Bağlantı yok, veri gönderilemedi.";
    }
}

bool BluetoothManager::isDeviceConnected() const
{
    return socket && socket->isOpen();
}

void BluetoothManager::handleDeviceDiscovery(const QBluetoothDeviceInfo &device)
{
    emit deviceFound(device.name(), device.address().toString());
}

void BluetoothManager::handleConnectionSuccess()
{
    emit deviceConnected();
    qDebug() << "✅ Bağlantı başarılı.";
}

void BluetoothManager::handleConnectionLost()
{
    emit deviceDisconnected();
    qDebug() << "🔌 Bağlantı kesildi.";
}

void BluetoothManager::handleIncomingData()
{
    QByteArray data = socket->readAll();
    qDebug() << "📥 Gelen veri:" << data;
    emit messageReceived(data);
}
