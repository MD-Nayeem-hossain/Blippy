#ifndef BLEEPY_UPDATE_MANAGER_H
#define BLEEPY_UPDATE_MANAGER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Update manager - checks for updates on GitHub
// ------------------------------------------------------------------------------------------------
class UpdateManager : public QObject {
    Q_OBJECT
public:
    explicit UpdateManager(QObject *parent = nullptr);
    ~UpdateManager() override;

    // Check for updates
    void checkForUpdates();

    // Set current version
    void setCurrentVersion(const QString &version);

    // Get current version
    QString currentVersion() const;

    // Get latest version
    QString latestVersion() const;

    // Get release notes
    QString releaseNotes() const;

    // Set GitHub repo (format: "owner/repo")
    void setGitHubRepo(const QString &repo);

    // Check if update is available
    bool isUpdateAvailable() const;

signals:
    void updateCheckStarted();
    void updateAvailable(const QString &version, const QString &releaseNotes);
    void noUpdateAvailable();
    void updateError(const QString &error);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    void onReplyFinished(QNetworkReply *reply);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    QString m_currentVersion = "0.1.0";
    QString m_latestVersion;
    QString m_releaseNotes;
    QString m_githubRepo = "blippy/blippy";
    QUrl m_downloadUrl;
};

} // namespace blippy

#endif // BLEEPY_UPDATE_MANAGER_H