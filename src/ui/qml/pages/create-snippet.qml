import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Page {
    id: page
    property string page: "create-snippet"
    property var snippetData: null

    readonly property var theme: Theme.current
    readonly property bool isEditing: snippetData !== null
    property var formData: {
        name: "",
        keyword: "",
        group: "",
        snippet: "",
        description: "",
        enabled: true,
        caseSensitive: false
    }

    Component.onCompleted: {
        if (isEditing && snippetData) {
            formData.name = snippetData.name || ""
            formData.keyword = snippetData.keyword || ""
            formData.group = snippetData.group || ""
            formData.snippet = snippetData.snippet || ""
            formData.description = snippetData.description || ""
            formData.enabled = snippetData.enabled !== false
            formData.caseSensitive = snippetData.caseSensitive || false
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 20

        // Header
        RowLayout {
            Layout.fillWidth: true

            ColumnLayout {
                Text {
                    text: isEditing ? "Edit Snippet" : "Create Snippet"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Text {
                    text: isEditing ? "Modify your text expansion" : "Build a new text expansion with placeholders"
                    font.pixelSize: 14
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }
        }

        // Form
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ColumnLayout {
                width: parent.width
                spacing: 20

                // Basic Info
                FormSection {
                    Layout.fillWidth: true
                    title: "Basic Information"
                    icon: "info"

                    FormField {
                        label: "Name"
                        placeholder: "My Snippet"
                        text: formData.name
                        onTextChanged: formData.name = text
                    }

                    FormField {
                        label: "Keyword (Trigger)"
                        placeholder: "h1, addr, email..."
                        text: formData.keyword
                        onTextChanged: formData.keyword = text
                        helper: "Type this to trigger the snippet"
                    }

                    FormField {
                        label: "Group"
                        placeholder: "work, personal, coding..."
                        text: formData.group
                        onTextChanged: formData.group = text
                        helper: "Organize snippets into groups"
                    }
                }

                // Snippet Content
                FormSection {
                    Layout.fillWidth: true
                    title: "Snippet Content"
                    icon: "snippet"

                    TextArea {
                        Layout.fillWidth: true
                        Layout.minimumHeight: 160
                        placeholderText: "Enter your snippet content...\n\nUse #placeholders for dynamic content:\n• #text:name - Custom text input\n• #time - Current time (14:32)\n• #date - Today's date (2024-01-15)\n• #day - Day name (Monday)\n• #remaining:2024-12-25 - Days until date\n• #emoji - Emoji picker\n• #random:8 - Random string\n• #shortcut:pt - Trigger another snippet"
                        text: formData.snippet
                        onTextChanged: formData.snippet = text
                        background: Rectangle {
                            radius: 8
                            color: theme.cardBg
                            border.color: theme.border
                        }
                        font.pixelSize: 13
                        font.family: "Monospace"
                        color: theme.textPrimary
                        selectionColor: theme.accent + "80"
                        wrapMode: TextArea.Wrap
                    }
                }

                // Placeholder Helper
                Rectangle {
                    Layout.fillWidth: true
                    Layout.minimumHeight: 120
                    radius: 12
                    color: theme.accent + "15"
                    border.color: theme.accent + "40"
                    border.width: 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 12

                        RowLayout {
                            Image { source: "qrc:/icons/lightbulb.svg"; width: 20; height: 20; color: theme.accent }
                            Text {
                                text: "Placeholder Quick Reference"
                                font.pixelSize: 14
                                font.weight: Font.Bold
                                color: theme.textPrimary
                            }
                        }

                        GridLayout {
                            Layout.fillWidth: true
                            columns: 2
                            rowSpacing: 8
                            columnSpacing: 16

                            Repeater {
                                model: [
                                    { ph: "#text:name", desc: "Prompt for text input" },
                                    { ph: "#time", desc: "Current time (14:32)" },
                                    { ph: "#date", desc: "Today's date (2024-01-15)" },
                                    { ph: "#day", desc: "Day name (Monday)" },
                                    { ph: "#remaining:2024-12-25", desc: "Days until date" },
                                    { ph: "#emoji", desc: "Insert emoji" },
                                    { ph: "#random:8", desc: "8-char random string" },
                                    { ph: "#shortcut:pt", desc: "Trigger 'pt' snippet" }
                                ]

                                PlaceholderRef {
                                    Layout.fillWidth: true
                                    placeholder: model.ph
                                    description: model.desc
                                }
                            }
                        }
                    }
                }

                // Options
                FormSection {
                    Layout.fillWidth: true
                    title: "Options"
                    icon: "sliders"

                    FormField {
                        label: "Description"
                        placeholder: "Optional description"
                        text: formData.description
                        onTextChanged: formData.description = text
                    }

                    RowLayout {
                        Layout.fillWidth: true

                        Switch {
                            id: enabledSwitch
                            checked: formData.enabled
                            Layout.alignment: Qt.AlignLeft
                        }

                        Text {
                            text: "Enabled"
                            font.pixelSize: 14
                            color: theme.textPrimary
                        }

                        Item { Layout.fillWidth: true }

                        Switch {
                            checked: formData.caseSensitive
                            Layout.alignment: Qt.AlignLeft
                        }

                        Text {
                            text: "Case Sensitive"
                            font.pixelSize: 14
                            color: theme.textPrimary
                        }
                    }
                }
            }
        }

        // Action buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Item { Layout.fillWidth: true }

            Button {
                text: "Cancel"
                flat: true
                Layout.preferredHeight: 44
                contentItem: Text { text: "Cancel"; color: theme.textSecondary; font.pixelSize: 14 }
                onClicked: stackView.pop()
            }

            Button {
                text: isEditing ? "Save Changes" : "Create Snippet"
                Layout.preferredHeight: 44
                background: Rectangle {
                    radius: 8
                    gradient: Gradient {
                        GradientStop { position: 0; color: theme.accent }
                        GradientStop { position: 1; color: theme.accentAlt }
                    }
                }
                contentItem: RowLayout {
                    spacing: 8
                    Image { source: isEditing ? "qrc:/icons/save.svg" : "qrc:/icons/check.svg"; width: 18; height: 18; color: "white" }
                    Text { text: isEditing ? "Save Changes" : "Create Snippet"; color: "white"; font.weight: Font.Medium }
                }
                onClicked: {
                    // Save snippet logic
                    console.log("Saving snippet:", formData)
                    stackView.pop()
                }
            }
        }
    }
}

// Helper Components
Component {
    id: formSection
    Rectangle {
        property string title
        property string icon

        Layout.fillWidth: true
        radius: 16
        color: theme.cardBg
        border.color: theme.border
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            // Section header
            Rectangle {
                Layout.fillWidth: true
                height: 56
                radius: 16
                color: theme.cardBg
                border.color: theme.border
                border.width: 1

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 16

                    Rectangle {
                        width: 36
                        height: 36
                        radius: 8
                        color: theme.accent + "20"

                        Image {
                            anchors.centerIn: parent
                            source: "qrc:/icons/" + icon + ".svg"
                            width: 20
                            height: 20
                            color: theme.accent
                        }
                    }

                    Text {
                        text: title
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }
                }
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16
            }
        }
    }
}

Component {
    id: formField
    ColumnLayout {
        property string label
        property string placeholder
        property string helper
        property string text
        property var onTextChanged

        Layout.fillWidth: true
        spacing: 6

        Text {
            text: label
            font.pixelSize: 13
            font.weight: Font.Medium
            color: theme.textPrimary
        }

        TextField {
            Layout.fillWidth: true
            placeholderText: placeholder
            text: text
            onTextChanged: onTextChanged(text)
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

        Text {
            text: helper
            font.pixelSize: 11
            color: theme.textMuted
            visible: helper && helper.length > 0
        }
    }
}

Component {
    id: placeholderRef
    Button {
        property string placeholder
        property string description

        Layout.fillWidth: true
        height: 40
        flat: true
        background: Rectangle {
            radius: 8
            color: theme.cardBg
            border.color: theme.border
        }
        contentItem: RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12

            Text {
                text: placeholder
                font.pixelSize: 12
                font.family: "Monospace"
                font.weight: Font.Medium
                color: theme.accent
            }

            Text {
                Layout.fillWidth: true
                text: description
                font.pixelSize: 12
                color: theme.textSecondary
            }

            Button {
                flat: true
                contentItem: RowLayout {
                    spacing: 4
                    Text { text: "Insert"; color: theme.accent; font.pixelSize: 11; font.weight: Font.Medium }
                    Image { source: "qrc:/icons/arrow-right.svg"; width: 12; height: 12; color: theme.accent }
                }
                onClicked: {
                    console.log("Insert:", placeholder)
                }
            }
        }
        onClicked: {
            console.log("Insert placeholder:", placeholder)
        }
    }
}