/****************************************************************************
** Meta object code from reading C++ file 'clientserver.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "clientserver.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clientserver.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_clientserver_t {
    QByteArrayData data[9];
    char stringdata[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_clientserver_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_clientserver_t qt_meta_stringdata_clientserver = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 19),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 7),
QT_MOC_LITERAL(4, 42, 17),
QT_MOC_LITERAL(5, 60, 5),
QT_MOC_LITERAL(6, 66, 5),
QT_MOC_LITERAL(7, 72, 13),
QT_MOC_LITERAL(8, 86, 2)
    },
    "clientserver\0sig_messageRecieved\0\0"
    "msgType\0std::vector<char>\0cdata\0vdata\0"
    "sig_exception\0ex"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_clientserver[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   34,    2, 0x06 /* Public */,
       1,    2,   41,    2, 0x26 /* Public | MethodCloned */,
       1,    1,   46,    2, 0x26 /* Public | MethodCloned */,
       7,    1,   49,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4, 0x80000000 | 4,    3,    5,    6,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    8,

       0        // eod
};

void clientserver::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        clientserver *_t = static_cast<clientserver *>(_o);
        switch (_id) {
        case 0: _t->sig_messageRecieved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const std::vector<char>(*)>(_a[2])),(*reinterpret_cast< const std::vector<char>(*)>(_a[3]))); break;
        case 1: _t->sig_messageRecieved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const std::vector<char>(*)>(_a[2]))); break;
        case 2: _t->sig_messageRecieved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sig_exception((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (clientserver::*_t)(int , const std::vector<char> & , const std::vector<char> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&clientserver::sig_messageRecieved)) {
                *result = 0;
            }
        }
        {
            typedef void (clientserver::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&clientserver::sig_exception)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject clientserver::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_clientserver.data,
      qt_meta_data_clientserver,  qt_static_metacall, 0, 0}
};


const QMetaObject *clientserver::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *clientserver::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_clientserver.stringdata))
        return static_cast<void*>(const_cast< clientserver*>(this));
    return QThread::qt_metacast(_clname);
}

int clientserver::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void clientserver::sig_messageRecieved(int _t1, const std::vector<char> & _t2, const std::vector<char> & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 3
void clientserver::sig_exception(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
