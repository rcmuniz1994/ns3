/****************************************************************************
** Meta object code from reading C++ file 'packetsmode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "packetsmode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'packetsmode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_netanim__PacketsMode_t {
    QByteArrayData data[18];
    char stringdata[275];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_netanim__PacketsMode_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_netanim__PacketsMode_t qt_meta_stringdata_netanim__PacketsMode = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 8),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 10),
QT_MOC_LITERAL(4, 42, 11),
QT_MOC_LITERAL(5, 54, 19),
QT_MOC_LITERAL(6, 74, 12),
QT_MOC_LITERAL(7, 87, 17),
QT_MOC_LITERAL(8, 105, 10),
QT_MOC_LITERAL(9, 116, 23),
QT_MOC_LITERAL(10, 140, 12),
QT_MOC_LITERAL(11, 153, 15),
QT_MOC_LITERAL(12, 169, 3),
QT_MOC_LITERAL(13, 173, 17),
QT_MOC_LITERAL(14, 191, 19),
QT_MOC_LITERAL(15, 211, 17),
QT_MOC_LITERAL(16, 229, 23),
QT_MOC_LITERAL(17, 253, 20)
    },
    "netanim::PacketsMode\0testSlot\0\0"
    "zoomInSlot\0zoomOutSlot\0fromTimeChangedSlot\0"
    "fromTimeText\0toTimeChangedSlot\0"
    "toTimeText\0allowedNodesChangedSlot\0"
    "allowedNodes\0regexFilterSlot\0reg\0"
    "showGridLinesSlot\0showPacketTableSlot\0"
    "filterClickedSlot\0submitFilterClickedSlot\0"
    "showGraphClickedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_netanim__PacketsMode[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x08,
       3,    0,   75,    2, 0x08,
       4,    0,   76,    2, 0x08,
       5,    1,   77,    2, 0x08,
       7,    1,   80,    2, 0x08,
       9,    1,   83,    2, 0x08,
      11,    1,   86,    2, 0x08,
      13,    0,   89,    2, 0x08,
      14,    0,   90,    2, 0x08,
      15,    0,   91,    2, 0x08,
      16,    0,   92,    2, 0x08,
      17,    0,   93,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void netanim::PacketsMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PacketsMode *_t = static_cast<PacketsMode *>(_o);
        switch (_id) {
        case 0: _t->testSlot(); break;
        case 1: _t->zoomInSlot(); break;
        case 2: _t->zoomOutSlot(); break;
        case 3: _t->fromTimeChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->toTimeChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->allowedNodesChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->regexFilterSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->showGridLinesSlot(); break;
        case 8: _t->showPacketTableSlot(); break;
        case 9: _t->filterClickedSlot(); break;
        case 10: _t->submitFilterClickedSlot(); break;
        case 11: _t->showGraphClickedSlot(); break;
        default: ;
        }
    }
}

const QMetaObject netanim::PacketsMode::staticMetaObject = {
    { &Mode::staticMetaObject, qt_meta_stringdata_netanim__PacketsMode.data,
      qt_meta_data_netanim__PacketsMode,  qt_static_metacall, 0, 0}
};


const QMetaObject *netanim::PacketsMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *netanim::PacketsMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_netanim__PacketsMode.stringdata))
        return static_cast<void*>(const_cast< PacketsMode*>(this));
    return Mode::qt_metacast(_clname);
}

int netanim::PacketsMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Mode::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE