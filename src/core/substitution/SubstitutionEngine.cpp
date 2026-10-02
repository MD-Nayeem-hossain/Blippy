#include "SubstitutionEngine.h"

#include <QDebug>
#include <QClipboard>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QTimer>
#include <QThread>
#include <QRandomGenerator>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// SubstitutionEngine implementation
// ------------------------------------------------------------------------------------------------
SubstitutionEngine::SubstitutionEngine(QObject *parent)
    : QObject(parent)
    , m_insertMethod(InsertMethod::Clipboard)
    , m_suppressed(false)
    , m_suppressionTimerId(-1)
{
}

SubstitutionEngine::~SubstitutionEngine() {
    if (m_suppressionTimerId >= 0) {
        killTimer(m_suppressionTimerId);
    }
}

// ------------------------------------------------------------------------------------------------
// Set insertion method
// ------------------------------------------------------------------------------------------------
void SubstitutionEngine::setInsertMethod(InsertMethod method) {
    m_insertMethod = method;
}

bool SubstitutionEngine::substitute(const QString &keyword, const QString &snippet, const QString &inputText) {
    qDebug() << "Substituting:" << keyword << "->" << snippet;

    emit substitutionStarted(keyword);

    // Evaluate placeholders in snippet
    QString evaluated = snippet;

    // Replace #{time}
    QRegularExpression timeRx("#\\{time\\}");
    evaluated.replace(timeRx, QTime::currentTime().toString("HH:mm"));

    // Replace #{date}
    QRegularExpression dateRx("#\\{date\\}");
    evaluated.replace(dateRx, QDate::currentDate().toString("yyyy-MM-dd"));

    // Replace #{day}
    QRegularExpression dayRx("#\\{day\\}");
    evaluated.replace(dayRx, QDate::currentDate().toString("dddd"));

    // Replace #{text:key} with inputText
    QRegularExpression textRx("#\\{text:\\s*(\\w+)\\}");
    auto match = textRx.match(evaluated);
    if (match.hasMatch()) {
        evaluated.replace(match.captured(0), inputText.isEmpty() ? "[Enter text]" : inputText);
    }

    // Replace #{emoji} with default
    QRegularExpression emojiRx("#\\{emoji\\}");
    evaluated.replace(emojiRx, "😀");

    // Replace #{random:N}
    QRegularExpression randomRx("#\\{random:(\\d+)\\}");
    auto randomMatch = randomRx.match(evaluated);
    if (randomMatch.hasMatch()) {
        bool ok = false;
        int length = randomMatch.captured(1).toInt(&ok);
        if (ok && length > 0) {
            QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
            QString random;
            for (int i = 0; i < length; ++i) {
                random += chars.at(QRandomGenerator::global()->bounded(chars.length()));
            }
            evaluated.replace(randomMatch.captured(0), random);
        }
    }

    // Replace #{remaining:YYYY-MM-DD}
    QRegularExpression remainingRx("#\\{remaining:(\\d{4}-\\d{2}-\\d{2})\\}");
    auto remainingMatch = remainingRx.match(evaluated);
    if (remainingMatch.hasMatch()) {
        QDate targetDate = QDate::fromString(remainingMatch.captured(1), "yyyy-MM-dd");
        if (targetDate.isValid()) {
            QDate current = QDate::currentDate();
            qint32 daysRemaining = current.daysTo(targetDate);
            if (daysRemaining >= 0) {
                evaluated.replace(remainingMatch.captured(0), QString("%1 day(s) remaining").arg(daysRemaining));
            } else {
                evaluated.replace(remainingMatch.captured(0), QString("Expired %1 day(s) ago").arg(-daysRemaining));
            }
        }
    }

    // Replace #{shortcut:KEY} - would trigger another substitution
    // For now, just mark it

    bool success = false;

    if (m_insertMethod == InsertMethod::Clipboard) {
        success = insertByClipboard(evaluated);
    } else {
        success = insertByKeystrokes(evaluated);
    }

    if (success) {
        startSuppression(300);
    }

    emit substitutionCompleted(keyword, success);
    return success;
}

bool SubstitutionEngine::insertByClipboard(const QString &text) {
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (!clipboard) {
        qWarning() << "No clipboard available";
        return false;
    }

    // Backup current clipboard
    m_recordedClipboard = clipboard->text();

    // Set new text
    clipboard->setText(text);

    // Simulate Ctrl+V (or Shift+Insert)
    QKeyEvent pressEvent(QEvent::KeyPress, Qt::Key_V, Qt::ControlModifier);
    QKeyEvent releaseEvent(QEvent::KeyRelease, Qt::Key_V, Qt::ControlModifier);

    QCoreApplication::postEvent(QGuiApplication::focusObject(), &pressEvent);
    QCoreApplication::postEvent(QGuiApplication::focusObject(), &releaseEvent);

    // Schedule clipboard restoration after 1 second
    QTimer::singleShot(1000, [this]() {
        QClipboard *clipboard = QGuiApplication::clipboard();
        if (clipboard && !m_recordedClipboard.isEmpty()) {
            clipboard->setText(m_recordedClipboard);
            m_recordedClipboard.clear();
        }
    });

    return true;
}

bool SubstitutionEngine::insertByKeystrokes(const QString &text) {
    // Simulate typing each character
    for (QChar c : text) {
        if (c == QChar::LineFeed) {
            QKeyEvent press(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
            QKeyEvent release(QEvent::KeyRelease, Qt::Key_Return, Qt::NoModifier);
            QCoreApplication::postEvent(QGuiApplication::focusObject(), &press);
            QCoreApplication::postEvent(QGuiApplication::focusObject(), &release);
        } else {
            QKeyEvent press(QEvent::KeyPress, c.unicode(), Qt::NoModifier);
            QKeyEvent release(QEvent::KeyRelease, c.unicode(), Qt::NoModifier);
            QCoreApplication::postEvent(QGuiApplication::focusObject(), &press);
            QCoreApplication::postEvent(QGuiApplication::focusObject(), &release);
        }
        QThread::msleep(5); // Small delay between keystrokes
    }
    return true;
}

void SubstitutionEngine::startSuppression(int ms) {
    if (m_suppressionTimerId >= 0) {
        killTimer(m_suppressionTimerId);
    }
    m_suppressed = true;
    m_suppressionTimerId = startTimer(ms);
    emit suppressionStarted(ms);
    qDebug() << "Substitution suppression started:" << ms << "ms";
}

bool SubstitutionEngine::isSuppressed() const {
    return m_suppressed;
}

void SubstitutionEngine::timerEvent(QTimerEvent *event) {
    if (event->timerId() == m_suppressionTimerId) {
        killTimer(m_suppressionTimerId);
        m_suppressionTimerId = -1;
        m_suppressed = false;
        emit suppressionEnded();
        qDebug() << "Substitution suppression ended";
    }
}

} // namespace blippy