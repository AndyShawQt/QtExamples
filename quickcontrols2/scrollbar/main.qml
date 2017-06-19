// 1109518
// Demostrates how to get original style scrollbars in Qt Quick Controls 2

import QtQuick.Controls 2.2
import QtQuick 2.7

ApplicationWindow
{
    id: window
    visible: true
    Item {
        anchors.fill: parent
        Flickable {
            id: flick
            anchors.fill: parent
            contentHeight: frame.height
            contentWidth: frame.width
            Rectangle {
                id: frame
                width: 1000
                height: 1000
                border.color: "black"
                border.width: 5
                color: "red"
            }
        }
        Button {
            id: decreaseVButton
            text: "<"
            rotation: 90
            height: 16
            width: vbar.width + 4
            anchors.top: flick.top
            anchors.right: flick.right
            onClicked: vbar.decrease();
        }
        Button {
            id: increaseVButton
            text: ">"
            rotation: 90
            height: 16
            width: vbar.width + 4
            y: flick.height - hbar.height - height - 4
            anchors.right: flick.right
            onClicked: vbar.increase();
        }
        ScrollBar {
            id: vbar
            active: true
            orientation: Qt.Vertical
            size: flick.height / frame.height
            anchors.top: decreaseVButton.bottom
            anchors.bottom: increaseVButton.top
            anchors.right: flick.right
            onActiveChanged: active = true
            onPositionChanged: flick.contentY = flick.contentWidth * position;
        }
        Button {
            id: decreaseHButton
            text: "<"
            width: 16
            height: hbar.height + 4
            anchors.bottom: flick.bottom
            anchors.left: flick.left
            onClicked: hbar.decrease();
        }
        Button {
            id: increaseHButton
            text: ">"
            width: 16
            height: hbar.height + 4
            anchors.bottom: flick.bottom
            x: flick.width - vbar.width - width - 4
            onClicked: hbar.increase();
        }
        ScrollBar {
            id: hbar
            active: true
            orientation: Qt.Horizontal
            size: flick.width / frame.width
            anchors.left: decreaseHButton.right
            anchors.right: increaseHButton.left
            anchors.bottom: flick.bottom
            onActiveChanged: active = true
            onPositionChanged: flick.contentX = flick.contentWidth * position;
        }
    }
}

