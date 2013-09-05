import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1

ApplicationWindow {

    Rectangle {
        width: 360
        height: 360
        Text {
            text: qsTr("Hello World")
            anchors.centerIn: parent
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                Qt.quit();
            }
        }
    }
}
