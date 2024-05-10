import QtQuick
import QtQuick.Controls

Window {
    id: root
    width: 320
    height: 150

    Column {
        anchors.fill: parent
        Row {
            CheckBox {
                text: qsTr("Limit time")
                checked: backend.limitTotalTimeFlag
                onCheckStateChanged: {
                    backend.limitTotalTimeFlag = checked
                }
            }
            TextField {
                text: backend.totalTimeLimit
                inputMask: "99:99"
                inputMethodHints: Qt.ImhDigitsOnly
                //validator: RegularExpressionValidator { regularExpression: /^([0-1]?[0-9]|2[0-3]):([0-5][0-9])$/ }
                width: 100
                background: Rectangle {
                    color: "transparent"
                    border.color: "gray"
                    border.width: 2
                    radius: (width * 0.05)
                }
                onEditingFinished: {
                    backend.totalTimeLimit = text
                }
            }
            Label {
                text: qsTr("hh:mm")
            }
        }
        Row {
            CheckBox {
                text: qsTr("Start hour")
                checked: backend.limitStartTimeFlag
                onCheckStateChanged: {
                    backend.limitStartTimeFlag = checked
                }
            }
            TextField {
                id: startTimeLimitText
                text: backend.startTimeLimit
                inputMask: "99:99"
                inputMethodHints: Qt.ImhDigitsOnly
                //validator: RegularExpressionValidator { regularExpression: /^([0-1]?[0-9]|2[0-3]):([0-5][0-9])$/ }
                width: 100
                background: Rectangle {
                    color: "transparent"
                    border.color: "grey"
                    border.width: 2
                    radius: (width * 0.05)
                }
                onEditingFinished: {
                    backend.startTimeLimit = text
                }
            }
            Label {
                text: qsTr("hh:mm")
            }
        }
    }
    Button {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: "OK"
        onClicked: {
            root.close()
        }
    }
}
