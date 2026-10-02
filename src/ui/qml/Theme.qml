pragma Singleton
import QtQuick

QtObject {
    id: themeManager

    property bool isDark: true
    property var current: isDark ? darkTheme : lightTheme

    readonly property var darkTheme: {
        background: "#0d1117",
        backgroundAlt: "#161b22",
        surface: "#161b22",
        sidebarBg: "#0d1117",
        cardBg: "#161b22",
        border: "#30363d",
        textPrimary: "#e6edf3",
        textSecondary: "#8b949e",
        textMuted: "#6e7681",
        accent: "#58a6ff",
        accentAlt: "#1f6feb",
        onAccent: "#ffffff",
        success: "#3fb950",
        warning: "#d29922",
        error: "#f85149",
        placeholder: "#21262d",
        placeholderText: "#8b949e"
    }

    readonly property var lightTheme: {
        background: "#f6f8fa",
        backgroundAlt: "#ffffff",
        surface: "#ffffff",
        sidebarBg: "#ffffff",
        cardBg: "#f6f8fa",
        border: "#d0d7de",
        textPrimary: "#24292f",
        textSecondary: "#57606a",
        textMuted: "#6e7781",
        accent: "#0969da",
        accentAlt: "#0550ae",
        onAccent: "#ffffff",
        success: "#2da44e",
        warning: "#9a6700",
        error: "#cf222e",
        placeholder: "#f3f4f6",
        placeholderText: "#6b7280"
    }

    function toggle() {
        isDark = !isDark
        current = isDark ? darkTheme : lightTheme
    }

    function getColor(name) {
        return current[name]
    }
}