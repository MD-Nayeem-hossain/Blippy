import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: page
    property string page: "settings"

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
                    text: "Settings"
                    font.pixelSize: 28
                    font.weight: Font.Bold
                    color: theme.textPrimary
                }
                Text {
                    text: "Customize your Blippy experience"
                    font.pixelSize: 14
                    color: theme.textSecondary
                }
            }

            Item { Layout.fillWidth: true }
        }

        // Settings sections
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            spacing: 16

            model: [
                { id: "general", title: "General", icon: "settings", items: [
                    { type: "toggle", title: "Enable Blippy", key: "enabled", default: true, desc: "Master switch for all text expansions" },
                    { type: "toggle", title: "Start at Login", key: "autostart", default: true, desc: "Launch Blippy automatically on startup" },
                    { type: "toggle", title: "Run in Background", key: "background", default: true, desc: "Keep running when window is closed" },
                    { type: "toggle", title: "Show Notifications", key: "notifications", default: true, desc: "Show toast when snippet expands" }
                ]},
                { id: "appearance", title: "Appearance", icon: "palette", items: [
                    { type: "toggle", title: "Dark Mode", key: "darkMode", default: true, desc: "Use dark theme (requires restart)" },
                    { type: "select", title: "Accent Color", key: "accentColor", options: ["Blue", "Purple", "Green", "Orange", "Pink", "Red"], default: "Blue", desc: "Choose your accent color" },
                    { type: "toggle", title: "Fluid Animations", key: "animations", default: true, desc: "Enable smooth transitions and effects" },
                    { type: "toggle", title: "Compact Mode", key: "compact", default: false, desc: "Reduce spacing for more content" }
                ]},
                { id: "behavior", title: "Behavior", icon: "sliders", items: [
                    { type: "toggle", title: "Auto Substitute", key: "autoSubstitute", default: true, desc: "Automatically expand on keyword match" },
                    { type: "toggle", title: "Trigger on Space", key: "triggerOnSpace", default: true, desc: "Expand when space is typed after keyword" },
                    { type: "toggle", title: "Keep Final Space", key: "keepSpace", default: true, desc: "Preserve space after expansion" },
                    { type: "select", title: "Insert Method", key: "insertMethod", options: ["Clipboard (Fast)", "Keystrokes (Compatible)"], default: "Clipboard (Fast)", desc: "How to insert expanded text" },
                    { type: "toggle", title: "Shift+Insert Paste", key: "shiftInsert", default: false, desc: "Use Shift+Insert instead of Ctrl+V" },
                    { type: "toggle", title: "Restore Clipboard", key: "restoreClipboard", default: true, desc: "Restore clipboard after substitution" },
                    { type: "slider", title: "Suppression Delay (ms)", key: "suppressionDelay", min: 100, max: 1000, default: 300, desc: "Delay to prevent double-paste" }
                ]},
                { id: "placeholders", title: "Placeholders", icon: "placeholder", items: [
                    { type: "toggle", title: "Enable Placeholders", key: "placeholdersEnabled", default: true, desc: "Allow #placeholder syntax in snippets" },
                    { type: "toggle", title: "Emoji Shortcodes", key: "emojiShortcodes", default: true, desc: "Enable :emoji: syntax" },
                    { type: "select", title: "Left Delimiter", key: "emojiLeft", options: [":", "(", "["], default: ":", desc: "Emoji shortcode left delimiter" },
                    { type: "select", title: "Right Delimiter", key: "emojiRight", options: [":", ")", "]"], default: ":", desc: "Emoji shortcode right delimiter" }
                ]},
                { id: "exclusions", title: "Excluded Apps", icon: "shield", items: [
                    { type: "list", title: "Apps Where Blippy is Disabled", key: "excludedApps", desc: "Blippy won't expand in these applications" }
                ]},
                { id: "advanced", title: "Advanced", icon: "terminal", items: [
                    { type: "toggle", title: "Debug Logging", key: "debugLog", default: false, desc: "Enable verbose logging for troubleshooting" },
                    { type: "button", title: "Open Log File", action: () => console.log("Open log file") },
                    { type: "button", title: "Reset All Settings", action: () => console.log("Reset settings"), destructive: true },
                    { type: "button", title: "Export Settings", action: () => console.log("Export settings") },
                    { type: "button", title: "Import Settings", action: () => console.log("Import settings") }
                ]}
            ]

            delegate: SettingsSection {
                Layout.fillWidth: true
                title: model.title
                icon: model.icon
                items: model.items
            }
        }
    }
}

// Settings Section Component
Component {
    id: settingsSection
    Rectangle {
        property string title
        property string icon
        property var items

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

            // Settings items
            ColumnLayout {
                spacing: 1
                Repeater {
                    model: items

                    SettingsItem {
                        Layout.fillWidth: true
                        title: model.title
                        desc: model.desc
                        type: model.type
                        checked: model.default
                        options: model.options
                        min: model.min
                        max: model.max
                        default: model.default
                        action: model.action
                        destructive: model.destructive
                    }
                }
            }
        }
    }
}

// Settings Item Component
Component {
    id: settingsItem
    Rectangle {
        property string title
        property string desc
        property string type // "toggle", "select", "slider", "button", "list"
        property bool checked
        property var options
        property int min
        property int max
        property int default
        property var action
        property bool destructive: false

        Layout.fillWidth: true
        height: type === "button" ? 48 : (desc ? 64 : 48)
        radius: 0
        color: "transparent"
        border.bottom: index === items.length - 1 ? 0 : { width: 1; color: theme.border }

        RowLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 16

            ColumnLayout {
                Layout.fillWidth: true
                spacing: desc ? 4 : 0

                Text {
                    text: title
                    font.pixelSize: 14
                    font.weight: Font.Medium
                    color: destructive ? theme.error : theme.textPrimary
                }

                Text {
                    text: desc
                    font.pixelSize: 12
                    color: theme.textSecondary
                    visible: desc && desc.length > 0
                }
            }

            Item { Layout.fillWidth: true }

            // Control based on type
            Loader {
                sourceComponent: type === "toggle" ? toggleComp :
                                 type === "select" ? selectComp :
                                 type === "slider" ? sliderComp :
                                 type === "button" ? buttonComp :
                                 type === "list" ? listComp : null
            }
        }
    }

    // Toggle Component
    Component {
        id: toggleComp
        Switch {
            checked: checked
            onToggled: console.log(title, ":", checked)
        }
    }

    // Select Component
    Component {
        id: selectComp
        ComboBox {
            width: 180
            model: options
            currentIndex: options.indexOf(default)
            onActivated: console.log(title, ":", model[index])
            background: Rectangle { radius: 6; color: theme.cardBg; border.color: theme.border }
        }
    }

    // Slider Component
    Component {
        id: sliderComp
        RowLayout {
            spacing: 12
            Slider {
                Layout.fillWidth: true
                from: min
                to: max
                value: default
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

    // Button Component
    Component {
        id: buttonComp
        Button {
            text: title
            flat: true
            contentItem: RowLayout {
                spacing: 8
                Text { text: title; color: destructive ? theme.error : theme.accent; font.weight: Font.Medium }
                Image { source: "qrc:/icons/arrow-right.svg"; width: 14; height: 14; color: destructive ? theme.error : theme.accent }
            }
            onClicked: action()
        }
    }

    // List Component
    Component {
        id: listComp
        Button {
            text: "Manage " + title
            flat: true
            contentItem: RowLayout {
                spacing: 8
                Text { text: "Manage " + title; color: theme.accent; font.weight: Font.Medium }
                Image { source: "qrc:/icons/arrow-right.svg"; width: 14; height: 14; color: theme.accent }
            }
            onClicked: action()
        }
    }
}