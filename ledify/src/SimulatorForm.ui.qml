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
            spacing: 0
            Repeater {
                id: lightRepeater
                model: 300

                Rectangle {
                    x: 0
                    width: 6
                    height: 16
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
