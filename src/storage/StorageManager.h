#ifndef BLEEPY_STORAGE_MANAGER_H
#define BLEEPY_STORAGE_MANAGER_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDateTime>
#include <QStandardPaths>
#include "types/Types.h"

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Storage manager - handles local storage and backup
// ------------------------------------------------------------------------------------------------
class StorageManager : public QObject {
    Q_OBJECT
public:
    explicit StorageManager(QObject *parent = nullptr);
    ~StorageManager() override;

    // Static instance access
    static StorageManager &instance();

    // Initialize storage paths
    bool initialize();

    // Save combos to local storage
    bool saveCombos(const QList<ComboData> &combos);

    // Load combos from local storage
    QList<ComboData> loadCombos();

    // Save settings
    bool saveSettings(const QJsonObject &settings);

    // Load settings
    QJsonObject loadSettings();

    // Create backup
    bool createBackup(const QString &backupName = QString());

    // List available backups
    QStringList listBackups() const;

    // Restore from backup
    bool restoreBackup(const QString &backupName);

    // Delete backup
    bool deleteBackup(const QString &backupName);

    // Get storage path
    QString storagePath() const;

    // Get backup path
    QString backupPath() const;

signals:
    void storageInitialized(bool success);
    void combosSaved(bool success);
    void combosLoaded(int count);
    void backupCreated(const QString &name);
    void backupRestored(const QString &name);
    void errorOccurred(const QString &message);

private:
    QString m_storagePath;
    QString m_backupPath;
    QString m_configFile;
    QString m_combosFile;
};

} // namespace blippy

#endif // BLEEPY_STORAGE_MANAGER_H