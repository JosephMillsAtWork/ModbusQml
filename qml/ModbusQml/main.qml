import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import com.minimoog77 1.0

ApplicationWindow {
    title: "Modbus register list model test"

    ModbusRegisterListModel {
        id: registerListModel

        modbusRegisterType: ModbusRegisterListModel.Register
        outputType: ModbusRegisterListModel.Integer32
        registerReadAddr: 0
        registerReadSize: 16
    }

    ListView {
        id: modbusRegisterListView

        anchors.fill: parent

        model: registerListModel
        delegate: Text { text: "Output: " + display }
    }

    Component.onCompleted: {
        registerListModel.connectToServer('127.0.0.1', 1502);
        registerListModel.readRegisters();
    }
}
