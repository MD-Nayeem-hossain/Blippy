#include "InputHook.h"

#include <QDebug>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QAbstractNativeEventFilter>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Platform-specific implementation (Windows)
// ------------------------------------------------------------------------------------------------
#ifdef Q_OS_WIN

#include <windows.h>
#include <QEvent>

struct InputHook::Private {
    HHOOK keyboardHook = nullptr;
    HHOOK mouseHook = nullptr;
    QString currentText;
    QSet<int> pressedKeys;

    static LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
            KBDLLHOOKSTRUCT *kb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            // Process key event - would emit signal to main thread
        }
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    static LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode >= 0 && (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)) {
            // Process mouse event
        }
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }
};

InputHook::InputHook(QObject *parent)
    : QObject(parent)
    , d_ptr(std::make_unique<Private>())
{
    qApp->installNativeEventFilter(this);
}

InputHook::~InputHook() {
    setEnabled(false);
    qApp->removeNativeEventFilter(this);
}

bool InputHook::setEnabled(bool enabled) {
    if (enabled == m_enabled) return true;

    if (enabled) {
        // Install Windows hooks
        d_ptr->keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, Private::keyboardProc, nullptr, 0);
        d_ptr->mouseHook = SetWindowsHookEx(WH_MOUSE_LL, Private::mouseProc, nullptr, 0);

        if (d_ptr->keyboardHook && d_ptr->mouseHook) {
            m_enabled = true;
            qDebug() << "Windows keyboard/mouse hooks installed";
            return true;
        } else {
            if (d_ptr->keyboardHook) UnhookWindowsHookEx(d_ptr->keyboardHook);
            if (d_ptr->mouseHook) UnhookWindowsHookEx(d_ptr->mouseHook);
            qWarning() << "Failed to install Windows hooks";
            return false;
        }
    } else {
        if (d_ptr->keyboardHook) UnhookWindowsHookEx(d_ptr->keyboardHook);
        if (d_ptr->mouseHook) UnhookWindowsHookEx(d_ptr->mouseHook);
        d_ptr->keyboardHook = nullptr;
        d_ptr->mouseHook = nullptr;
        m_enabled = false;
        return true;
    }
}

bool InputHook::isEnabled() const {
    return m_enabled;
}

bool InputHook::processKeyEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    return true;
}

bool InputHook::processMouseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    return true;
}

bool InputHook::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
    Q_UNUSED(eventType);
    Q_UNUSED(message);
    Q_UNUSED(result);
    return false;
}

#endif // Q_OS_WIN

// ------------------------------------------------------------------------------------------------
// Platform-specific implementation (Linux - X11/Wayland)
// ------------------------------------------------------------------------------------------------
#ifdef Q_OS_LINUX

struct InputHook::Private {
    // Stub implementation for Linux without XRecord
    QString currentText;
    QSet<int> pressedKeys;
};

InputHook::InputHook(QObject *parent)
    : QObject(parent)
    , d_ptr(std::make_unique<Private>())
{
    qApp->installNativeEventFilter(this);
    qDebug() << "Linux input hook initialized (stub implementation)";
}

InputHook::~InputHook() {
    setEnabled(false);
    qApp->removeNativeEventFilter(this);
}

bool InputHook::setEnabled(bool enabled) {
    if (enabled == m_enabled) return true;

    if (enabled) {
        m_enabled = true;
        qDebug() << "Linux input hook enabled (stub)";
        return true;
    } else {
        m_enabled = false;
        return true;
    }
}

bool InputHook::isEnabled() const {
    return m_enabled;
}

bool InputHook::processKeyEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    return true;
}

bool InputHook::processMouseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    return true;
}

bool InputHook::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
    Q_UNUSED(eventType);
    Q_UNUSED(message);
    Q_UNUSED(result);
    return false;
}

#endif // Q_OS_LINUX

// ------------------------------------------------------------------------------------------------
// Platform-specific implementation (macOS)
// ------------------------------------------------------------------------------------------------
#ifdef Q_OS_MACOS

#include <ApplicationServices/ApplicationServices.h>

struct InputHook::Private {
    CFMachPortRef eventTap = nullptr;
    CFRunLoopSourceRef runLoopSource = nullptr;
    QString currentText;
    QSet<int> pressedKeys;

    static CGEventRef eventTapCallback(CGEventTapProxy, CGEventType type, CGEventRef event, void *userInfo) {
        InputHook *hook = static_cast<InputHook*>(userInfo);
        // Process event
        return event;
    }
};

InputHook::InputHook(QObject *parent)
    : QObject(parent)
    , d_ptr(std::make_unique<Private>())
{
}

InputHook::~InputHook() {
    setEnabled(false);
}

bool InputHook::setEnabled(bool enabled) {
    if (enabled == m_enabled) return true;

    if (enabled) {
        CGEventMask eventMask = (1 << kCGEventKeyDown) | (1 << kCGEventKeyUp) | (1 << kCGEventFlagsChanged);

        d_ptr->eventTap = CGEventTapCreate(
            kCGSessionEventTap,
            kCGHeadInsertEventTap,
            kCGEventTapOptionDefault,
            eventMask,
            Private::eventTapCallback,
            this
        );

        if (d_ptr->eventTap) {
            d_ptr->runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, d_ptr->eventTap, 0);
            CFRunLoopAddSource(CFRunLoopGetCurrent(), d_ptr->runLoopSource, kCFRunLoopCommonModes);
            CGEventTapEnable(d_ptr->eventTap, true);
            m_enabled = true;
            qDebug() << "macOS Event Tap installed";
            return true;
        }
        qWarning() << "Failed to install macOS Event Tap (may need accessibility permissions)";
        return false;
    } else {
        if (d_ptr->eventTap) {
            CGEventTapEnable(d_ptr->eventTap, false);
            CFRunLoopRemoveSource(CFRunLoopGetCurrent(), d_ptr->runLoopSource, kCFRunLoopCommonModes);
            CFRelease(d_ptr->runLoopSource);
            CFRelease(d_ptr->eventTap);
        }
        d_ptr->eventTap = nullptr;
        d_ptr->runLoopSource = nullptr;
        m_enabled = false;
        return true;
    }
}

bool InputHook::isEnabled() const {
    return m_enabled;
}

bool InputHook::processKeyEvent(QKeyEvent *event) {
    Q_UNUSED(event);
    return true;
}

bool InputHook::processMouseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    return true;
}

bool InputHook::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
    Q_UNUSED(eventType);
    Q_UNUSED(message);
    Q_UNUSED(result);
    return false;
}

#endif // Q_OS_MACOS

} // namespace blippy