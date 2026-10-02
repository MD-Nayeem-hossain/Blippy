#ifndef BLEEPY_SUBSTITUTION_ENGINE_H
#define BLEEPY_SUBSTITUTION_ENGINE_H

#include <QObject>
#include <QString>
#include <QClipboard>
#include <QGuiApplication>
#include <QTimer>
#include "types/Types.h"

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Substitution engine - handles text expansion and insertion
// ------------------------------------------------------------------------------------------------
class SubstitutionEngine : public QObject {
    Q_OBJECT
public:
    explicit SubstitutionEngine(QObject *parent = nullptr);
    ~SubstitutionEngine() override;

    // Set insertion method
    void setInsertMethod(InsertMethod method);

    // Perform substitution with placeholder evaluation
    bool substitute(const QString &keyword, const QString &snippet, const QString &inputText = QString());

    // Insert text via clipboard (most apps)
    bool insertByClipboard(const QString &text);

    // Insert text via keystroke simulation (sensitive apps)
    bool insertByKeystrokes(const QString &text);

    // Start substitution suppression (double-paste prevention)
    void startSuppression(int ms = 300);

    // Check if substitution is suppressed
    bool isSuppressed() const;

signals:
    void substitutionStarted(const QString &keyword);
    void substitutionCompleted(const QString &keyword, bool success);
    void suppressionStarted(int ms);
    void suppressionEnded();

private:
    InsertMethod m_insertMethod = InsertMethod::Clipboard;
    bool m_suppressed = false;
    int m_suppressionTimerId = -1;
    QString m_recordedClipboard;

    void timerEvent(QTimerEvent *event) override;
};

} // namespace blippy

#endif // BLEEPY_SUBSTITUTION_ENGINE_H