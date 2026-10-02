#ifndef BLEEPY_IMPORT_MANAGER_H
#define BLEEPY_IMPORT_MANAGER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QList>
#include "types/Types.h"

namespace blippy {

// ------------------------------------------------------------------------------------------------
// Import manager - handles migration from Beeftext, Text Blaze, etc.
// ------------------------------------------------------------------------------------------------
class ImportManager : public QObject {
    Q_OBJECT
public:
    explicit ImportManager(QObject *parent = nullptr);
    ~ImportManager() override;

    // Import from Beeftext JSON file
    QList<ComboData> importFromBeeftext(const QString &filePath);

    // Import from Text Blaze CSV/JSON export
    QList<ComboData> importFromTextBlaze(const QString &filePath);

    // Import from generic JSON
    QList<ComboData> importFromJson(const QString &filePath);

    // Detect file format
    enum class Format {
        Unknown,
        Beeftext,
        TextBlazeCSV,
        TextBlazeJSON,
        GenericJSON
    };
    Format detectFormat(const QString &filePath) const;

    // Preview import (without saving)
    struct ImportPreview {
        int totalCombos = 0;
        int validCombos = 0;
        int invalidCombos = 0;
        QStringList errors;
        QList<ComboData> combos;
    };
    ImportPreview previewImport(const QString &filePath);

signals:
    void importStarted(const QString &source);
    void importProgress(int current, int total);
    void importCompleted(bool success, int count, const QStringList &errors);
    void importError(const QString &error);

private:
    QList<ComboData> parseBeeftext(const QJsonObject &obj);
    QList<ComboData> parseTextBlazeCSV(const QString &content);
    QList<ComboData> parseTextBlazeJSON(const QJsonObject &obj);
    QList<ComboData> parseGenericJSON(const QJsonObject &obj);
    ComboData createComboFromBeeftext(const QJsonObject &obj);
    ComboData createComboFromTextBlaze(const QJsonObject &obj);
};

} // namespace blippy

#endif // BLEEPY_IMPORT_MANAGER_H