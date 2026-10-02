import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Page {
    id: page
    property string page: "import"

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
                    text: "Import Data"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Text {
                    text: "Migrate your snippets from other tools"
                    font.pixelSize: 14
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }
        }

        // Import source cards
        GridLayout {
            Layout.fillWidth: true
            columns: 3
            rowSpacing: 16
            columnSpacing: 16

            ImportSourceCard {
                Layout.fillWidth: true
                title: "Beeftext"
                subtitle: "JSON export (.json)"
                icon: "beeftext"
                color: "#ff6b6b"
                formats: ["json"]
                onImport: fileDialog.open()
            }

            ImportSourceCard {
                Layout.fillWidth: true
                title: "Text Blaze"
                subtitle: "CSV or JSON export"
                icon: "textblaze"
                color: "#4ecdc4"
                formats: ["csv", "json"]
                onImport: fileDialog.open()
            }

            ImportSourceCard {
                Layout.fillWidth: true
                title: "Generic JSON"
                subtitle: "Custom JSON format"
                icon: "json"
                color: "#45b7d1"
                formats: ["json"]
                onImport: fileDialog.open()
            }
        }

        // File dialog
        FileDialog {
            id: fileDialog
            title: "Select Import File"
            folder: QStandardPaths.writableLocation(QStandardPaths.DownloadLocation)
            nameFilters: ["JSON files (*.json)", "CSV files (*.csv)", "All files (*)"]
            onAccepted: {
                ImportManager.import(fileDialog.fileUrl)
            }
        }

        // Preview section
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            visible: importPreview.validCombos > 0
            radius: 16
            color: theme.cardBg
            border.color: theme.border
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 16

                RowLayout {
                    Text {
                        text: "Import Preview"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }
                    Item { Layout.fillWidth: true }
                    Text {
                        text: "Total: ${importPreview.totalCombos}  •  Valid: ${importPreview.validCombos}  •  Invalid: ${importPreview.invalidCombos}"
                        font.pixelSize: 12
                        color: theme.textSecondary
                    }
                }

                // Preview list
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 4
                    model: importPreview.combos

                    delegate: ImportPreviewItem {
                        Layout.fillWidth: true
                        name: model.name
                        keyword: model.keyword
                        snippet: model.snippet
                        valid: model.isValid()
                    }
                }

                // Import button
                Button {
                    Layout.alignment: Qt.AlignRight
                    text: "Import ${importPreview.validCombos} Snippets"
                    enabled: importPreview.validCombos > 0
                    Layout.preferredHeight: 44
                    background: Rectangle {
                        radius: 8
                        gradient: Gradient {
                            GradientStop { position: 0; color: theme.success }
                            GradientStop { position: 1; color: "#2da44e" }
                        }
                    }
                    contentItem: RowLayout {
                        spacing: 8
                        Image { source: "qrc:/icons/import.svg"; width: 18; height: 18; color: "white" }
                        Text { text: "Import ${importPreview.validCombos} Snippets"; color: "white"; font.weight: Font.Medium }
                    }
                    onClicked: ImportManager.confirmImport(importPreview.combos)
                }
            }
        }

        // Supported formats info
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 160
            radius: 16
            color: theme.cardBg
            border.color: theme.border
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 16

                RowLayout {
                    Image { source: "qrc:/icons/info.svg"; width: 24; height: 24; color: theme.accent }
                    Text {
                        text: "Supported Import Formats"
                        font.pixelSize: 18
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 3
                    rowSpacing: 8
                    columnSpacing: 16

                    FormatInfo { format: "Beeftext JSON"; details: "Exported from Beeftext app"; extensions: ".json" }
                    FormatInfo { format: "Text Blaze CSV"; details: "Exported from Text Blaze"; extensions: ".csv" }
                    FormatInfo { format: "Text Blaze JSON"; details: "Exported from Text Blaze"; extensions: ".json" }
                    FormatInfo { format: "Generic JSON"; details: "Custom Blippy format"; extensions: ".json" }
                    FormatInfo { format: "Plain Text"; details: "One snippet per line"; extensions: ".txt" }
                    FormatInfo { format: "Clipboard"; details: "Paste directly from clipboard"; extensions: "N/A" }
                }
            }
        }
    }
}

// Import Source Card Component
Component {
    id: importSourceCard
    Button {
        property string title
        property string subtitle
        property string icon
        property color color
        property var formats
        property var onImport

        Layout.fillWidth: true
        Layout.preferredHeight: 160
        flat: true
        background: Rectangle {
            radius: 16
            color: theme.cardBg
            border.color: theme.border
            border.width: 1
        }
        contentItem: ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 16

            Rectangle {
                width: 56
                height: 56
                radius: 14
                color: color

                Image {
                    anchors.centerIn: parent
                    source: "qrc:/icons/" + icon + ".svg"
                    width: 28
                    height: 28
                    color: "white"
                }
            }

            ColumnLayout {
                spacing: 4

                Text {
                    text: title
                    font.pixelSize: 18
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }

                Text {
                    text: subtitle
                    font.pixelSize: 13
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }

            RowLayout {
                Repeater {
                    model: formats
                    Text {
                        text: model
                        font.pixelSize: 11
                        color: theme.textMuted
                        padding: 4
                        background: Rectangle {
                            radius: 4
                            color: theme.accent + "15"
                        }
                    }
                }
            }
        }
        onClicked: onImport()
    }
}

// Import Preview Item Component
Component {
    id: importPreviewItem
    Rectangle {
        property string name
        property string keyword
        property string snippet
        property bool valid

        Layout.fillWidth: true
        height: 48
        radius: 8
        color: valid ? theme.cardBg : theme.error + "15"
        border.color: valid ? theme.border : theme.error
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12

            Rectangle {
                width: 12
                height: 12
                radius: 6
                color: valid ? theme.success : theme.error
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2

                Text {
                    text: name || keyword
                    font.pixelSize: 13
                    font.weight: Font.Medium
                    color: theme.textPrimary
                }

                Text {
                    text: keyword ? keyword : "No keyword"
                    font.pixelSize: 11
                    font.family: "Monospace"
                    color: theme.accent
                }
            }

            Text {
                text: snippet.length > 40 ? snippet.substring(0, 40) + "..." : snippet
                font.pixelSize: 11
                color: theme.textSecondary
                elide: Text.ElideRight
            }
        }
    }
}

// Format Info Component
Component {
    id: formatInfo
    Rectangle {
        property string format
        property string details
        property string extensions

        Layout.fillWidth: true
        Layout.preferredHeight: 70
        radius: 10
        color: theme.placeholder
        border.color: theme.border
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 4

            Text {
                text: format
                font.pixelSize: 13
                font.weight: Font.Medium
                color: theme.textPrimary
            }

            Text {
                text: details
                font.pixelSize: 11
                color: theme.textSecondary
            }

            Text {
                text: "Extensions: ${extensions}"
                font.pixelSize: 10
                color: theme.textMuted
            }
        }
    }
}