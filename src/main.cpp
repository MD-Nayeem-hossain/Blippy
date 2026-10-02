#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QJSEngine>

#include "core/input/InputManager.h"
#include "core/input/InputHook.h"
#include "core/placeholder/PlaceholderEngine.h"
#include "core/substitution/SubstitutionEngine.h"
#include "models/ComboModel.h"
#include "storage/StorageManager.h"
#include "storage/ImportManager.h"
#include "network/SyncManager.h"
#include "network/UpdateManager.h"
#include "types/Types.h"
#include <QQmlEngine>

// Global accessor functions for singleton managers
namespace blippy {
namespace internal {
    StorageManager &storageManager() {
        static StorageManager instance;
        return instance;
    }
    
    ComboModel &comboModel() {
        static ComboModel instance;
        return instance;
    }
    
    PlaceholderEngine &placeholderEngine() {
        static PlaceholderEngine instance;
        return instance;
    }
    
    SubstitutionEngine &substitutionEngine() {
        static SubstitutionEngine instance;
        return instance;
    }
    
    ImportManager &importManager() {
        static ImportManager instance;
        return instance;
    }
    
    SyncManager &syncManager() {
        static SyncManager instance;
        return instance;
    }
    
    UpdateManager &updateManager() {
        static UpdateManager instance;
        return instance;
    }
    
    InputHook &inputHook() {
        static InputHook instance;
        return instance;
    }
} // namespace internal
} // namespace blippy

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QCoreApplication::setApplicationName("Blippy");
    QCoreApplication::setApplicationVersion("0.1.0");
    QCoreApplication::setOrganizationName("Blippy");
    QCoreApplication::setOrganizationDomain("blippy.app");

    // Use default style to avoid Material dependency issues
    // QQuickStyle::setStyle("Material");

    qDebug() << "Blippy v0.1.0 - Cross-Platform Text Expander";
    qDebug() << "Copyright (c) 2024 Blippy Contributors";

    // Initialize core managers
    blippy::internal::storageManager().initialize();
    blippy::internal::comboModel().loadCombos(); // Load from storage

    blippy::internal::placeholderEngine().setComboModel(&blippy::internal::comboModel());

    blippy::internal::syncManager().initialize();

    blippy::internal::updateManager().setCurrentVersion("0.1.0");
    blippy::internal::updateManager().setGitHubRepo("blippy/blippy");
    blippy::internal::updateManager().checkForUpdates();

    // Initialize input hook (cross-platform)
    blippy::internal::inputHook().setEnabled(true);

    // Register Theme singleton using function provider (more reliable in Qt 6)
    qmlRegisterSingletonType<QObject>("Blippy.Theme", 1, 0, "Theme", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        QQmlComponent component(engine, QUrl("qrc:/qml/Theme.qml"));
        if (component.isError()) {
            qCritical() << "Theme.qml errors:" << component.errors();
            return nullptr;
        }
        QObject *obj = component.create();
        if (!obj) {
            qCritical() << "Failed to create Theme singleton";
            return nullptr;
        }
        qDebug() << "Theme singleton created successfully";
        return obj;
    });
    qDebug() << "Theme singleton registered";
    
    // Also register the types for QML
    qmlRegisterType<blippy::ComboModel>("Blippy.Models", 1, 0, "ComboModel");
    qmlRegisterType<blippy::StorageManager>("Blippy.Storage", 1, 0, "StorageManager");
    qmlRegisterType<blippy::ImportManager>("Blippy.Import", 1, 0, "ImportManager");
    qmlRegisterType<blippy::SyncManager>("Blippy.Sync", 1, 0, "SyncManager");
    qmlRegisterType<blippy::UpdateManager>("Blippy.Update", 1, 0, "UpdateManager");
    qmlRegisterType<blippy::InputHook>("Blippy.Input", 1, 0, "InputHook");
    qmlRegisterType<blippy::PlaceholderEngine>("Blippy.Placeholder", 1, 0, "PlaceholderEngine");
    qmlRegisterType<blippy::SubstitutionEngine>("Blippy.Substitution", 1, 0, "SubstitutionEngine");
    
    qDebug() << "Types registered";

    // Set up QML engine
    QQmlApplicationEngine engine;

    // Expose C++ objects to QML
    engine.rootContext()->setContextProperty("ComboModel", &blippy::internal::comboModel());
    engine.rootContext()->setContextProperty("PlaceholderEngine", &blippy::internal::placeholderEngine());
    engine.rootContext()->setContextProperty("SubstitutionEngine", &blippy::internal::substitutionEngine());
    engine.rootContext()->setContextProperty("StorageManager", &blippy::internal::storageManager());
    engine.rootContext()->setContextProperty("ImportManager", &blippy::internal::importManager());
    engine.rootContext()->setContextProperty("SyncManager", &blippy::internal::syncManager());
    engine.rootContext()->setContextProperty("UpdateManager", &blippy::internal::updateManager());
    engine.rootContext()->setContextProperty("InputHook", &blippy::internal::inputHook());

    // Load QML
    const QUrl url(QStringLiteral("qrc:/qml/ui/qml/main.qml"));
    qDebug() << "Loading QML from:" << url;

    // Connect to warnings to catch QML errors
    QObject::connect(&engine, &QQmlApplicationEngine::warnings,
                     [](const QList<QQmlError> &warnings) {
        for (const QQmlError &warning : warnings) {
            qCritical() << "QML Warning:" << warning.toString();
        }
    });
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            qCritical() << "Failed to load QML:" << url;
            QCoreApplication::exit(-1);
        }
    }, Qt::DirectConnection);

    engine.load(url);
    
    if (engine.rootObjects().isEmpty()) {
        qCritical() << "Failed to load QML - root objects empty";
        return -1;
    }

    qDebug() << "Blippy UI loaded successfully";

    return app.exec();
}