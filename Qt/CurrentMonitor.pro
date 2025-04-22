QT       += core gui widgets bluetooth charts texttospeech mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    bluetoothInitialization.cpp \
    currentchartwidget.cpp \
    main.cpp \
    currentmonitorfirstwindow.cpp \
    mqttOperations.cpp

HEADERS += \
    bluetoothInitialization.h \
    currentchartwidget.h \
    currentmonitorfirstwindow.h \
    mqttOperations.h

FORMS += \
    currentmonitorfirstwindow.ui

# Varsayılan deploy kuralı (opsiyonel)
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

