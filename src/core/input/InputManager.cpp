#include "InputManager.h"
#include "types/Types.h"  // For BlippyConfig and PlaceholderType

#include <QDebug>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QClipboard>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Config access - singleton
// ------------------------------------------------------------------------------------------------
BlippyConfig &config() {
    static BlippyConfig instance;
    return instance;
}

// ------------------------------------------------------------------------------------------------
// InputManager implementation
// ------------------------------------------------------------------------------------------------
InputManager::InputManager(QObject *parent)
    : QObject(parent)
    , m_enabled(false)
    , m_substitutionActive(false)
    , m_suppressionTimerActive(false)
    , m_suppressionTimerId(-1)
{
    // Constructor - hook will be enabled on demand
}

InputManager::~InputManager() {
    // Disable hook on destruction
    setEnabled(false);
}

bool InputManager::setEnabled(bool enabled) {
    m_enabled = enabled;
    
    if (enabled) {
        // Platform-specific: enable keyboard hook
        // Windows: SetWindowsHookEx(WH_KEYBOARD_LL, ...)
        // Linux: X11/Wayland hook
        // macOS: Event tap
        qWarning() << "Input hook enable not fully implemented - stub";
        return true;
    } else {
        // Disable hook and clear suppression
        if (m_suppressionTimerId >= 0) {
            killTimer(m_suppressionTimerId);
            m_suppressionTimerId = -1;
            m_suppressionTimerActive = false;
        }
        m_substitutionActive = false;
        m_recordedClipboard.clear();
        qWarning() << "Input hook disable not fully implemented - stub";
        return true;
    }
}

bool InputManager::isEnabled() const {
    return m_enabled;
}

bool InputManager::processKeyEvent(QKeyEvent *event) {
    if (!m_enabled) {
        // Pass event through to next hook
        return true;
    }
    
    // Double-paste prevention check
    if (m_substitutionActive && m_suppressionTimerActive) {
        // During suppression window, intercept Ctrl+V
        if (event->modifiers() & Qt::ControlModifier && 
            (event->key() == Qt::Key_V || event->key() == Qt::Key_Insert)) {
            // Show tooltip and defer paste
            Q_EMIT shortcutTriggered("deferred_paste");
            qDebug() << "Ctrl+V during suppression - deferred";
            return true; // Event handled (suppressed)
        }
        // Ignore other keys during suppression
        qDebug() << "Key suppressed during substitution";
        return true;
    }
    
    // Process normal key event
    // Update current text buffer
    if (event->type() == QKeyEvent::KeyPress) {
        // Add character to current text (simplified)
        QString text = event->text();
        if (!text.isEmpty()) {
            m_currentText += text;
            // Emit character typed signal logic would go here
        }
        
        // Check for shortcut patterns
        // In a full implementation, this would check against registered shortcuts
    }
    
    return true;
}

void InputManager::handleShortcut(const Shortcut &shortcut) {
    qDebug() << "Shortcut handled:" << int(shortcut.key);
    
    // Mark substitution as active and start suppression timer
    m_substitutionActive = true;
    m_suppressionTimerActive = true;
    
    // Start 300ms suppression timer
    if (m_suppressionTimerId < 0) {
        m_suppressionTimerId = startTimer(300);
        qDebug() << "Started substitution suppression timer (300ms)";
        
        // Record clipboard content before substitution
        QClipboard *clipboard = QGuiApplication::clipboard();
        if (clipboard) {
            m_recordedClipboard = clipboard->text().toUtf8().constData();
            qDebug() << "Recorded clipboard (first 50 chars):" 
                     << (m_recordedClipboard.length() > 50 ? m_recordedClipboard.left(50) + "..." : m_recordedClipboard);
        }
    }
}

void InputManager::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);
    
    if (m_suppressionTimerId >= 0) {
        // Timer expired - clear suppression
        killTimer(m_suppressionTimerId);
        m_suppressionTimerId = -1;
        m_suppressionTimerActive = false;
        m_substitutionActive = false;
        
        // Restore recorded clipboard content
        QClipboard *clipboard = QGuiApplication::clipboard();
        if (clipboard) {
            clipboard->setText(m_recordedClipboard);
            qDebug() << "Substitution suppression expired - clipboard restored";
        }
        
        // Emit that substitution is complete
        Q_EMIT shortcutTriggered("substitution_complete");
    }
}

QString InputManager::currentText() const {
    return m_currentText;
}

void InputManager::setCurrentText(const QString &text) {
    m_currentText = text;
}

// ------------------------------------------------------------------------------------------------
// InputManager instance access
// ------------------------------------------------------------------------------------------------
InputManager &InputManager::instance() {
    static InputManager instance;
    return instance;
}

} // namespace blippy