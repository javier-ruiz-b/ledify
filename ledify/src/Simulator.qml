import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.1
import ledify.simulator 1.0


ApplicationWindow {
    id: appWindow
    visible: true

    width: 1900
    height: 200

    title: "Ledify Simulator"

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    SimulatorForm {
        id: root
        anchors.fill: parent

        Keys.onReturnPressed: {
            simulator.sendCommand(tfCommand.text)
            tfCommand.text = ""
        }

    }


    function toHex(d) {
        return  ("0"+(Number(d).toString(16))).slice(-2).toUpperCase()
    }


    SimulatorController {
        id: simulator
        onLedDataChanged: {
            var ledData = simulator.ledData
            var ledColors = root.ledColors
            for (var i = 0; (i < ledData.length) &&
                            (i < ledColors.model); i++) {
                var value = ledData[i]
                var r = toHex(value >> 16)
                var g = toHex((value >> 8) & 0xFF)
                var b = toHex(value & 0xFF)
                var rgb = "FF" + r + g + b;
//                console.log("[" + i + "] = #" + rgb)
                ledColors.itemAt(i).color = "#" + rgb
            }
        }
    }
}
