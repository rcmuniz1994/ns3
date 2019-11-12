/****************************************************************************
** Meta object code from reading C++ file 'fileeditfactory.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qtpropertybrowser/src/fileeditfactory.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileeditfactory.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FileEditFactory_t {
    QByteArrayData data[11];
    char stringdata[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_FileEditFactory_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_FileEditFactory_t qt_meta_stringdata_FileEditFactory = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 19),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 11),
QT_MOC_LITERAL(4, 49, 8),
QT_MOC_LITERAL(5, 58, 5),
QT_MOC_LITERAL(6, 64, 17),
QT_MOC_LITERAL(7, 82, 6),
QT_MOC_LITERAL(8, 89, 12),
QT_MOC_LITERAL(9, 102, 19),
QT_MOC_LITERAL(10, 122, 6)
    },
    "FileEditFactory\0slotPropertyChanged\0"
    "\0QtProperty*\0property\0value\0"
    "slotFilterChanged\0filter\0slotSetValue\0"
    "slotEditorDestroyed\0object\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FileEditFactory[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x08,
       6,    2,   39,    2, 0x08,
       8,    1,   44,    2, 0x08,
       9,    1,   47,    2, 0x08,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    5,
    QMetaType::Void, 0x80000000 | 3, QMetaType::QString,    4,    7,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QObjectStar,   10,

       0        // eod
};

void FileEditFactory::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FileEditFactory *_t = static_cast<FileEditFactory *>(_o);
        switch (_id) {
        case 0: _t->slotPropertyChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->slotFilterChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->slotSetValue((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->slotEditorDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject FileEditFactory::staticMetaObject = {
    { &QtAbstractEditorFactory<FilePathManager>::staticMetaObject, qt_meta_stringdata_FileEditFactory.data,
      qt_meta_data_FileEditFactory,  qt_static_metacall, 0, 0}
};


const QMetaObject *FileEditFactory::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileEditFactory::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileEditFactory.stringdata))
        return static_cast<void*>(const_cast< FileEditFactory*>(this));
    return QtAbstractEditorFactory<FilePathManager>::qt_metacast(_clname);
}

int FileEditFactory::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtAbstractEditorFactory<FilePathManager>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
