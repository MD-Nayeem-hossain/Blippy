import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: page
    property string page: "snippets"

    readonly property var theme: Theme.current

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // Header with search and add button
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                Text {
                    text: "Snippets"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Text {
                    text: "Manage your text expansions"
                    font.pixelSize: 14
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }

            TextField {
                id: searchField
                Layout.fillWidth: true
                Layout.maximumWidth: 300
                placeholderText: "Search snippets..."
                padding: 12
                background: Rectangle {
                    radius: 8
                    color: theme.cardBg
                    border.color: theme.border
                }
                contentItem: TextInput {
                    color: theme.textPrimary
                    font.pixelSize: 13
                }
            }

            Button {
                id: addBtn
                text: "New Snippet"
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
                    Rectangle {
                        width: 16
                        height: 16
                        radius: 4
                        color: "white"
                    }
                    Text { text: "New Snippet"; color: "white"; font.weight: Font.Medium }
                }
                onClicked: console.log("New Snippet clicked")
            }
        }

        // Snippet list
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 8
            model: [
                { id: "1", name: "Greeting", keyword: "h1", snippet: "Hello #{text:name}!", group: "work", enabled: true },
                { id: "2", name: "Address", keyword: "addr", snippet: "123 Main St\nCity, State 12345", group: "personal", enabled: true },
                { id: "3", name: "Email", keyword: "email", snippet: "example@example.com", group: "work", enabled: true },
                { id: "4", name: "Phone", keyword: "phone", snippet: "(555) 123-4567", group: "personal", enabled: true }
            ]

            delegate: Rectangle {
                Layout.fillWidth: true
                height: 80
                radius: 12
                color: theme.cardBg
                border.color: theme.border
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16

                    // Enable toggle
                    Switch {
                        checked: model.enabled
                        onToggled: console.log("Toggle:", model.id)
                    }

                    // Snippet info
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        RowLayout {
                            Text {
                                text: model.name
                                font.pixelSize: 15
                                font.weight: Font.Medium
                                color: theme.textPrimary
                            }
                            Item { Layout.fillWidth: true }
                            Text {
                                text: model.group
                                font.pixelSize: 11
                                color: theme.accent
                                padding: 4
                                background: Rectangle {
                                    radius: 4
                                    color: theme.accent + "20"
                                }
                            }
                        }

                        RowLayout {
                            Text {
                                text: model.keyword
                                font.pixelSize: 12
                                font.family: "Monospace"
                                color: theme.accent
                                background: Rectangle {
                                    radius: 4
                                    color: theme.accent + "15"
                                    anchors.margins: -4
                                }
                            }
                            Text {
                                text: "→"
                                font.pixelSize: 12
                                color: theme.textMuted
                            }
                            Text {
                                text: model.snippet.length > 50 ? model.snippet.substring(0, 50) + "..." : model.snippet
                                font.pixelSize: 12
                                color: theme.textSecondary
                                elide: Text.ElideRight
                            }
                        }
                    }

                    // Actions
                    RowLayout {
                        spacing: 8

                        Button {
                            flat: true
                            contentItem: Rectangle {
                                width: 18
                                height: 18
                                radius: 4
                                color: theme.textSecondary
                            }
                            onClicked: console.log("Edit:", model.id)
                        }

                        Button {
                            flat: true
                            contentItem: Rectangle {
                                width: 18
                                height: 18
                                radius: 4
                                color: theme.textSecondary
                            }
                            onClicked: Qt.setClipboardText(model.snippet)
                        }

                        Button {
                            flat: true
                            contentItem: Rectangle {
                                width: 18
                                height: 18
                                radius: 4
                                color: theme.error
                            }
                            onClicked: console.log("Delete:", model.id)
                        }
                    }
                }
            }
        }
    }
}