#include "ComboModel.h"
#include "types/Types.h"
#include "storage/StorageManager.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QUuid>
#include <QRegularExpression>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// ComboModel implementation
// ------------------------------------------------------------------------------------------------
ComboModel::ComboModel(QObject *parent) : QObject(parent) {
    // Initialize with some default combos if no file exists
    QFile defaultFile(":/default_combos.json");
    if (defaultFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(defaultFile.readAll());
        if (doc.isArray()) {
            QJsonArray arr = doc.array();
            for (const QJsonValue &val : arr) {
                if (val.isObject()) {
                    ComboData data = ComboData::fromJson(val.toObject());
                    m_combos.append(data);
                }
            }
        }
        defaultFile.close();
    }
    // If no default file, that's OK - user will import their own
}

ComboModel::~ComboModel() = default;

// ------------------------------------------------------------------------------------------------
// Static instance access
// ------------------------------------------------------------------------------------------------
ComboModel &ComboModel::instance() {
    static ComboModel model;
    return model;
}

// ------------------------------------------------------------------------------------------------
// Load combos from JSON file path
// ------------------------------------------------------------------------------------------------
bool ComboModel::loadFromPath(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open combo file:" << filePath;
        return false;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    
    if (!doc.isArray()) {
        qWarning() << "Combo file is not a JSON array";
        return false;
    }
    
    m_combos.clear();
    QJsonArray arr = doc.array();
    for (const QJsonValue &val : arr) {
        if (val.isObject()) {
            ComboData data = ComboData::fromJson(val.toObject());
            m_combos.append(data);
        }
    }
    
    qDebug() << "Loaded" << m_combos.count() << "combos from" << filePath;
    emit combosLoaded();
    return true;
}

// ------------------------------------------------------------------------------------------------
// Save combos to JSON file path
// ------------------------------------------------------------------------------------------------
bool ComboModel::saveToPath(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not save combo file:" << filePath;
        return false;
    }
    
    QJsonArray arr;
    for (const ComboData &data : m_combos) {
        arr.append(QJsonDocument(data.toJson()).object());
    }
    
    QJsonDocument doc(arr);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    qDebug() << "Saved" << m_combos.count() << "combos to" << filePath;
    emit combosSaved();
    return true;
}

// ------------------------------------------------------------------------------------------------
// Add a new combo
// ------------------------------------------------------------------------------------------------
bool ComboModel::addCombo(const ComboData &combo) {
    // Check if keyword already exists
    if (hasKeyword(combo.keyword)) {
        qWarning() << "Combo with keyword already exists:" << combo.keyword;
        return false;
    }
    
    m_combos.append(combo);
    // Sort by keyword for easy lookup
    std::sort(m_combos.begin(), m_combos.end(), [](const ComboData &a, const ComboData &b) {
        return a.keyword < b.keyword;
    });
    
    emit comboAdded();
    qDebug() << "Added combo:" << combo.keyword;
    return true;
}

// ------------------------------------------------------------------------------------------------
// Remove a combo by keyword
// ------------------------------------------------------------------------------------------------
bool ComboModel::removeCombo(const QString &keyword) {
    for (int i = 0; i < m_combos.size(); ++i) {
        if (m_combos[i].keyword == keyword) {
            m_combos.removeAt(i);
            emit comboRemoved();
            qDebug() << "Removed combo:" << keyword;
            return true;
        }
    }
    qWarning() << "Combo not found for removal:" << keyword;
    return false;
}

// ------------------------------------------------------------------------------------------------
// Get combo by keyword
// ------------------------------------------------------------------------------------------------
ComboData *ComboModel::comboByKeyword(const QString &keyword) {
    for (ComboData &data : m_combos) {
        if (data.keyword == keyword) {
            return &data;
        }
    }
    return nullptr;
}

// ------------------------------------------------------------------------------------------------
// Get all enabled combos
// ------------------------------------------------------------------------------------------------
QList<ComboData> ComboModel::enabledCombos() const {
    QList<ComboData> result;
    for (const ComboData &data : m_combos) {
        if (data.enabled) {
            result.append(data);
        }
    }
    return result;
}

// ------------------------------------------------------------------------------------------------
// Count of combos
// ------------------------------------------------------------------------------------------------
int ComboModel::count() const {
    return m_combos.size();
}

// ------------------------------------------------------------------------------------------------
// Check if keyword exists
// ------------------------------------------------------------------------------------------------
bool ComboModel::hasKeyword(const QString &keyword) const {
    for (const ComboData &data : m_combos) {
        if (data.keyword == keyword) {
            return true;
        }
    }
    return false;
}

// ------------------------------------------------------------------------------------------------
// Get all keywords
// ------------------------------------------------------------------------------------------------
QStringList ComboModel::allKeywords() const {
    QStringList keywords;
    for (const ComboData &data : m_combos) {
        keywords.append(data.keyword);
    }
    return keywords;
}

// ------------------------------------------------------------------------------------------------
// Load combos from storage
// ------------------------------------------------------------------------------------------------
void ComboModel::loadCombos() {
    StorageManager &storage = StorageManager::instance();
    QList<ComboData> loaded = storage.loadCombos();
    m_combos = loaded;
    emit combosLoaded();
}

// ------------------------------------------------------------------------------------------------
// Find combos matching input
// ------------------------------------------------------------------------------------------------
QList<ComboData> ComboModel::matchingCombos(const QString &input) const {
    QList<ComboData> result;
    for (const ComboData &data : m_combos) {
        if (data.matches(input)) {
            result.append(data);
        }
    }
    return result;
}

// ------------------------------------------------------------------------------------------------
// ComboData::evaluatePlaceholders implementation
// ------------------------------------------------------------------------------------------------
QString ComboData::evaluatePlaceholders(const QString &inputText) const {
    QString result = snippet;
    
    // Replace #{text:key} with input
    QRegularExpression textRx("#\\{text:\\s*(\\w+)\\}");
    auto match = textRx.match(result);
    if (match.hasMatch()) {
        QString key = match.captured(1);
        // In real app, would look up user input for this key
        result.replace(match.captured(0), inputText.isEmpty() ? "[Enter text]" : inputText);
    }
    
    // Replace #{time}
    QRegularExpression timeRx("#\\{time\\}");
    result.replace(timeRx, QTime::currentTime().toString("HH:mm"));
    
    // Replace #{date}
    QRegularExpression dateRx("#\\{date\\}");
    result.replace(dateRx, QDate::currentDate().toString("yyyy-MM-dd"));
    
    // Replace #{day}
    QRegularExpression dayRx("#\\{day\\}");
    result.replace(dayRx, QDate::currentDate().toString("dddd"));
    
    return result;
}

} // namespace blippy