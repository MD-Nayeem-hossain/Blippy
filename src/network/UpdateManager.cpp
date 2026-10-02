#include "UpdateManager.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QDebug>

namespace blippy {

// ------------------------------------------------------------------------------------------------
// UpdateManager implementation
// ------------------------------------------------------------------------------------------------
UpdateManager::UpdateManager(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &UpdateManager::onReplyFinished);
}

UpdateManager::~UpdateManager() = default;

void UpdateManager::checkForUpdates() {
    emit updateCheckStarted();

    QUrl url(QString("https://api.github.com/repos/%1/releases/latest").arg(m_githubRepo));
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Blippy-Updater");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    QNetworkReply *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::downloadProgress,
            this, &UpdateManager::onDownloadProgress);
}

void UpdateManager::setCurrentVersion(const QString &version) {
    m_currentVersion = version;
}

QString UpdateManager::currentVersion() const {
    return m_currentVersion;
}

QString UpdateManager::latestVersion() const {
    return m_latestVersion;
}

QString UpdateManager::releaseNotes() const {
    return m_releaseNotes;
}

void UpdateManager::setGitHubRepo(const QString &repo) {
    m_githubRepo = repo;
}

bool UpdateManager::isUpdateAvailable() const {
    if (m_latestVersion.isEmpty() || m_currentVersion.isEmpty()) {
        return false;
    }

    // Simple version comparison (semantic versioning)
    QStringList currentParts = m_currentVersion.split('.');
    QStringList latestParts = m_latestVersion.split('.');

    for (int i = 0; i < qMax(currentParts.size(), latestParts.size()); ++i) {
        int current = i < currentParts.size() ? currentParts[i].toInt() : 0;
        int latest = i < latestParts.size() ? latestParts[i].toInt() : 0;

        if (latest > current) return true;
        if (latest < current) return false;
    }

    return false;
}

void UpdateManager::onReplyFinished(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        emit updateError("Network error: " + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        emit updateError("Invalid response format");
        reply->deleteLater();
        return;
    }

    QJsonObject obj = doc.object();

    if (obj.contains("tag_name")) {
        m_latestVersion = obj["tag_name"].toString().remove(0, 1); // Remove 'v' prefix
    }

    if (obj.contains("body")) {
        m_releaseNotes = obj["body"].toString();
    }

    if (obj.contains("assets") && obj["assets"].isArray()) {
        QJsonArray assets = obj["assets"].toArray();
        for (const QJsonValue &val : assets) {
            QJsonObject asset = val.toObject();
            if (asset.contains("name") && asset["name"].toString().contains("Blippy")) {
                m_downloadUrl = QUrl(asset["browser_download_url"].toString());
                break;
            }
        }
    }

    if (isUpdateAvailable()) {
        emit updateAvailable(m_latestVersion, m_releaseNotes);
    } else {
        emit noUpdateAvailable();
    }

    reply->deleteLater();
}

void UpdateManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    emit downloadProgress(bytesReceived, bytesTotal);
}

} // namespace blippy