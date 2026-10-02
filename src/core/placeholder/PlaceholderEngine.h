#ifndef BLEEPY_PLACEHOLDER_ENGINE_H
#define BLEEPY_PLACEHOLDER_ENGINE_H

#include <QObject>
#include <QString>
#include "types/Types.h"
#include "models/ComboModel.h"

namespace blippy {

// Result from placeholder evaluation
struct EvaluationResult {
    bool success = false;           // Was evaluation successful?
    QString resultText;             // The resulting text after evaluation
    bool needsDialog = false;       // Whether user input is needed
    QString dialogPrompt;           // Prompt text for user input
    PlaceholderType type = PlaceholderType::Text;   // The placeholder type
};

// Main placeholder engine
class PlaceholderEngine : public QObject {
    Q_OBJECT
public:
    explicit PlaceholderEngine(QObject *parent = nullptr);
    ~PlaceholderEngine() override;

    // Set the combo model for shortcut resolution
    void setComboModel(ComboModel *model);

    // Evaluate a placeholder string
    // Returns result and whether dialog is needed
    EvaluationResult evaluate(const QString &placeholderText);

    // Get current typed text context
    QString currentContext() const;

    // Set current context (text typed so far)
    void setCurrentContext(const QString &context);

    // Trigger shortcut resolution (e.g., "pt" → "Payments/Treasury")
    bool resolveShortcut(const QString &text);

    // Static instance access
    static PlaceholderEngine &instance();

signals:
    // Emitted when placeholder evaluation needs user input
    void placeholderNeedInput(PlaceholderType type, const QString &prompt);

    // Emitted when shortcut was resolved
    void shortcutResolved(const QString &keyword, const QString &expansion);

    // Emitted when placeholder was evaluated
    void placeholderEvaluated(const QString &original, const QString &result);

private:
    ComboModel *m_comboModel = nullptr;
    QString m_currentContext;
};

} // namespace blippy

#endif // BLEEPY_PLACEHOLDER_ENGINE_H