#ifndef BLEEPY_SYNC_MANAGER_H
#define BLEEPY_SYNC_MANAGER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QJsonObject>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Sync manager - handles rclone-based cloud backup (Google Drive, etc.)
// User pre-configures rclone, no OAuth in app
// ------------------------------------------------------------------------------------------------
class SyncManager : public QObject {
    Q_OBJECT
public:
    explicit SyncManager(QObject *parent = nullptr);
    ~SyncManager() override;

    // Initialize with rclone config path
    bool initialize(const QString &rcloneConfigPath = QString());

    // Check if rclone is available
    bool isRcloneAvailable() const;

    // Check if configured
    bool isConfigured() const;

    // Sync local backup to remote (Google Drive, etc.)
    bool syncToRemote(const QString &remoteName, const QString &localPath, const QString &remotePath);

    // Sync from remote to local
    bool syncFromRemote(const QString &remoteName, const QString &remotePath, const QString &localPath);

    // List remotes configured in rclone
    QStringList listRemotes() const;

    // List remote directory contents
    QStringList listRemote(const QString &remoteName, const QString &remotePath) const;

    // Get last sync time
    QDateTime lastSyncTime() const;

    // Set auto-sync enabled
    void setAutoSyncEnabled(bool enabled);

    // Check if auto-sync is enabled
    bool isAutoSyncEnabled() const;

signals:
    void syncStarted();
    void syncProgress(int percent, const QString &status);
    void syncCompleted(bool success, const QString &message);
    void syncError(const QString &error);
    void rcloneNotFound();

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessReadyReadStandardOutput();
    void onProcessReadyReadStandardError();

private:
    QString m_rcloneConfigPath;
    QString m_rclonePath;
    bool m_autoSyncEnabled = false;
    QDateTime m_lastSyncTime;
    QProcess *m_process = nullptr;

    bool findRclone();
    QStringList parseRcloneOutput(const QString &output);
};

} // namespace blippy

#endif // BLEEPY_SYNC_MANAGER_H