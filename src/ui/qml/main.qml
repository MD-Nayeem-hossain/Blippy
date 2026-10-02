import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Blippy.Theme 1.0

// Blippy Main Window - Fluid, GenZ-inspired UI
ApplicationWindow {
    id: window
    visible: true
    width: 1000
    height: 700
    minimumWidth: 900
    minimumHeight: 600
    title: "Blippy"
    color: Theme.current.background

    property bool sidebarOpen: true

    readonly property real sidebarWidth: sidebarOpen ? 220 : 60

    // Content area with Loader for page switching
    Loader {
        id: pageLoader
        anchors.fill: parent
        anchors.leftMargin: sidebarWidth
        source: "pages/dashboard.qml"
    }

    // Sidebar
    Rectangle {
        id: sidebar
        width: sidebarWidth
        height: parent.height
        color: Theme.current.surface
        border.color: Theme.current.border
        border.width: 1

        Behavior on width {
            NumberAnimation { duration: 250; easing.type: Easing.OutCubic }
        }

        Column {
            id: sidebarContent
            spacing: 4
            anchors.margins: 8

            // Logo / App name
            RowLayout {
                id: logoRow
                width: parent.width
                spacing: 12
                Layout.fillWidth: true

                Rectangle {
                    width: 28
                    height: 28
                    radius: 6
                    color: Theme.current.accent
                }

                Text {
                    id: appTitle
                    text: "Blippy"
                    font.pixelSize: 18
                    font.weight: Font.Bold
                    color: Theme.current.accent
                    opacity: sidebarOpen ? 1 : 0
                    visible: sidebarOpen

                    Behavior on opacity {
                        NumberAnimation { duration: 150 }
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: Theme.current.border
                opacity: sidebarOpen ? 1 : 0
                Behavior on opacity { NumberAnimation { duration: 150 } }
            }

            // Navigation items - inline rectangles
            Column {
                id: navColumn
                spacing: 2

                // Dashboard
                Rectangle {
                    id: navDashboard
                    property string page: "dashboard"
                    property bool isSelected: pageLoader.item && pageLoader.item.page === page
                    height: 44
                    width: parent.width
                    radius: 10
                    color: isSelected ? Theme.current.accent : "transparent"
                    border.color: isSelected ? Theme.current.accent : "transparent"
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: sidebarOpen ? 12 : 0

                        Rectangle {
                            width: 22
                            height: 22
                            radius: 6
                            color: isSelected ? Theme.current.onAccent : Theme.current.accent
                        }

                        Text {
                            Layout.fillWidth: true
                            visible: sidebarOpen
                            text: "Dashboard"
                            font.pixelSize: 14
                            font.weight: isSelected ? Font.Medium : Font.Normal
                            color: isSelected ? Theme.current.onAccent : Theme.current.textPrimary
                            opacity: sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: pageLoader.source = "pages/dashboard.qml"
                        onEntered: { if (!isSelected) parent.color = Theme.current.cardBg }
                        onExited: { if (!isSelected) parent.color = "transparent" }
                    }
                }

                // Snippets
                Rectangle {
                    id: navSnippets
                    property string page: "snippets"
                    property bool isSelected: pageLoader.item && pageLoader.item.page === page
                    height: 44
                    width: parent.width
                    radius: 10
                    color: isSelected ? Theme.current.accent : "transparent"
                    border.color: isSelected ? Theme.current.accent : "transparent"
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: sidebarOpen ? 12 : 0

                        Rectangle {
                            width: 22
                            height: 22
                            radius: 6
                            color: isSelected ? Theme.current.onAccent : Theme.current.accent
                        }

                        Text {
                            Layout.fillWidth: true
                            visible: sidebarOpen
                            text: "Snippets"
                            font.pixelSize: 14
                            font.weight: isSelected ? Font.Medium : Font.Normal
                            color: isSelected ? Theme.current.onAccent : Theme.current.textPrimary
                            opacity: sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: pageLoader.source = "pages/snippets.qml"
                        onEntered: { if (!isSelected) parent.color = Theme.current.cardBg }
                        onExited: { if (!isSelected) parent.color = "transparent" }
                    }
                }

                // Placeholders
                Rectangle {
                    id: navPlaceholders
                    property string page: "placeholders"
                    property bool isSelected: pageLoader.item && pageLoader.item.page === page
                    height: 44
                    width: parent.width
                    radius: 10
                    color: isSelected ? Theme.current.accent : "transparent"
                    border.color: isSelected ? Theme.current.accent : "transparent"
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: sidebarOpen ? 12 : 0

                        Rectangle {
                            width: 22
                            height: 22
                            radius: 6
                            color: isSelected ? Theme.current.onAccent : Theme.current.accent
                        }

                        Text {
                            Layout.fillWidth: true
                            visible: sidebarOpen
                            text: "Placeholders"
                            font.pixelSize: 14
                            font.weight: isSelected ? Font.Medium : Font.Normal
                            color: isSelected ? Theme.current.onAccent : Theme.current.textPrimary
                            opacity: sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: pageLoader.source = "pages/placeholders.qml"
                        onEntered: { if (!isSelected) parent.color = Theme.current.cardBg }
                        onExited: { if (!isSelected) parent.color = "transparent" }
                    }
                }

                // Glossary
                Rectangle {
                    id: navGlossary
                    property string page: "glossary"
                    property bool isSelected: pageLoader.item && pageLoader.item.page === page
                    height: 44
                    width: parent.width
                    radius: 10
                    color: isSelected ? Theme.current.accent : "transparent"
                    border.color: isSelected ? Theme.current.accent : "transparent"
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: sidebarOpen ? 12 : 0

                        Rectangle {
                            width: 22
                            height: 22
                            radius: 6
                            color: isSelected ? Theme.current.onAccent : Theme.current.accent
                        }

                        Text {
                            Layout.fillWidth: true
                            visible: sidebarOpen
                            text: "Glossary"
                            font.pixelSize: 14
                            font.weight: isSelected ? Font.Medium : Font.Normal
                            color: isSelected ? Theme.current.onAccent : Theme.current.textPrimary
                            opacity: sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: pageLoader.source = "pages/glossary.qml"
                        onEntered: { if (!isSelected) parent.color = Theme.current.cardBg }
                        onExited: { if (!isSelected) parent.color = "transparent" }
                    }
                }

                // Backup
                Rectangle {
                    id: navBackup
                    property string page: "backup"
                    property bool isSelected: pageLoader.item && pageLoader.item.page === page
                    height: 44
                    width: parent.width
                    radius: 10
                    color: isSelected ? Theme.current.accent : "transparent"
                    border.color: isSelected ? Theme.current.accent : "transparent"
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: sidebarOpen ? 12 : 0

                        Rectangle {
                            width: 22
                            height: 22
                            radius: 6
                            color: isSelected ? Theme.current.onAccent : Theme.current.accent
                        }

                        Text {
                            Layout.fillWidth: true
                            visible: sidebarOpen
                            text: "Backup"
                            font.pixelSize: 14
                            font.weight: isSelected ? Font.Medium : Font.Normal
                            color: isSelected ? Theme.current.onAccent : Theme.current.textPrimary
                            opacity: sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: pageLoader.source = "pages/backup.qml"
                        onEntered: { if (!isSelected) parent.color = Theme.current.cardBg }
                        onExited: { if (!isSelected) parent.color = "transparent" }
                    }
                }

                // Import
                Rectangle {
                    id: navImport
                    property string page: "import"
                    property bool isSelected: pageLoader.item && pageLoader.item.page === page
                    height: 44
                    width: parent.width
                    radius: 10
                    color: isSelected ? Theme.current.accent : "transparent"
                    border.color: isSelected ? Theme.current.accent : "transparent"
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: sidebarOpen ? 12 : 0

                        Rectangle {
                            width: 22
                            height: 22
                            radius: 6
                            color: isSelected ? Theme.current.onAccent : Theme.current.accent
                        }

                        Text {
                            Layout.fillWidth: true
                            visible: sidebarOpen
                            text: "Import"
                            font.pixelSize: 14
                            font.weight: isSelected ? Font.Medium : Font.Normal
                            color: isSelected ? Theme.current.onAccent : Theme.current.textPrimary
                            opacity: sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: pageLoader.source = "pages/import.qml"
                        onEntered: { if (!isSelected) parent.color = Theme.current.cardBg }
                        onExited: { if (!isSelected) parent.color = "transparent" }
                    }
                }

                // Settings
                Rectangle {
                    id: navSettings
                    property string page: "settings"
                    property bool isSelected: pageLoader.item && pageLoader.item.page === page
                    height: 44
                    width: parent.width
                    radius: 10
                    color: isSelected ? Theme.current.accent : "transparent"
                    border.color: isSelected ? Theme.current.accent : "transparent"
                    Behavior on color { ColorAnimation { duration: 200 } }
                    Behavior on border.color { ColorAnimation { duration: 200 } }
                    Layout.fillWidth: true

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: sidebarOpen ? 12 : 0

                        Rectangle {
                            width: 22
                            height: 22
                            radius: 6
                            color: isSelected ? Theme.current.onAccent : Theme.current.accent
                        }

                        Text {
                            Layout.fillWidth: true
                            visible: sidebarOpen
                            text: "Settings"
                            font.pixelSize: 14
                            font.weight: isSelected ? Font.Medium : Font.Normal
                            color: isSelected ? Theme.current.onAccent : Theme.current.textPrimary
                            opacity: sidebarOpen ? 1 : 0
                            Behavior on opacity { NumberAnimation { duration: 150 } }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: pageLoader.source = "pages/settings.qml"
                        onEntered: { if (!isSelected) parent.color = Theme.current.cardBg }
                        onExited: { if (!isSelected) parent.color = "transparent" }
                    }
                }
            }

            // Toggle sidebar button at bottom
            Rectangle {
                id: toggleBtn
                width: parent.width
                height: 40
                radius: 8
                color: "transparent"
                border.color: Theme.current.border
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8

                    Rectangle {
                        width: 20
                        height: 20
                        radius: 4
                        color: Theme.current.textSecondary
                    }

                    Text {
                        Layout.fillWidth: true
                        visible: sidebarOpen
                        text: "Collapse"
                        font.pixelSize: 12
                        color: Theme.current.textSecondary
                        opacity: sidebarOpen ? 1 : 0
                        Behavior on opacity { NumberAnimation { duration: 150 } }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: sidebarOpen = !sidebarOpen
                    onEntered: toggleBtn.color = Theme.current.cardBg
                    onExited: toggleBtn.color = "transparent"
                }
            }
        }
    }
}