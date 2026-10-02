#include "StorageManager.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QStandardPaths>
#include <QCryptographicHash>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// StorageManager implementation
// ------------------------------------------------------------------------------------------------
StorageManager::StorageManager(QObject *parent)
    : QObject(parent)
{
}

StorageManager::~StorageManager() = default;

// ------------------------------------------------------------------------------------------------
// Static instance access
// ------------------------------------------------------------------------------------------------
StorageManager &StorageManager::instance() {
    static StorageManager instance;
    return instance;
}

// ------------------------------------------------------------------------------------------------
// Initialize storage paths
// ------------------------------------------------------------------------------------------------
bool StorageManager::initialize() {
    // Get config directory
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    m_storagePath = configDir + "/blippy";
    m_backupPath = m_storagePath + "/backups";
    m_configFile = m_storagePath + "/settings.json";
    m_combosFile = m_storagePath + "/combos.json";

    // Create directories
    QDir dir;
    if (!dir.mkpath(m_storagePath)) {
        qWarning() << "Failed to create storage directory:" << m_storagePath;
        emit errorOccurred("Failed to create storage directory");
        return false;
    }
    if (!dir.mkpath(m_backupPath)) {
        qWarning() << "Failed to create backup directory:" << m_backupPath;
        return false;
    }

    qDebug() << "Storage initialized at:" << m_storagePath;
    emit storageInitialized(true);
    return true;
}

bool StorageManager::saveCombos(const QList<ComboData> &combos) {
    QJsonArray arr;
    for (const ComboData &combo : combos) {
        arr.append(QJsonDocument(combo.toJson()).object());
    }

    QJsonDocument doc(arr);
    QFile file(m_combosFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open combos file for writing:" << m_combosFile;
        emit errorOccurred("Failed to save combos");
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Saved" << combos.count() << "combos to" << m_combosFile;
    emit combosSaved(true);
    return true;
}

QList<ComboData> StorageManager::loadCombos() {
    QList<ComboData> combos;

    QFile file(m_combosFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Combos file not found, starting fresh:" << m_combosFile;
        emit combosLoaded(0);
        return combos;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isArray()) {
        qWarning() << "Invalid combos file format";
        emit errorOccurred("Invalid combos file format");
        return combos;
    }

    QJsonArray arr = doc.array();
    for (const QJsonValue &val : arr) {
        if (val.isObject()) {
            ComboData data = ComboData::fromJson(val.toObject());
            combos.append(data);
        }
    }

    qDebug() << "Loaded" << combos.count() << "combos from" << m_combosFile;
    emit combosLoaded(combos.count());
    return combos;
}

bool StorageManager::saveSettings(const QJsonObject &settings) {
    QJsonDocument doc(settings);
    QFile file(m_configFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open settings file for writing";
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

QJsonObject StorageManager::loadSettings() {
    QFile file(m_configFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QJsonObject();
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        return QJsonObject();
    }

    return doc.object();
}

bool StorageManager::createBackup(const QString &backupName) {
    QString name = backupName;
    if (name.isEmpty()) {
        name = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    }

    QString backupFile = m_backupPath + "/" + name + ".json";

    // Copy combos file to backup
    if (!QFile::copy(m_combosFile, backupFile)) {
        qWarning() << "Failed to create backup:" << backupFile;
        emit errorOccurred("Failed to create backup");
        return false;
    }

    // Also backup settings
    QString settingsBackup = m_backupPath + "/" + name + "_settings.json";
    QFile::copy(m_configFile, settingsBackup);

    qDebug() << "Backup created:" << backupFile;
    emit backupCreated(name);
    return true;
}

QStringList StorageManager::listBackups() const {
    QStringList backups;
    QDir dir(m_backupPath);
    if (!dir.exists()) {
        return backups;
    }

    QStringList filters;
    filters << "*.json";
    dir.setNameFilters(filters);

    QFileInfoList files = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time | QDir::Reversed);
    for (const QFileInfo &info : files) {
        if (!info.fileName().endsWith("_settings.json")) {
            backups << info.baseName();
        }
    }

    return backups;
}

bool StorageManager::restoreBackup(const QString &backupName) {
    QString backupFile = m_backupPath + "/" + backupName + ".json";
    QString settingsBackup = m_backupPath + "/" + backupName + "_settings.json";

    if (!QFile::exists(backupFile)) {
        qWarning() << "Backup not found:" << backupFile;
        return false;
    }

    // Restore combos
    if (!QFile::copy(backupFile, m_combosFile)) {
        qWarning() << "Failed to restore combos from backup";
        return false;
    }

    // Restore settings if available
    if (QFile::exists(settingsBackup)) {
        QFile::copy(settingsBackup, m_configFile);
    }

    qDebug() << "Backup restored:" << backupName;
    emit backupRestored(backupName);
    return true;
}

bool StorageManager::deleteBackup(const QString &backupName) {
    QString backupFile = m_backupPath + "/" + backupName + ".json";
    QString settingsBackup = m_backupPath + "/" + backupName + "_settings.json";

    bool ok = true;
    if (QFile::exists(backupFile)) {
        ok = QFile::remove(backupFile);
    }
    if (QFile::exists(settingsBackup)) {
        ok = QFile::remove(settingsBackup) && ok;
    }

    return ok;
}

QString StorageManager::storagePath() const {
    return m_storagePath;
}

QString StorageManager::backupPath() const {
    return m_backupPath;
}

} // namespace blippy