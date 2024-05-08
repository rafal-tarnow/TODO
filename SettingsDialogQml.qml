import QtQuick
import QtQuick.Controls

Window {
    id: root
    width: 100
    height: 100

    Column {
        Row {
            CheckBox {
                text: qsTr("Limit time")
            }
            TextEdit {}
        }
        Row {
            CheckBox {
                text: qsTr("Start hour")
            }
        }
    }
}
