#include "mqttOperations.h"
#include <QSslConfiguration>
#include <QThread>


MQTTManager::MQTTManager(QObject *parent)
    : QObject(parent)
{
    mqttClient = new QMqttClient(this);

    connect(mqttClient, &QMqttClient::connected, this, [=]() {
        qDebug() << "✅ MQTT bağlantısı başarılı";
        emit connected();
    });

    connect(mqttClient, &QMqttClient::disconnected, this, [=]() {
        qDebug() << "🔌 MQTT bağlantısı kesildi";
        emit disconnected();
    });

    connect(mqttClient, &QMqttClient::messageReceived, this, [=](const QByteArray &message, const QMqttTopicName &topic) {
        emit messageReceived(QString::fromUtf8(message), topic.name());
    });
}

MQTTManager::~MQTTManager()
{
    if (mqttClient->state() == QMqttClient::Connected)
        mqttClient->disconnectFromHost();
    delete mqttClient;
}

void MQTTManager::connectToBroker(const QString &host, quint16 port,
                                  const QString &username, const QString &password,
                                  const QString &clientId)


{


    if (mqttClient->state() == QMqttClient::Connected) {
        qDebug() << "ℹ️ MQTT zaten bağlı. Yeniden bağlanmadan önce bağlantıyı kes.";
        return;
    }


    mqttClient->setHostname(host);
    mqttClient->setPort(port);
    mqttClient->setUsername(username);
    mqttClient->setPassword(password);
    mqttClient->setClientId(clientId);

    if (mqttClient->state() == QMqttClient::Connected) {
        mqttClient->disconnectFromHost();
        QThread::msleep(100);
    }
    QSslConfiguration sslConf = QSslConfiguration::defaultConfiguration();
    mqttClient->connectToHostEncrypted(sslConf);
}
void MQTTManager::disconnectFromBroker()
{
    mqttClient->disconnectFromHost();
}

void MQTTManager::publishMessage(const QString &topic, const QString &message)
{
    if (mqttClient->state() == QMqttClient::Connected) {
        mqttClient->publish(QMqttTopicName(topic), message.toUtf8());
        qDebug() << "📤 MQTT Yayınlandı →" << topic << ":" << message;
    } else {
        qDebug() << "❌ MQTT Bağlı değil, mesaj gönderilemedi.";
    }
}

void MQTTManager::subscribeToTopic(const QString &topic)
{
    if (!mqttClient || mqttClient->state() != QMqttClient::Connected) {
        qDebug() << "❌ MQTT bağlantısı yok, subscribe başarısız.";
        return;
    }

    auto *subscription = mqttClient->subscribe(QMqttTopicFilter(topic), 0);
    if (!subscription) {
        qDebug() << "⚠️ Subscribe işlemi başarısız oldu → Topic:" << topic;
    } else {
        qDebug() << "✅ Subscribed to topic:" << topic;
    }
}



bool MQTTManager::isConnected() const
{
    return mqttClient->state() == QMqttClient::Connected;
}
