import QtQuick
import QtQuick.Controls
import Blippy.Theme 1.0

Item {
    id: navItem
    property color iconColor
    property string text
    property string page
    property bool sidebarOpen: true
    property string selectedPage: ""

    readonly property bool isSelected: selectedPage === page

    height: 44
    width: parent.width

    signal clicked()

    Rectangle {
        id: bg
        anchors.fill: parent
        radius: 10
        color: isSelected ? Theme.current.accent : "transparent"
        border.color: isSelected ? Theme.current.accent : "transparent"

        Behavior on color {
            ColorAnimation { duration: 200; easing.type: Easing.OutCubic }
        }
        Behavior on border.color {
            ColorAnimation { duration: 200; easing.type: Easing.OutCubic }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: sidebarOpen ? 12 : 0

        // Icon - colored rectangle instead of SVG
        Rectangle {
            id: icon
            width: 22
            height: 22
            radius: 6
            color: isSelected ? Theme.current.onAccent : iconColor

            Behavior on color {
                ColorAnimation { duration: 200 }
            }
        }

        // Text
        Text {
            id: label
            Layout.fillWidth: true
            visible: sidebarOpen
            text: text
            font.pixelSize: 14
            font.weight: isSelected ? Font.Medium : Font.Normal
            color: isSelected ? Theme.current.onAccent : Theme.current.textPrimary
            opacity: sidebarOpen ? 1 : 0

            Behavior on opacity {
                NumberAnimation { duration: 150 }
            }
            Behavior on color {
                ColorAnimation { duration: 200 }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: navItem.clicked()

        onEntered: {
            if (!isSelected) bg.color = Theme.current.cardBg
        }
        onExited: {
            if (!isSelected) bg.color = "transparent"
        }
    }
}