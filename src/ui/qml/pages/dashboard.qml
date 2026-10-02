import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: page
    property string page: "dashboard"

    readonly property var theme: Theme.current

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // Header
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                Text {
                    text: "Welcome back! 👋"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Text {
                    text: "Your snippets are ready to blip"
                    font.pixelSize: 14
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }

            // Stats cards
            Repeater {
                model: [
                    { label: "Total Snippets", value: "47", icon: "snippet", color: theme.accent },
                    { label: "Today's Blips", value: "12", icon: "zap", color: theme.success },
                    { label: "Placeholders Used", value: "8", icon: "placeholder", color: theme.warning },
                    { label: "Time Saved", value: "2.3h", icon: "clock", color: theme.error }
                ]

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 90
                    radius: 16
                    color: theme.cardBg
                    border.color: theme.border
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16

                        Rectangle {
                            width: 48
                            height: 48
                            radius: 12
                            color: model.color

                            Rectangle {
                                anchors.centerIn: parent
                                width: 24
                                height: 24
                                radius: 6
                                color: "white"
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4

                            Text {
                                text: model.value
                                font.pixelSize: 24
                                font.weight: Font.Bold
                                color: theme.textPrimary
                            }

                            Text {
                                text: model.label
                                font.pixelSize: 12
                                color: theme.textSecondary
                            }
                        }
                    }
                }
            }
        }

        // Quick actions
        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                flat: true
                background: Rectangle {
                    radius: 12
                    color: theme.cardBg
                    border.color: theme.border
                    border.width: 1
                }
                contentItem: RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    Rectangle {
                        width: 44
                        height: 44
                        radius: 10
                        color: theme.accent

                        Rectangle {
                            anchors.centerIn: parent
                            width: 22
                            height: 22
                            radius: 6
                            color: "white"
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text { text: "Create Snippet"; font.pixelSize: 15; font.weight: Font.Medium; color: theme.textPrimary }
                        Text { text: "Add a new text expansion"; font.pixelSize: 12; color: theme.textSecondary }
                    }
                    Item { Layout.fillWidth: true }
                    Rectangle {
                        width: 18
                        height: 18
                        radius: 4
                        color: theme.textMuted
                    }
                }
                onClicked: console.log("Create snippet clicked")
            }

            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                flat: true
                background: Rectangle {
                    radius: 12
                    color: theme.cardBg
                    border.color: theme.border
                    border.width: 1
                }
                contentItem: RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    Rectangle {
                        width: 44
                        height: 44
                        radius: 10
                        color: theme.accent

                        Rectangle {
                            anchors.centerIn: parent
                            width: 22
                            height: 22
                            radius: 6
                            color: "white"
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text { text: "Import Data"; font.pixelSize: 15; font.weight: Font.Medium; color: theme.textPrimary }
                        Text { text: "Migrate from Beeftext/Text Blaze"; font.pixelSize: 12; color: theme.textSecondary }
                    }
                    Item { Layout.fillWidth: true }
                    Rectangle {
                        width: 18
                        height: 18
                        radius: 4
                        color: theme.textMuted
                    }
                }
                onClicked: console.log("Import clicked")
            }

            Button {
                Layout.fillWidth: true
                Layout.preferredHeight: 80
                flat: true
                background: Rectangle {
                    radius: 12
                    color: theme.cardBg
                    border.color: theme.border
                    border.width: 1
                }
                contentItem: RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    Rectangle {
                        width: 44
                        height: 44
                        radius: 10
                        color: theme.accent

                        Rectangle {
                            anchors.centerIn: parent
                            width: 22
                            height: 22
                            radius: 6
                            color: "white"
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2
                        Text { text: "Setup Backup"; font.pixelSize: 15; font.weight: Font.Medium; color: theme.textPrimary }
                        Text { text: "Configure cloud sync"; font.pixelSize: 12; color: theme.textSecondary }
                    }
                    Item { Layout.fillWidth: true }
                    Rectangle {
                        width: 18
                        height: 18
                        radius: 4
                        color: theme.textMuted
                    }
                }
                onClicked: console.log("Backup clicked")
            }
        }

        // Recent activity
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12

            RowLayout {
                Text {
                    text: "Recent Activity"
                    font.pixelSize: 18
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Item { Layout.fillWidth: true }
                Button {
                    text: "View All"
                    flat: true
                    contentItem: Text {
                        text: "View All"
                        color: theme.accent
                        font.pixelSize: 13
                    }
                }
            }

            ColumnLayout {
                spacing: 8

                Repeater {
                    model: [
                        { time: "2 min ago", action: "Created snippet", detail: "\"addr\" → address template", icon: "plus", color: theme.success },
                        { time: "15 min ago", action: "Used placeholder", detail: "\"#time\" in email signature", icon: "placeholder", color: theme.accent },
                        { time: "1 hour ago", action: "Blip executed", detail: "\"h1\" expanded to greeting", icon: "zap", color: theme.warning },
                        { time: "3 hours ago", action: "Synced to cloud", detail: "Google Drive backup complete", icon: "cloud", color: theme.info }
                    ]

                    Rectangle {
                        Layout.fillWidth: true
                        height: 56
                        radius: 10
                        color: theme.cardBg
                        border.color: theme.border
                        border.width: 1

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 12
                            spacing: 12

                            Rectangle {
                                width: 36
                                height: 36
                                radius: 8
                                color: model.color

                                Rectangle {
                                    anchors.centerIn: parent
                                    width: 18
                                    height: 18
                                    radius: 4
                                    color: "white"
                                }
                            }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2
                                Text { text: model.action; font.pixelSize: 13; font.weight: Font.Medium; color: theme.textPrimary }
                                Text { text: model.detail; font.pixelSize: 11; color: theme.textSecondary }
                            }

                            Text { text: model.time; font.pixelSize: 11; color: theme.textMuted }
                        }
                    }
                }
            }
        }
    }
}