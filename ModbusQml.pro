# Add more folders to ship with the application, here
folder_01.source = qml/ModbusQml
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

qtHaveModule(widgets) {
    QT += widgets
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    thirdparty/libmodbus/src/modbus.c \
    thirdparty/libmodbus/src/modbus-tcp.c \
    thirdparty/libmodbus/src/modbus-rtu.c \
    thirdparty/libmodbus/src/modbus-data.c \
    modbusregisterlistmodel.cpp

HEADERS += thirdparty/libmodbus/src/modbus.h \
    thirdparty/libmodbus/src/modbus-version.h \
    thirdparty/libmodbus/src/modbus-tcp.h \
    thirdparty/libmodbus/src/modbus-tcp-private.h \
    thirdparty/libmodbus/src/modbus-rtu.h \
    thirdparty/libmodbus/src/modbus-rtu-private.h \
    thirdparty/libmodbus/src/modbus-private.h \
    modbusregisterlistmodel.h

INCLUDEPATH += thirdparty/libmodbus

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()
