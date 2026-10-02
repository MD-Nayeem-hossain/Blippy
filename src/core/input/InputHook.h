#ifndef BLEEPY_INPUT_HOOK_H
#define BLEEPY_INPUT_HOOK_H

#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QAbstractNativeEventFilter>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Cross-platform keyboard/mouse hook interface
// ------------------------------------------------------------------------------------------------
class InputHook : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT
public:
    explicit InputHook(QObject *parent = nullptr);
    ~InputHook() override;

    // Enable/disable the hook
    bool setEnabled(bool enabled);

    // Check if currently enabled
    bool isEnabled() const;

    // Process key event
    bool processKeyEvent(QKeyEvent *event);

    // Process mouse event
    bool processMouseEvent(QMouseEvent *event);

signals:
    void keyPressed(int virtualKey, const QString &text, Qt::KeyboardModifiers modifiers);
    void keyReleased(int virtualKey, const QString &text, Qt::KeyboardModifiers modifiers);
    void mouseClicked(Qt::MouseButton button, const QPoint &pos);
    void shortcutDetected(const QString &keyword);

protected:
    // QAbstractNativeEventFilter interface
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

private:
    bool m_enabled = false;

    // Platform-specific implementation
    class Private;
    std::unique_ptr<Private> d_ptr;
};

} // namespace blippy

#endif // BLEEPY_INPUT_HOOK_H