#include "ImportManager.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QRegularExpression>
#include <QDateTime>
#include <QUuid>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// ImportManager implementation
// ------------------------------------------------------------------------------------------------
ImportManager::ImportManager(QObject *parent) : QObject(parent) {}

ImportManager::~ImportManager() = default;

ImportManager::Format ImportManager::detectFormat(const QString &filePath) const {
    QFileInfo info(filePath);
    QString suffix = info.suffix().toLower();

    if (suffix == "json") {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();

            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                // Check for Beeftext format
                if (obj.contains("combos") && obj["combos"].isArray()) {
                    return Format::Beeftext;
                }
                // Check for Text Blaze format
                if (obj.contains("snippets") && obj["snippets"].isArray()) {
                    return Format::TextBlazeJSON;
                }
                // Generic JSON with combos array
                if (obj.contains("combos") && obj["combos"].isArray()) {
                    return Format::GenericJSON;
                }
            }
        }
        return Format::GenericJSON;
    } else if (suffix == "csv") {
        return Format::TextBlazeCSV;
    }

    return Format::Unknown;
}

ImportManager::ImportPreview ImportManager::previewImport(const QString &filePath) {
    ImportPreview preview;

    Format format = detectFormat(filePath);
    if (format == Format::Unknown) {
        preview.errors << "Unknown file format";
        return preview;
    }

    QList<ComboData> combos;
    switch (format) {
        case Format::Beeftext:
            combos = importFromBeeftext(filePath);
            break;
        case Format::TextBlazeCSV:
            combos = importFromTextBlaze(filePath);
            break;
        case Format::TextBlazeJSON:
            combos = importFromTextBlaze(filePath);
            break;
        case Format::GenericJSON:
            combos = importFromJson(filePath);
            break;
        default:
            break;
    }

    preview.totalCombos = combos.count();
    for (const ComboData &combo : combos) {
        if (combo.isValid()) {
            preview.validCombos++;
            preview.combos.append(combo);
        } else {
            preview.invalidCombos++;
            preview.errors << QString("Invalid combo: %1").arg(combo.keyword);
        }
    }

    return preview;
}

QList<ComboData> ImportManager::importFromBeeftext(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open Beeftext file:" << filePath;
        return {};
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        qWarning() << "Invalid Beeftext JSON format";
        return {};
    }

    return parseBeeftext(doc.object());
}

QList<ComboData> ImportManager::importFromTextBlaze(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open Text Blaze file:" << filePath;
        return {};
    }

    QFileInfo info(filePath);
    if (info.suffix().toLower() == "csv") {
        QString content = file.readAll();
        file.close();
        return parseTextBlazeCSV(content);
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        return parseTextBlazeJSON(doc.object());
    }
}

QList<ComboData> ImportManager::importFromJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open JSON file:" << filePath;
        return {};
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format";
        return {};
    }

    return parseGenericJSON(doc.object());
}

QList<ComboData> ImportManager::parseBeeftext(const QJsonObject &obj) {
    QList<ComboData> combos;

    if (!obj.contains("combos") || !obj["combos"].isArray()) {
        qWarning() << "Beeftext JSON missing combos array";
        return combos;
    }

    QJsonArray arr = obj["combos"].toArray();
    for (const QJsonValue &val : arr) {
        if (val.isObject()) {
            ComboData combo = createComboFromBeeftext(val.toObject());
            if (combo.isValid()) {
                combos.append(combo);
            }
        }
    }

    return combos;
}

QList<ComboData> ImportManager::parseTextBlazeCSV(const QString &content) {
    QList<ComboData> combos;
    QString mutableContent = content;
    QTextStream stream(&mutableContent);
    QString header = stream.readLine(); // Skip header

    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty()) continue;

        // Simple CSV parsing (name, trigger, snippet)
        QStringList parts = line.split(',');
        if (parts.size() >= 3) {
            ComboData combo;
            combo.keyword = parts[1].trimmed().remove('"');
            combo.snippet = parts[2].trimmed().remove('"');
            combo.name = parts[0].trimmed().remove('"');
            combo.enabled = true;
            combo.creationDate = QDateTime::currentDateTime();
            combo.modificationDate = QDateTime::currentDateTime();
            combo.id = QUuid::createUuid();

            if (combo.isValid()) {
                combos.append(combo);
            }
        }
    }

    return combos;
}

QList<ComboData> ImportManager::parseTextBlazeJSON(const QJsonObject &obj) {
    QList<ComboData> combos;

    if (!obj.contains("snippets") || !obj["snippets"].isArray()) {
        qWarning() << "Text Blaze JSON missing snippets array";
        return combos;
    }

    QJsonArray arr = obj["snippets"].toArray();
    for (const QJsonValue &val : arr) {
        if (val.isObject()) {
            ComboData combo = createComboFromTextBlaze(val.toObject());
            if (combo.isValid()) {
                combos.append(combo);
            }
        }
    }

    return combos;
}

QList<ComboData> ImportManager::parseGenericJSON(const QJsonObject &obj) {
    QList<ComboData> combos;

    if (!obj.contains("combos") || !obj["combos"].isArray()) {
        return combos;
    }

    QJsonArray arr = obj["combos"].toArray();
    for (const QJsonValue &val : arr) {
        if (val.isObject()) {
            ComboData combo = ComboData::fromJson(val.toObject());
            if (combo.isValid()) {
                combos.append(combo);
            }
        }
    }

    return combos;
}

ComboData ImportManager::createComboFromBeeftext(const QJsonObject &obj) {
    ComboData combo;

    // Beeftext format: { "keyword": "...", "snippet": "...", "name": "...", "enabled": true, ... }
    if (obj.contains("keyword")) combo.keyword = obj["keyword"].toString();
    if (obj.contains("snippet")) combo.snippet = obj["snippet"].toString();
    if (obj.contains("name")) combo.name = obj["name"].toString();
    if (obj.contains("description")) combo.description = obj["description"].toString();
    if (obj.contains("enabled")) combo.enabled = obj["enabled"].toBool(true);
    if (obj.contains("group")) combo.group = obj["group"].toString();

    // Handle placeholders
    if (obj.contains("placeholders") && obj["placeholders"].isArray()) {
        QJsonArray phArr = obj["placeholders"].toArray();
        for (const QJsonValue &val : phArr) {
            if (val.isString()) {
                combo.placeholders.insert(val.toString());
            }
        }
    }

    // Auto-detect placeholders in snippet
    QRegularExpression phRx("#\\{(\\w+)(?::([^}]+))?\\}");
    auto match = phRx.match(combo.snippet);
    while (match.hasMatch()) {
        combo.placeholders.insert(match.captured(1));
        match = phRx.match(combo.snippet, match.capturedEnd());
    }

    combo.enabled = true;
    combo.creationDate = QDateTime::currentDateTime();
    combo.modificationDate = QDateTime::currentDateTime();
    combo.id = QUuid::createUuid();

    return combo;
}

ComboData ImportManager::createComboFromTextBlaze(const QJsonObject &obj) {
    ComboData combo;

    // Text Blaze format: { "name": "...", "trigger": "...", "content": "...", "description": "...", "enabled": true }
    if (obj.contains("name")) combo.name = obj["name"].toString();
    if (obj.contains("trigger")) combo.keyword = obj["trigger"].toString();
    if (obj.contains("content")) combo.snippet = obj["content"].toString();
    if (obj.contains("description")) combo.description = obj["description"].toString();
    if (obj.contains("enabled")) combo.enabled = obj["enabled"].toBool(true);

    // Handle placeholders in content
    QRegularExpression phRx("\\{=([^}]+)\\}");
    auto match = phRx.match(combo.snippet);
    while (match.hasMatch()) {
        combo.placeholders.insert(match.captured(1));
        match = phRx.match(combo.snippet, match.capturedEnd());
    }

    combo.enabled = true;
    combo.creationDate = QDateTime::currentDateTime();
    combo.modificationDate = QDateTime::currentDateTime();
    combo.id = QUuid::createUuid();

    return combo;
}

} // namespace blippy