#ifndef MQTTOPERATIONS_H
#define MQTTOPERATIONS_H

#include <QObject>
#include <QMqttClient>


class MQTTManager : public QObject
{
    Q_OBJECT

public:
    explicit MQTTManager(QObject *parent = nullptr);
    ~MQTTManager();

    void connectToBroker(const QString &host, quint16 port,
                         const QString &username = "", const QString &password = "",
                         const QString &clientId = "qt-client");

    void publishMessage(const QString &topic, const QString &message);
    void subscribeToTopic(const QString &topic);
    bool isConnected() const;
    void disconnectFromBroker() ;
signals:
    void connected();
    void disconnected();
    void messageReceived(const QString &message, const QString &topic);

private:
    QMqttClient *mqttClient;

};









#endif // MQTTOPERATIONS_H
