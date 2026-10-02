#include "SyncManager.h"

#include <QDebug>
#include <QProcess>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDateTime>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// SyncManager implementation
// ------------------------------------------------------------------------------------------------
SyncManager::SyncManager(QObject *parent)
    : QObject(parent)
    , m_process(new QProcess(this))
{
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &SyncManager::onProcessFinished);
    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &SyncManager::onProcessReadyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError,
            this, &SyncManager::onProcessReadyReadStandardError);

    findRclone();
}

SyncManager::~SyncManager() = default;

bool SyncManager::findRclone() {
    // Try to find rclone in PATH
    QStringList paths = {
        "rclone",
        "/usr/bin/rclone",
        "/usr/local/bin/rclone",
        "/opt/homebrew/bin/rclone",
        "C:/Program Files/rclone/rclone.exe",
        "C:/rclone/rclone.exe"
    };

    for (const QString &path : paths) {
        QProcess test;
        test.start(path, {"version"});
        if (test.waitForFinished(2000) && test.exitCode() == 0) {
            m_rclonePath = path;
            qDebug() << "Found rclone at:" << m_rclonePath;
            return true;
        }
    }

    qWarning() << "rclone not found in PATH";
    return false;
}

bool SyncManager::initialize(const QString &rcloneConfigPath) {
    if (!findRclone()) {
        emit rcloneNotFound();
        return false;
    }

    if (!rcloneConfigPath.isEmpty()) {
        m_rcloneConfigPath = rcloneConfigPath;
    } else {
        // Default rclone config location
        QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        m_rcloneConfigPath = configDir + "/rclone/rclone.conf";
    }

    qDebug() << "SyncManager initialized with rclone:" << m_rclonePath;
    qDebug() << "Config path:" << m_rcloneConfigPath;

    return true;
}

bool SyncManager::isRcloneAvailable() const {
    return !m_rclonePath.isEmpty();
}

bool SyncManager::isConfigured() const {
    return QFileInfo::exists(m_rcloneConfigPath);
}

bool SyncManager::syncToRemote(const QString &remoteName, const QString &localPath, const QString &remotePath) {
    if (!isRcloneAvailable() || !isConfigured()) {
        emit syncError("rclone not available or not configured");
        return false;
    }

    QStringList args = {
        "sync",
        localPath,
        remoteName + ":" + remotePath,
        "--config", m_rcloneConfigPath,
        "--progress",
        "-v"
    };

    emit syncStarted();
    m_process->start(m_rclonePath, args);
    return true;
}

bool SyncManager::syncFromRemote(const QString &remoteName, const QString &remotePath, const QString &localPath) {
    if (!isRcloneAvailable() || !isConfigured()) {
        emit syncError("rclone not available or not configured");
        return false;
    }

    QStringList args = {
        "sync",
        remoteName + ":" + remotePath,
        localPath,
        "--config", m_rcloneConfigPath,
        "--progress",
        "-v"
    };

    emit syncStarted();
    m_process->start(m_rclonePath, args);
    return true;
}

QStringList SyncManager::listRemotes() const {
    if (!isRcloneAvailable()) {
        return QStringList();
    }

    QProcess proc;
    proc.start(m_rclonePath, {"listremotes", "--config", m_rcloneConfigPath});
    if (proc.waitForFinished(5000)) {
        QString output = proc.readAllStandardOutput();
        return output.split('\n', Qt::SkipEmptyParts);
    }
    return QStringList();
}

QStringList SyncManager::listRemote(const QString &remoteName, const QString &remotePath) const {
    if (!isRcloneAvailable()) {
        return QStringList();
    }

    QProcess proc;
    proc.start(m_rclonePath, {"lsf", remoteName + ":" + remotePath, "--config", m_rcloneConfigPath});
    if (proc.waitForFinished(5000)) {
        QString output = proc.readAllStandardOutput();
        return output.split('\n', Qt::SkipEmptyParts);
    }
    return QStringList();
}

QDateTime SyncManager::lastSyncTime() const {
    return m_lastSyncTime;
}

void SyncManager::setAutoSyncEnabled(bool enabled) {
    m_autoSyncEnabled = enabled;
}

bool SyncManager::isAutoSyncEnabled() const {
    return m_autoSyncEnabled;
}

void SyncManager::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitStatus);

    if (exitCode == 0) {
        m_lastSyncTime = QDateTime::currentDateTime();
        emit syncCompleted(true, "Sync completed successfully");
        qDebug() << "Sync completed successfully";
    } else {
        QString error = m_process->readAllStandardError();
        emit syncCompleted(false, "Sync failed with exit code " + QString::number(exitCode) + ": " + error);
        qWarning() << "Sync failed:" << error;
    }
}

void SyncManager::onProcessReadyReadStandardOutput() {
    QString output = m_process->readAllStandardOutput();
    // Parse progress from rclone output
    // rclone outputs lines like: "Transferred:  1.234 MiB / 1.234 MiB, 100%, 2.345 MiB/s, ETA 0s"
    QRegularExpression progressRx("Transferred:\\s+([\\d.]+)\\s*([KMGT]?iB)\\s*/\\s*([\\d.]+)\\s*([KMGT]?iB),\\s*(\\d+)%");
    auto match = progressRx.match(output);
    if (match.hasMatch()) {
        bool ok = false;
        int percent = match.captured(5).toInt(&ok);
        if (ok) {
            emit syncProgress(percent, "Syncing...");
        }
    }
}

void SyncManager::onProcessReadyReadStandardError() {
    QString error = m_process->readAllStandardError();
    if (!error.trimmed().isEmpty()) {
        qWarning() << "rclone error:" << error;
    }
}

} // namespace blippy