/****************************************************************************
** Meta object code from reading C++ file 'animpacket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "animpacket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'animpacket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_netanim__AnimWirelessCircles_t {
    QByteArrayData data[2];
    char stringdata[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_netanim__AnimWirelessCircles_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_netanim__AnimWirelessCircles_t qt_meta_stringdata_netanim__AnimWirelessCircles = {
    {
QT_MOC_LITERAL(0, 0, 28),
QT_MOC_LITERAL(1, 29, 4)
    },
    "netanim::AnimWirelessCircles\0rect\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_netanim__AnimWirelessCircles[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QRectF, 0x00095103,

       0        // eod
};

void netanim::AnimWirelessCircles::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject netanim::AnimWirelessCircles::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_netanim__AnimWirelessCircles.data,
      qt_meta_data_netanim__AnimWirelessCircles,  qt_static_metacall, 0, 0}
};


const QMetaObject *netanim::AnimWirelessCircles::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *netanim::AnimWirelessCircles::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_netanim__AnimWirelessCircles.stringdata))
        return static_cast<void*>(const_cast< AnimWirelessCircles*>(this));
    if (!strcmp(_clname, "QGraphicsEllipseItem"))
        return static_cast< QGraphicsEllipseItem*>(const_cast< AnimWirelessCircles*>(this));
    return QObject::qt_metacast(_clname);
}

int netanim::AnimWirelessCircles::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QRectF*>(_v) = rect(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setRect(*reinterpret_cast< QRectF*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
struct qt_meta_stringdata_netanim__AnimPacket_t {
    QByteArrayData data[2];
    char stringdata[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_netanim__AnimPacket_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_netanim__AnimPacket_t qt_meta_stringdata_netanim__AnimPacket = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 3)
    },
    "netanim::AnimPacket\0pos\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_netanim__AnimPacket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       1,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QPointF, 0x00095103,

       0        // eod
};

void netanim::AnimPacket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject netanim::AnimPacket::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_netanim__AnimPacket.data,
      qt_meta_data_netanim__AnimPacket,  qt_static_metacall, 0, 0}
};


const QMetaObject *netanim::AnimPacket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *netanim::AnimPacket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_netanim__AnimPacket.stringdata))
        return static_cast<void*>(const_cast< AnimPacket*>(this));
    return QGraphicsObject::qt_metacast(_clname);
}

int netanim::AnimPacket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QPointF*>(_v) = pos(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPos(*reinterpret_cast< QPointF*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
