import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: page
    property string page: "glossary"

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
                    text: "Glossary & Corrections"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Text {
                    text: "Auto-correct common typos and expand abbreviations"
                    font.pixelSize: 14
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Add Entry"
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
                    Text { text: "Add Entry"; color: "white"; font.weight: Font.Medium }
                }
            }
        }

        // Tabs for different glossary types
        TabBar {
            Layout.fillWidth: true
            id: tabBar
            currentIndex: 0

            TabButton { text: "Auto-Correct" }
            TabButton { text: "Abbreviations" }
            TabButton { text: "Brand Terms" }
            TabButton { text: "Custom Rules" }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            // Auto-Correct
            ListView {
                clip: true
                spacing: 8
                model: [
                    { typo: "teh", correct: "the", enabled: true },
                    { typo: "recieve", correct: "receive", enabled: true },
                    { typo: "seperate", correct: "separate", enabled: true },
                    { typo: "occured", correct: "occurred", enabled: true },
                    { typo: "definately", correct: "definitely", enabled: true },
                    { typo: "accomodate", correct: "accommodate", enabled: false },
                    { typo: "neccessary", correct: "necessary", enabled: true },
                    { typo: "priviledge", correct: "privilege", enabled: true }
                ]

                delegate: GlossaryItem {
                    Layout.fillWidth: true
                    typo: model.typo
                    correct: model.correct
                    enabled: model.enabled
                    type: "autocorrect"
                }
            }

            // Abbreviations
            ListView {
                clip: true
                spacing: 8
                model: [
                    { abbr: "btw", full: "by the way", enabled: true },
                    { abbr: "fyi", full: "for your information", enabled: true },
                    { abbr: "asap", full: "as soon as possible", enabled: true },
                    { abbr: "imo", full: "in my opinion", enabled: true },
                    { abbr: "tbh", full: "to be honest", enabled: true },
                    { abbr: "idk", full: "i don't know", enabled: true },
                    { abbr: "omw", full: "on my way", enabled: false },
                    { abbr: "tl;dr", full: "too long; didn't read", enabled: true }
                ]

                delegate: GlossaryItem {
                    Layout.fillWidth: true
                    typo: model.abbr
                    correct: model.full
                    enabled: model.enabled
                    type: "abbreviation"
                }
            }

            // Brand Terms
            ListView {
                clip: true
                spacing: 8
                model: [
                    { term: "Blippy", correct: "Blippy", enabled: true },
                    { term: "blippy", correct: "Blippy", enabled: true },
                    { term: "BLIPPY", correct: "Blippy", enabled: true },
                    { term: "text expander", correct: "text expander", enabled: false },
                    { term: "snippet tool", correct: "snippet tool", enabled: false }
                ]

                delegate: GlossaryItem {
                    Layout.fillWidth: true
                    typo: model.term
                    correct: model.correct
                    enabled: model.enabled
                    type: "brand"
                }
            }

            // Custom Rules
            ColumnLayout {
                spacing: 16

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
                            Image { source: "qrc:/icons/settings.svg"; width: 24; height: 24; color: theme.accent }
                            Text {
                                text: "Custom Correction Rules"
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                color: theme.textPrimary
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            wrapMode: Text.Wrap
                            text: "Create advanced rules with regex patterns, case sensitivity, and context-aware corrections."
                            font.pixelSize: 13
                            color: theme.textSecondary
                        }

                        Button {
                            text: "Create Rule"
                            icon.source: "qrc:/icons/plus.svg"
                            onClicked: console.log("Create custom rule")
                        }
                    }
                }
            }
        }
    }
}

// Glossary Item Component
Component {
    id: glossaryItem
    Rectangle {
        property string typo
        property string correct
        property bool enabled
        property string type: "autocorrect"

        Layout.fillWidth: true
        height: 56
        radius: 10
        color: theme.cardBg
        border.color: theme.border
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 16

            // Type indicator
            Rectangle {
                width: 36
                height: 36
                radius: 8
                color: type === "autocorrect" ? theme.error :
                       type === "abbreviation" ? theme.accent :
                       type === "brand" ? theme.warning : theme.success

                Image {
                    anchors.centerIn: parent
                    source: type === "autocorrect" ? "qrc:/icons/spell-check.svg" :
                            type === "abbreviation" ? "qrc:/icons/shortcut.svg" :
                            type === "brand" ? "qrc:/icons/trademark.svg" : "qrc:/icons/settings.svg"
                    width: 18
                    height: 18
                    color: "white"
                }
            }

            // Typo/abbreviation
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2

                Text {
                    text: typo
                    font.pixelSize: 13
                    font.family: "Monospace"
                    font.weight: Font.Medium
                    color: theme.textPrimary
                }

                RowLayout {
                    Text { text: "→"; font.pixelSize: 11; color: theme.textMuted }
                    Text {
                        text: correct
                        font.pixelSize: 12
                        color: theme.accent
                    }
                }
            }

            // Enabled toggle
            Switch {
                checked: enabled
                onToggled: console.log("Toggle:", typo, checked)
            }

            // Delete button
            Button {
                flat: true
                contentItem: Image { source: "qrc:/icons/trash.svg"; width: 16; height: 16; color: theme.error }
            }
        }
    }
}