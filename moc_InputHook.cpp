/****************************************************************************
** Meta object code from reading C++ file 'InputHook.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/core/input/InputHook.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InputHook.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN6blippy9InputHookE_t {};
} // unnamed namespace

template <> constexpr inline auto blippy::InputHook::qt_create_metaobjectdata<qt_meta_tag_ZN6blippy9InputHookE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "blippy::InputHook",
        "keyPressed",
        "",
        "virtualKey",
        "text",
        "Qt::KeyboardModifiers",
        "modifiers",
        "keyReleased",
        "mouseClicked",
        "Qt::MouseButton",
        "button",
        "QPoint",
        "pos",
        "shortcutDetected",
        "keyword"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'keyPressed'
        QtMocHelpers::SignalData<void(int, const QString &, Qt::KeyboardModifiers)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 }, { 0x80000000 | 5, 6 },
        }}),
        // Signal 'keyReleased'
        QtMocHelpers::SignalData<void(int, const QString &, Qt::KeyboardModifiers)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::QString, 4 }, { 0x80000000 | 5, 6 },
        }}),
        // Signal 'mouseClicked'
        QtMocHelpers::SignalData<void(Qt::MouseButton, const QPoint &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 }, { 0x80000000 | 11, 12 },
        }}),
        // Signal 'shortcutDetected'
        QtMocHelpers::SignalData<void(const QString &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 14 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<InputHook, qt_meta_tag_ZN6blippy9InputHookE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject blippy::InputHook::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6blippy9InputHookE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6blippy9InputHookE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6blippy9InputHookE_t>.metaTypes,
    nullptr
} };

void blippy::InputHook::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<InputHook *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->keyPressed((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<Qt::KeyboardModifiers>>(_a[3]))); break;
        case 1: _t->keyReleased((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<Qt::KeyboardModifiers>>(_a[3]))); break;
        case 2: _t->mouseClicked((*reinterpret_cast<std::add_pointer_t<Qt::MouseButton>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[2]))); break;
        case 3: _t->shortcutDetected((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (InputHook::*)(int , const QString & , Qt::KeyboardModifiers )>(_a, &InputHook::keyPressed, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (InputHook::*)(int , const QString & , Qt::KeyboardModifiers )>(_a, &InputHook::keyReleased, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (InputHook::*)(Qt::MouseButton , const QPoint & )>(_a, &InputHook::mouseClicked, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (InputHook::*)(const QString & )>(_a, &InputHook::shortcutDetected, 3))
            return;
    }
}

const QMetaObject *blippy::InputHook::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *blippy::InputHook::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6blippy9InputHookE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QAbstractNativeEventFilter"))
        return static_cast< QAbstractNativeEventFilter*>(this);
    return QObject::qt_metacast(_clname);
}

int blippy::InputHook::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void blippy::InputHook::keyPressed(int _t1, const QString & _t2, Qt::KeyboardModifiers _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}

// SIGNAL 1
void blippy::InputHook::keyReleased(int _t1, const QString & _t2, Qt::KeyboardModifiers _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3);
}

// SIGNAL 2
void blippy::InputHook::mouseClicked(Qt::MouseButton _t1, const QPoint & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void blippy::InputHook::shortcutDetected(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
