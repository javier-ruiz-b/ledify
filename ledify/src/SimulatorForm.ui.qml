import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

Pane {
    property var ledColors: lightRepeater
    property var tfCommand: tfCommand
    ColumnLayout {
        spacing: 16
        anchors.centerIn: parent
        Row {
            spacing: 2
            Material.elevation: 6
            Repeater {
                id: lightRepeater
                model: 32

                Rectangle {
                    x: 0
                    width: 24
                    height: 24
                    color: "#000"
                }
            }
        }

        TextField {
            id: tfCommand
            text: qsTr("")
            anchors.left: parent.left
            anchors.right: parent.right
            font.weight: Font.Bold
            placeholderText: "Command"
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
