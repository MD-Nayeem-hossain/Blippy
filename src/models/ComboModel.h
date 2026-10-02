#ifndef BLEEPY_COMBO_MODEL_H
#define BLEEPY_COMBO_MODEL_H

#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include "types/Types.h"

namespace blippy {

// Main combo model - singleton
class ComboModel : public QObject {
    Q_OBJECT
public:
    explicit ComboModel(QObject *parent = nullptr);
    ~ComboModel() override;
    
    // Static instance access
    static ComboModel &instance();
    
    // Load combos from JSON file path
    bool loadFromPath(const QString &filePath);
    
    // Save combos to JSON file path
    bool saveToPath(const QString &filePath);
    
    // Add a new combo
    bool addCombo(const ComboData &combo);
    
    // Remove a combo by keyword
    bool removeCombo(const QString &keyword);
    
    // Get combo by keyword
    ComboData *comboByKeyword(const QString &keyword);
    
    // Get all enabled combos
    QList<ComboData> enabledCombos() const;
    
    // Count of combos
    int count() const;
    
    // Check if keyword exists
    bool hasKeyword(const QString &keyword) const;
    
    // Get all keywords
    QStringList allKeywords() const;
    
    // Find combos matching input
    QList<ComboData> matchingCombos(const QString &input) const;

    // Load combos from storage
    void loadCombos();
    
signals:
    // Emitted when combos are loaded/saved
    void combosLoaded();
    void combosSaved();
    void comboAdded();
    void comboRemoved();
    
private:
    Q_DISABLE_COPY(ComboModel)
    QList<ComboData> m_combos;
    QString m_filePath;
};

} // namespace blippy

#endif // BLEEPY_COMBO_MODEL_H