#ifndef BLEEPY_TYPES_H
#define BLEEPY_TYPES_H

#include <QColor>
#include <QDateTime>
#include <QUuid>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTime>
#include <QDate>
#include <QSet>
#include <QtGlobal>

namespace blippy {

enum class PlaceholderType { Text, Time, Date, Day, Remaining, Emoji, Random, Shortcut };
enum class InsertMethod { Clipboard, Keystrokes };
enum class BackupTarget { Local, Rclone, None };
enum class HookState { Enabled, Disabled, Suppressed };
enum class EMatchingMode { Strict, Loose };
enum class ECaseSensitivity { CaseInsensitive, CaseSensitive };

struct BlippyConfig {
    InsertMethod defaultInsertMethod = InsertMethod::Clipboard;
    BackupTarget backupTarget = BackupTarget::Local;
    bool autoBackup = true;
    qint32 substitutionSuppressionMs = 300;
    QColor accentColor = QColor(66, 135, 245);
    QStringList excludedApplications;
    QStringList defaultKeywords = { "h1", "h2", "addr", "email", "tel" };
    BlippyConfig() = default;
};

struct ComboData {
    QUuid id;
    QString keyword;
    QString snippet;
    QString description;
    QString group;
    QString name;  // Display name
    EMatchingMode matchingMode;
    ECaseSensitivity caseSensitivity;
    bool enabled;
    QDateTime creationDate;
    QDateTime modificationDate;
    QSet<QString> placeholders;
    
    ComboData() 
        : id(QUuid::createUuid())
        , matchingMode(EMatchingMode::Strict)
        , caseSensitivity(ECaseSensitivity::CaseInsensitive)
        , enabled(true)
        , creationDate(QDateTime::currentDateTime())
        , modificationDate(QDateTime::currentDateTime())
    {}
    
    bool isValid() const {
        return !keyword.isEmpty() && !snippet.isEmpty();
    }
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["id"] = id.toString();
        obj["keyword"] = keyword;
        obj["snippet"] = snippet;
        obj["description"] = description;
        obj["group"] = group;
        obj["matchingMode"] = static_cast<int>(matchingMode);
        obj["caseSensitivity"] = static_cast<int>(caseSensitivity);
        obj["enabled"] = enabled;
        obj["creationDate"] = creationDate.toString(Qt::ISODate);
        obj["modificationDate"] = modificationDate.toString(Qt::ISODate);
        
        QJsonArray phArray;
        for (const QString &ph : placeholders) {
            phArray.append(ph);
        }
        obj["placeholders"] = phArray;
        
        return obj;
    }
    
    static ComboData fromJson(const QJsonObject &obj) {
        ComboData data;
        if (obj.contains("id")) data.id = QUuid(obj["id"].toString());
        if (obj.contains("keyword")) data.keyword = obj["keyword"].toString();
        if (obj.contains("snippet")) data.snippet = obj["snippet"].toString();
        if (obj.contains("description")) data.description = obj["description"].toString();
        if (obj.contains("group")) data.group = obj["group"].toString();
        if (obj.contains("matchingMode")) data.matchingMode = 
            static_cast<EMatchingMode>(obj["matchingMode"].toInt(0));
        if (obj.contains("caseSensitivity")) data.caseSensitivity = 
            static_cast<ECaseSensitivity>(obj["caseSensitivity"].toInt(0));
        if (obj.contains("enabled")) data.enabled = obj["enabled"].toBool();
        if (obj.contains("creationDate")) data.creationDate = QDateTime::fromString(
            obj["creationDate"].toString(), Qt::ISODate);
        if (obj.contains("modificationDate")) data.modificationDate = QDateTime::fromString(
            obj["modificationDate"].toString(), Qt::ISODate);
        if (obj.contains("placeholders")) {
            QJsonArray arr = obj["placeholders"].toArray();
            for (const QJsonValue &val : arr) {
                if (val.isString()) {
                    data.placeholders.insert(val.toString());
                }
            }
        }
        return data;
    }
    
    bool matches(const QString &input) const {
        if (keyword.isEmpty()) return false;
        Qt::CaseSensitivity cs = caseSensitivity == ECaseSensitivity::CaseInsensitive ? Qt::CaseInsensitive : Qt::CaseSensitive;
        return input.compare(keyword, cs) == 0;
    }
    
    QString evaluatePlaceholders(const QString &inputText) const;
};

} // namespace blippy

#endif // BLEEPY_TYPES_H