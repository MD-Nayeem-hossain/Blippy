import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: page
    property string page: "backup"

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
                    text: "Backup & Sync"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Text {
                    text: "Keep your snippets safe and synced across devices"
                    font.pixelSize: 14
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }
        }

        // Backup status cards
        GridLayout {
            Layout.fillWidth: true
            columns: 3
            rowSpacing: 16
            columnSpacing: 16

            StatusCard {
                Layout.fillWidth: true
                title: "Local Backup"
                subtitle: "Last backup: 2 hours ago"
                status: "active"
                icon: "database"
                color: theme.success
                action: () => BackupManager.createLocalBackup()
                actionText: "Backup Now"
            }

            StatusCard {
                Layout.fillWidth: true
                title: "Cloud Sync"
                subtitle: "Google Drive • Synced 1h ago"
                status: "synced"
                icon: "cloud"
                color: theme.accent
                action: () => BackupManager.syncToCloud()
                actionText: "Sync Now"
            }

            StatusCard {
                Layout.fillWidth: true
                title: "Version History"
                subtitle: "12 backups available"
                status: "ready"
                icon: "history"
                color: theme.warning
                action: () => stackView.push("qrc:/pages/version-history.qml")
                actionText: "View History"
            }
        }

        // Cloud configuration
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            radius: 16
            color: theme.cardBg
            border.color: theme.border
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 16

                RowLayout {
                    Image { source: "qrc:/icons/cloud.svg"; width: 24; height: 24; color: theme.accent }
                    Text {
                        text: "Google Drive Sync (via rclone)"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }
                }

                Text {
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                    text: "Configure rclone with your Google Drive credentials, then Blippy will automatically sync your backups. No OAuth in the app - you set up rclone once, Blippy uses it."
                    font.pixelSize: 13
                    color: theme.textSecondary
                }

                RowLayout {
                    spacing: 12

                    Button {
                        text: "Setup rclone"
                        icon.source: "qrc:/icons/settings.svg"
                        onClicked: console.log("Open rclone setup")
                    }

                    Button {
                        text: "Test Connection"
                        icon.source: "qrc:/icons/cloud-check.svg"
                        flat: true
                        contentItem: RowLayout {
                            spacing: 8
                            Text { text: "Test Connection"; color: theme.accent; font.weight: Font.Medium }
                            Image { source: "qrc:/icons/arrow-right.svg"; width: 16; height: 16; color: theme.accent }
                        }
                    }

                    Button {
                        text: "Sync Now"
                        icon.source: "qrc:/icons/sync.svg"
                        background: Rectangle {
                            radius: 8
                            gradient: Gradient {
                                GradientStop { position: 0; color: theme.accent }
                                GradientStop { position: 1; color: theme.accentAlt }
                            }
                        }
                        contentItem: RowLayout {
                            spacing: 8
                            Image { source: "qrc:/icons/sync.svg"; width: 16; height: 16; color: "white" }
                            Text { text: "Sync Now"; color: "white"; font.weight: Font.Medium }
                        }
                    }
                }
            }
        }

        // Local backup settings
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            radius: 16
            color: theme.cardBg
            border.color: theme.border
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 16

                RowLayout {
                    Image { source: "qrc:/icons/database.svg"; width: 24; height: 24; color: theme.success }
                    Text {
                        text: "Local Backup Settings"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16

                    ColumnLayout {
                        Layout.fillWidth: true

                        SettingToggle {
                            title: "Auto Backup"
                            subtitle: "Automatically backup after each snippet change"
                            checked: true
                        }

                        SettingToggle {
                            title: "Compress Backups"
                            subtitle: "Save space with compression"
                            checked: true
                        }

                        SettingToggle {
                            title: "Encrypt Local Backups"
                            subtitle: "AES-256 encryption for local files"
                            checked: false
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true

                        SettingSlider {
                            title: "Max Backups to Keep"
                            subtitle: "Older backups will be deleted automatically"
                            value: 12
                            min: 1
                            max: 100
                        }

                        SettingSlider {
                            title: "Backup Interval (minutes)"
                            subtitle: "How often to check for changes"
                            value: 30
                            min: 5
                            max: 1440
                        }
                    }
                }
            }
        }
    }
}

// Status Card Component
Component {
    id: statusCard
    Rectangle {
        property string title
        property string subtitle
        property string status // "active", "synced", "ready", "error"
        property string icon
        property color color
        property var action
        property string actionText

        Layout.fillWidth: true
        Layout.preferredHeight: 140
        radius: 16
        color: theme.cardBg
        border.color: theme.border
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 12

            RowLayout {
                Rectangle {
                    width: 44
                    height: 44
                    radius: 10
                    color: color

                    Image {
                        anchors.centerIn: parent
                        source: "qrc:/icons/" + icon + ".svg"
                        width: 22
                        height: 22
                        color: "white"
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    Text {
                        text: title
                        font.pixelSize: 14
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }

                    Text {
                        text: subtitle
                        font.pixelSize: 12
                        color: theme.textSecondary
                    }
                }

                Item { Layout.fillWidth: true }

                // Status indicator
                Rectangle {
                    width: 10
                    height: 10
                    radius: 5
                    color: status === "active" || status === "synced" ? theme.success :
                           status === "ready" ? theme.warning : theme.error
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: theme.border
            }

            Button {
                Layout.alignment: Qt.AlignRight
                text: actionText
                flat: true
                contentItem: RowLayout {
                    spacing: 8
                    Text { text: actionText; color: color; font.weight: Font.Medium; font.pixelSize: 12 }
                    Image { source: "qrc:/icons/arrow-right.svg"; width: 14; height: 14; color: color }
                }
                onClicked: action()
            }
        }
    }
}

// Setting Toggle Component
Component {
    id: settingToggle
    Rectangle {
        property string title
        property string subtitle
        property bool checked

        Layout.fillWidth: true
        Layout.preferredHeight: 56
        radius: 0
        color: "transparent"
        border.bottom: index === items.length - 1 ? 0 : { width: 1; color: theme.border }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 16

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Text {
                    text: title
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    color: theme.textPrimary
                }

                Text {
                    text: subtitle
                    font.pixelSize: 12
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }

            Switch {
                checked: checked
                onToggled: console.log(title, ":", checked)
            }
        }
    }
}

// Setting Slider Component
Component {
    id: settingSlider
    Rectangle {
        property string title
        property string subtitle
        property int value
        property int min
        property int max

        Layout.fillWidth: true
        Layout.preferredHeight: 56
        radius: 0
        color: "transparent"
        border.bottom: index === items.length - 1 ? 0 : { width: 1; color: theme.border }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Text {
                    text: title
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    color: theme.textPrimary
                }

                Text {
                    text: subtitle
                    font.pixelSize: 12
                    color: theme.textSecondary
                }
            }

            RowLayout {
                spacing: 12
                Slider {
                    Layout.fillWidth: true
                    from: min
                    to: max
                    value: value
                    stepSize: 1
                    onValueChanged: console.log(title, ":", value)
                }
                Text {
                    text: value
                    font.pixelSize: 13
                    font.family: "Monospace"
                    color: theme.accent
                    Layout.preferredWidth: 50
                }
            }
        }
    }
}