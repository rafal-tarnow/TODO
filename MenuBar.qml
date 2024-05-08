import QtQuick
import QtQuick.Controls
import "."

Item {
    anchors.fill: parent
    Button {
        anchors.fill: parent
        text: qsTr("Settings")
        onClicked: {
            backend.showSettings()
            settingsDialogQml.show()
        }
    }
    SettingsDialogQml {
        id: settingsDialogQml
    }
}
