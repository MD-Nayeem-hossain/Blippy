#include "PlaceholderEngine.h"
#include "types/Types.h"
#include "models/ComboModel.h"

#include <QDebug>
#include <QRegularExpression>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QRandomGenerator>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// PlaceholderEngine implementation
// ------------------------------------------------------------------------------------------------
PlaceholderEngine::PlaceholderEngine(QObject *parent)
    : QObject(parent)
    , m_comboModel(nullptr)
    , m_currentContext()
{
}

PlaceholderEngine::~PlaceholderEngine() = default;

void PlaceholderEngine::setComboModel(ComboModel *model) {
    m_comboModel = model;
}

EvaluationResult PlaceholderEngine::evaluate(const QString &placeholderText) {
    EvaluationResult result;
    result.success = false;
    result.needsDialog = false;
    result.dialogPrompt = QString();
    result.type = PlaceholderType::Text;
    result.resultText = placeholderText;
    
    QString text = placeholderText.trimmed();
    
    // Check if it starts with # (placeholder trigger)
    if (text.isEmpty() || text[0] != '#') {
        result.resultText = text;
        return result;
    }
    
    // Remove the leading #
    QString content = text.mid(1);
    
    // Check for placeholder types
    // #{text} → custom text input
    if (content.startsWith("text")) {
        // Extract the key name if present: #{text:greeting} or just #text
        QString keyName;
        QString rest = content.mid(5); // Skip "text"
        
        if (rest.startsWith(':')) {
            keyName = rest.mid(1);
        } else if (!rest.isEmpty()) {
            keyName = rest;
        }
        
        result.type = PlaceholderType::Text;
        result.needsDialog = true;
        result.dialogPrompt = keyName.isEmpty() ? "Enter text:" : QString("Enter %1:").arg(keyName);
        result.dialogPrompt += " (or cancel)";
        
        qDebug() << "Placeholder needs text input - prompt:" << result.dialogPrompt;
        return result;
    }
    
    // #{time} → current time
    if (content == "time") {
        result.type = PlaceholderType::Time;
        QTime time = QTime::currentTime();
        result.resultText = time.toString("HH:mm");
        qDebug() << "Placeholder time:" << result.resultText;
        return result;
    }
    
    // #{date} → today's date
    if (content == "date") {
        result.type = PlaceholderType::Date;
        QDate date = QDate::currentDate();
        result.resultText = date.toString("yyyy-MM-dd");
        qDebug() << "Placeholder date:" << result.resultText;
        return result;
    }
    
    // #{day} → day name
    if (content == "day") {
        result.type = PlaceholderType::Day;
        QDate date = QDate::currentDate();
        QString dayName = date.toString("dddd"); // Monday, Tuesday, etc.
        result.resultText = dayName;
        qDebug() << "Placeholder day:" << result.resultText;
        return result;
    }
    
    // #{remaining:YYYY-MM-DD} → time remaining
    if (content.startsWith("remaining:")) {
        result.type = PlaceholderType::Remaining;
        QString dateStr = content.mid(12); // Skip "remaining:"
        QDate targetDate = QDate::fromString(dateStr, "yyyy-MM-dd");
        
        if (targetDate.isValid()) {
            QDate current = QDate::currentDate();
            qint32 daysRemaining = current.daysTo(targetDate);
            if (daysRemaining >= 0) {
                result.resultText = QString("%1 day(s) remaining").arg(daysRemaining);
            } else {
                result.resultText = QString("Expired %1 day(s) ago").arg(-daysRemaining);
            }
        } else {
            result.resultText = "Invalid date format";
        }
        qDebug() << "Placeholder remaining:" << result.resultText;
        return result;
    }
    
    // #{emoji} → emoji selector
    if (content == "emoji") {
        result.type = PlaceholderType::Emoji;
        result.resultText = "😀"; // Default emoji - real app would show selector
        qDebug() << "Placeholder emoji (placeholder)";
        return result;
    }
    
    // #{random:N} → N-character random string
    if (content.startsWith("random:")) {
        result.type = PlaceholderType::Random;
        bool ok = false;
        int length = content.mid(7).toInt(&ok);
        if (ok && length > 0) {
            QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
            QString random;
            // Use QRandomGenerator instead of qsrand/qrand
            for (int i = 0; i < length; ++i) {
                random += chars.at(QRandomGenerator::global()->bounded(chars.length()));
            }
            result.resultText = random;
        } else {
            result.resultText = "Invalid random length";
        }
        qDebug() << "Placeholder random:" << result.resultText;
        return result;
    }
    
    // #{shortcut:KEY} → trigger shortcut resolution
    if (content.startsWith("shortcut:")) {
        result.type = PlaceholderType::Shortcut;
        QString key = content.mid(9); // Skip "shortcut:"
        // Call resolveShortcut and fill result
        bool resolved = resolveShortcut(key);
        if (resolved) {
            result.success = true;
            // The shortcutResolved signal will have the expansion
        } else {
            result.resultText = "Shortcut not found: " + key;
        }
        return result;
    }
    
    // Default: treat as custom text placeholder
    result.type = PlaceholderType::Text;
    result.needsDialog = true;
    result.dialogPrompt = QString("Enter text for: %1").arg(content);
    result.dialogPrompt += " (or cancel)";
    
    qDebug() << "Placeholder default text input - content:" << content;
    return result;
}

bool PlaceholderEngine::resolveShortcut(const QString &text) {
    // Try to resolve a shortcut like "pt" → "Payments/Treasury"
    if (text.isEmpty()) {
        qWarning() << "Empty shortcut text";
        return false;
    }
    
    // Look up in combo model
    if (m_comboModel) {
        // Search for combo with matching keyword
        // In a full implementation, this would search the combo list
        QString keyword = text;
        QString expansion = "Expanded: " + keyword; // Placeholder expansion
        
        qDebug() << "Resolving shortcut:" << keyword << "->" << expansion;
        
        emit shortcutResolved(keyword, expansion);
        return true;
    }
    
    qWarning() << "No combo model set for shortcut resolution";
    return false;
}

QString PlaceholderEngine::currentContext() const {
    return m_currentContext;
}

void PlaceholderEngine::setCurrentContext(const QString &context) {
    m_currentContext = context;
}

// ------------------------------------------------------------------------------------------------
// Static instance access
// ------------------------------------------------------------------------------------------------
PlaceholderEngine &PlaceholderEngine::instance() {
    static PlaceholderEngine engine;
    return engine;
}

} // namespace blippy