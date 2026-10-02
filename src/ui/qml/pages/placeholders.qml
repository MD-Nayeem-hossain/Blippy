import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: page
    property string page: "placeholders"

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
                    text: "Placeholders"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Text {
                    text: "Dynamic content that expands when you trigger a snippet"
                    font.pixelSize: 14
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Add Placeholder"
                icon.source: "qrc:/icons/plus.svg"
                Layout.preferredHeight: 40
                background: Rectangle {
                    radius: 8
                    gradient: Gradient {
                        GradientStop { position: 0; color: theme.accent }
                        GradientStop { position: 1; color: theme.accentAlt }
                    }
                }
                contentItem: RowLayout {
                    spacing: 8
                    Image { source: "qrc:/icons/plus.svg"; width: 16; height: 16; color: "white" }
                    Text { text: "Add Placeholder"; color: "white"; font.weight: Font.Medium }
                }
            }
        }

        // Placeholder types grid
        GridLayout {
            Layout.fillWidth: true
            columns: 4
            rowSpacing: 16
            columnSpacing: 16

            Repeater {
                model: [
                    { type: "text", name: "Text Input", desc: "Prompt for custom text", icon: "text-input", example: "#text:name", color: theme.accent },
                    { type: "time", name: "Current Time", desc: "Inserts current time (HH:mm)", icon: "clock", example: "#time → 14:32", color: theme.success },
                    { type: "date", name: "Current Date", desc: "Inserts today's date (YYYY-MM-DD)", icon: "calendar", example: "#date → 2024-01-15", color: theme.warning },
                    { type: "day", name: "Day Name", desc: "Inserts day of week", icon: "sun", example: "#day → Monday", color: theme.info },
                    { type: "remaining", name: "Time Remaining", desc: "Days until target date", icon: "timer", example: "#remaining:2024-12-25", color: theme.error },
                    { type: "emoji", name: "Emoji", desc: "Insert emoji from picker", icon: "smile", example: "#emoji → 😀", color: theme.accentAlt },
                    { type: "random", name: "Random String", desc: "Generate random characters", icon: "shuffle", example: "#random:8 → aB3x9Km2", color: theme.success },
                    { type: "shortcut", name: "Shortcut Trigger", desc: "Trigger another snippet", icon: "link", example: "#shortcut:pt", color: theme.warning }
                ]

                PlaceholderCard {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    name: model.name
                    desc: model.desc
                    icon: model.icon
                    example: model.example
                    color: model.color
                }
            }
        }

        // Shortcut placeholders explanation
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            radius: 16
            color: theme.cardBg
            border.color: theme.border
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 12

                RowLayout {
                    Image { source: "qrc:/icons/lightbulb.svg"; width: 24; height: 24; color: theme.warning }
                    Text {
                        text: "Pro Tip: Shortcut Placeholders"
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }
                }

                Text {
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                    text: "Use #shortcut:pt inside any snippet to trigger another snippet! Type 'pt' in a text placeholder and it'll expand to 'Payments & Treasury' automatically."
                    font.pixelSize: 13
                    color: theme.textSecondary
                }

                Button {
                    text: "Try it now"
                    icon.source: "qrc:/icons/arrow-right.svg"
                    flat: true
                    contentItem: RowLayout {
                        spacing: 8
                        Text { text: "Try it now"; color: theme.accent; font.weight: Font.Medium }
                        Image { source: "qrc:/icons/arrow-right.svg"; width: 16; height: 16; color: theme.accent }
                    }
                    onClicked: stackView.push("qrc:/pages/create-snippet.qml")
                }
            }
        }
    }
}

// Placeholder Card Component
Component {
    id: placeholderCard
    Rectangle {
        property string name
        property string desc
        property string icon
        property string example
        property color color

        Layout.fillWidth: true
        Layout.preferredHeight: 140
        radius: 16
        color: theme.cardBg
        border.color: theme.border
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
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
                        text: name
                        font.pixelSize: 14
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }

                    Text {
                        text: desc
                        font.pixelSize: 12
                        color: theme.textSecondary
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: theme.border
            }

            RowLayout {
                Layout.fillWidth: true

                Text {
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                    text: example
                    font.pixelSize: 12
                    font.family: "Monospace"
                    color: theme.accent
                    background: Rectangle {
                        radius: 4
                        color: theme.accent + "15"
                        anchors.margins: -4
                    }
                }

                Button {
                    flat: true
                    contentItem: RowLayout {
                        spacing: 4
                        Text { text: "Copy"; color: theme.accent; font.pixelSize: 11; font.weight: Font.Medium }
                        Image { source: "qrc:/icons/copy.svg"; width: 14; height: 14; color: theme.accent }
                    }
                    onClicked: Qt.setClipboardText(example)
                }
            }
        }
    }
}