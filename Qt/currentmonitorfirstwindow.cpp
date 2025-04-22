#include "currentmonitorfirstwindow.h"
#include "ui_currentmonitorfirstwindow.h"

#include <QRandomGenerator>
#include "mqttOperations.h"


currentMonitorFirstWindow::currentMonitorFirstWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::currentMonitorFirstWindow)
{
    ui->setupUi(this);

    bluetooth = new BluetoothManager(this);
    plotter = new Plotter(this);
    mqtt = new MQTTManager(this);

    this->setWindowTitle("Bluetooth Motor Control & MQTT Monitor");


    ui->chartWidget->setLayout(new QVBoxLayout());
    ui->chartWidget->layout()->addWidget(plotter);
    plotter->setInfoLabel(ui->labelMinMax);

    ui->motorStatusLabel->setText("⚪ Motor is idle");
    ui->bluetoothStatusLabel->setText("⚪ Bluetooth is ready for connection");
    ui->bluetoothTextEdit->moveCursor(QTextCursor::End);

    // MQTT

   ui->mqttStatusLabel->setText("⚪ MQTT status: Not connected");
   ui->topicPublish->setPlaceholderText("Enter topic name");
   ui->topicSubscribe->setPlaceholderText("Enter topic name");
   ui->transmittedPublishTextEdit->setPlaceholderText("Sent messages can be viewed here");
   ui->publishLineEdit->setPlaceholderText("Enter the message to publish");
   ui->receivedTextEdit->setPlaceholderText("Received messages will be visible here!");
   ui->hostLineEdit->setPlaceholderText("e.g. broker.hivemq.com");
   ui->portLineEdit->setPlaceholderText("e.g. 1883 or 8883 for TLS");
   ui->usernameLineEdit->setPlaceholderText("Enter your MQTT username");
   ui->passwordLineEdit->setPlaceholderText("Enter your MQTT password");
   ui->clientID->setPlaceholderText("Unique ID for this client");
   ui->bluetoothTextEdit->setPlaceholderText("Motor control logs and Bluetooth messages will appear here...");
   ui->alarmTextEdit->setPlaceholderText("Real-time current data will be displayed here...");


    //// TOOLTIPS
    ui->reverseButton->setToolTip("Stops the motor first and then rotates the motor counterclockwise.") ;
    ui->forwardButton->setToolTip("Rotates the motor clockwise");
    ui->stopButton->setToolTip("Stops the motor (PWM=0 , IN PINs=0)");


    // UI butonları BluetoothManager fonksiyonlarına bağlandı
    connect(ui->scanBluetoothButton, &QPushButton::clicked, bluetooth, &BluetoothManager::beginDeviceScan);
    connect(ui->connectToDeviceButton, &QPushButton::clicked, this, &currentMonitorFirstWindow::connectBluetoothDevice);
    connect(ui->disconnectFromDeviceButton, &QPushButton::clicked, bluetooth, &BluetoothManager::disconnectFromDevice);

    // BluetoothManager sinyalleri GUI'ye bağlandı
    connect(bluetooth, &BluetoothManager::deviceFound, this, &currentMonitorFirstWindow::addDeviceToComboBox);
    connect(bluetooth, &BluetoothManager::deviceConnected, this, &currentMonitorFirstWindow::onDeviceConnected);
    connect(bluetooth, &BluetoothManager::deviceDisconnected, this, &currentMonitorFirstWindow::onDeviceDisconnected);
    connect(bluetooth, &BluetoothManager::messageReceived, this, &currentMonitorFirstWindow::parseBluetoothData);


    // MOTOR CONTROL CONNECTS


    connect(ui->pwmSlider, &QSlider::valueChanged, this, &currentMonitorFirstWindow::onpwmSlider_valueChanged);
    connect(ui->pwmSlider, &QSlider::valueChanged, ui->pwmSpinBox, &QSpinBox::setValue);
    connect(ui->pwmSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), ui->pwmSlider, &QSlider::setValue);
    // forward reverse stop
    connect(ui->forwardButton, &QPushButton::clicked, this, &currentMonitorFirstWindow::forwardButton_clicked);
    connect(ui->reverseButton, &QPushButton::clicked,this, &currentMonitorFirstWindow::reverseButton_clicked);
    connect(ui->stopButton, &QPushButton::clicked, this, &currentMonitorFirstWindow::stopButton_clicked);
    connect(ui->sendCommandsButton, &QPushButton::clicked, this, &currentMonitorFirstWindow::sendCommandsButton_clicked);


     /// mqtt connect
    connect(ui->connectMQTTButton, &QPushButton::clicked, this, &currentMonitorFirstWindow::mqttConnectToBrokerButton);
    connect(ui->disconnectMQTTButton,&QPushButton::clicked, this, &currentMonitorFirstWindow::mqttDisconnectFromBrokerButton);
    connect(ui->publishButton, &QPushButton::clicked, this, &currentMonitorFirstWindow::publishMessagesButton);
    connect(ui->subscribeButton, &QPushButton::clicked, this, &currentMonitorFirstWindow::subscribeToTopicButton);


    connect(mqtt,&MQTTManager::connected,this,&currentMonitorFirstWindow::isConnected) ;
    connect(mqtt,&MQTTManager::disconnected,this,&currentMonitorFirstWindow::isDisconnected) ;
    connect(mqtt, &MQTTManager::messageReceived, this,&currentMonitorFirstWindow::handleMqttMessage);


}

void currentMonitorFirstWindow::isConnected()
{
    ui->mqttStatusLabel->setText("🟢 MQTT Connected");
}




void currentMonitorFirstWindow::isDisconnected()
{
    ui->mqttStatusLabel->setText("🔴 MQTT Not Connected");
}



void currentMonitorFirstWindow::mqttConnectToBrokerButton()
{

 QString hostName =   ui->hostLineEdit->text() ;
 quint16 portName =   ui->portLineEdit->text().toUShort();
 QString userName =   ui->usernameLineEdit->text();
 QString password =   ui->passwordLineEdit->text();
 QString clientID =   ui->clientID->text() ;

 mqtt->connectToBroker(hostName,portName,userName,password,clientID);

}



void currentMonitorFirstWindow::mqttDisconnectFromBrokerButton()
{
    mqtt->disconnectFromBroker();
}

void currentMonitorFirstWindow::subscribeToTopicButton()
{
    QString topictobeSubscribed = ui->topicSubscribe->text();
    mqtt->subscribeToTopic(topictobeSubscribed);
}

void currentMonitorFirstWindow::handleMqttMessage(const QString &message, const QString &topic)
{
    QString formatted = QString("[%1] Message: %2 Topic: %3").arg(QTime::currentTime().toString("HH:mm:ss")).arg(message, topic);
    ui->receivedTextEdit->append(formatted);
}



void currentMonitorFirstWindow::publishMessagesButton()
{

    QString message_to_publish = ui->publishLineEdit->text();
    QString topic_to_publish = ui->topicPublish->text() ;
    mqtt->publishMessage(topic_to_publish,message_to_publish);

    QString message_topic_to_TextEdit = QString("[%1] Message: %2 Topic: %3").arg(QTime::currentTime().toString("HH:mm:ss")).arg(message_to_publish).arg(topic_to_publish);
    ui->transmittedPublishTextEdit->append(message_topic_to_TextEdit);

}

void currentMonitorFirstWindow::forwardButton_clicked()
{
    selectedDirection = "FORWARD";
    updateSelectionPreview();
}

void currentMonitorFirstWindow::reverseButton_clicked()
{
    selectedDirection = "REVERSE";
    updateSelectionPreview();
}

void currentMonitorFirstWindow::stopButton_clicked()
{
    selectedDirection = "STOP";
    selectedPWM = 0 ;
    updateSelectionPreview();
}

void currentMonitorFirstWindow::onpwmSlider_valueChanged(int value)
{
    selectedPWM = value;
    updateSelectionPreview();
}
void currentMonitorFirstWindow::updateSelectionPreview()
{
    ui->directionLabel->setText("Direction: " + selectedDirection);
    if(selectedDirection == "STOP" )
    {
        ui->pwmValueLabel->setText(QString("PWM: 0"));

    }else {
        ui->pwmValueLabel->setText(QString("PWM: %1").arg(selectedPWM));
    }

}

void currentMonitorFirstWindow::sendCommandsButton_clicked()
{
    // Örnek: "PWM:50\nFORWARD\n"
    QString command = QString("PWM:%1\n%2\n").arg(selectedPWM).arg(selectedDirection);
    bluetooth->transmitData(command.toUtf8());




    QString directionColor = (selectedDirection == "FORWARD") ? "green" :
                                 (selectedDirection == "REVERSE") ? "orange" :
                                 "red";

    QString logEntry = QString("[%1] <font color='%2'>📤 Sent → Direction: %3</font> | PWM: %4")
                           .arg(QTime::currentTime().toString("HH:mm:ss"))
                           .arg(directionColor)
                           .arg(selectedDirection)
                           .arg(selectedPWM);
    ui->bluetoothTextEdit->append(logEntry) ;

    // Durumu motor status kısmında da güncelle:
    if (selectedDirection == "FORWARD") {
        ui->motorStatusLabel->setText("🟢 Motor is running forward");
    } else if (selectedDirection == "REVERSE") {
        ui->motorStatusLabel->setText("🟡 Motor is running reverse");
    } else {
        ui->motorStatusLabel->setText("🔴 Motor is stopped");
    }
}




void currentMonitorFirstWindow::parseBluetoothData(QByteArray data)
{
    QString receivedData = QString::fromUtf8(data).trimmed();

    // 🔍 LOG - Geliştirme sırasında görebilirsin
    qDebug() << "📥 Bluetooth data received:" << receivedData;

    if (receivedData.startsWith("Current:")) {
        QString valueStr = receivedData.section(":", 1).trimmed();
        bool ok = false;
        float current = valueStr.toFloat(&ok);

        if (ok) {
            handleCurrentData(current);
        } else {
            ui->alarmTextEdit->append("❌ Parse failed: " + valueStr);
        }
    } else {
        // Gereksiz tekrarı azaltmak için aynı veri tekrar geldiyse göstermeyebilirsin
        ui->alarmTextEdit->append("❗ Unknown Data: " + receivedData);
    }
}


void currentMonitorFirstWindow::handleCurrentData(float current)
{
    // 📊 Grafik güncelle
    plotter->addData(current);

    // 📄 Alarm paneline yaz
    QString displayText = QString("[%1] Current: %2 A").arg(QTime::currentTime().toString("HH:mm:ss")).arg(current, 0, 'f', 3);
    ui->alarmTextEdit->append(displayText);

    // İsteğe bağlı: çok yüksekse uyarı da verebiliriz
    if (current > 1.5f) {
        ui->alarmTextEdit->append("<span style='color:red;'>🚨 HIGH CURRENT!</span>");
    }
}









void currentMonitorFirstWindow::connectBluetoothDevice()
{
    int index = ui->devicesComboBox->currentIndex();
    if (index < 0 || index >= discoveredAddresses.size()) {
        qWarning() << "⚠️ Geçerli cihaz yok.";
        return;
    }

    QString address = discoveredAddresses.at(index);
    bluetooth->connectToAddress(address);  // ✅ Güncellenmiş fonksiyon adı
}

void currentMonitorFirstWindow::addDeviceToComboBox(const QString &name, const QString &address)
{
    QString label = name + " - " + address;
    ui->devicesComboBox->addItem(label);
    discoveredAddresses.append(address);
}

void currentMonitorFirstWindow::onDeviceConnected()
{
    QString label = ui->devicesComboBox->currentText();
    QString labelBluetooth =QString("✅ Connected to: %1").arg(label) ;
    ui->bluetoothTextEdit->append(labelBluetooth);
    ui->bluetoothStatusLabel->setText("🟢 Bluetooth is connected");

}

void currentMonitorFirstWindow::onDeviceDisconnected()
{
    QString label = ui->devicesComboBox->currentText();
    QString labelBluetooth =QString("❌ Disconnected from: %1").arg(label) ;
    ui->bluetoothTextEdit->append(labelBluetooth);
    ui->bluetoothStatusLabel->setText("🔴 Bluetooth is disconnected");

}

currentMonitorFirstWindow::~currentMonitorFirstWindow()
{
    delete ui;
}
