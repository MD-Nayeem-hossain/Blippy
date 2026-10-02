#ifndef BLEEPY_INPUT_MANAGER_H
#define BLEEPY_INPUT_MANAGER_H

#include <QObject>
#include <QString>
#include <QKeyEvent>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Shortcut definition
// ------------------------------------------------------------------------------------------------
struct Shortcut {
    Qt::KeyboardModifiers modifiers;
    Qt::Key key;
    bool isValid() const {
        // Check if we have a valid key and non-empty modifier combination
        return key != Qt::Key::Key_unknown;
    }
};

// ------------------------------------------------------------------------------------------------
// Placeholder result (forward declaration - defined in Types.h)
// ------------------------------------------------------------------------------------------------
// Moved to Types.h to avoid circular dependencies

// ------------------------------------------------------------------------------------------------
// InputManager class
// ------------------------------------------------------------------------------------------------
class InputManager : public QObject {
    Q_OBJECT
public:
    explicit InputManager(QObject *parent = nullptr);
    ~InputManager() override;

    // Enable/disable the keyboard hook
    bool setEnabled(bool enabled);

    // Check if currently enabled
    bool isEnabled() const;

    // Process a key event
    bool processKeyEvent(QKeyEvent *event);

    // Get currently typed text (for combo matching)
    QString currentText() const;

    // Set current text
    void setCurrentText(const QString &text);

    // Handle shortcut trigger
    void handleShortcut(const Shortcut &shortcut);

    // Static instance access
    static InputManager &instance();

protected:
    // Timer event for suppression
    void timerEvent(QTimerEvent *event) override;

signals:
    // Emitted when a recognized shortcut is triggered
    void shortcutTriggered(const QString &keyword);
    
    // Emitted when placeholder should be shown
    void showPlaceholderDropdown(int typeId, const QString &currentText);
    
    // Emitted when text should be inserted
    void textInserted(const QString &text);

private:
    // Member variables
    bool m_enabled = false;
    bool m_substitutionActive = false;
    bool m_suppressionTimerActive = false;
    QString m_recordedClipboard;
    QString m_currentText;
    
    // Timer ID for suppression
    int m_suppressionTimerId = -1;
    
    // Shortcut tracking
    Shortcut m_lastShortcut;
    
    // Key state tracking
    QSet<Qt::Key> m_pressedKeys;
    
    // Placeholder type (moved to Types.h inclusion)
    // placeholderType is now accessed via Types.h
};

} // namespace blippy

#endif // BLEEPY_INPUT_MANAGER_H