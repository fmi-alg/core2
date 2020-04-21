/****************************************************************************
** Meta object code from reading C++ file 'NewWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NewWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NewWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NewWindow_t {
    QByteArrayData data[13];
    char stringdata0[217];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NewWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NewWindow_t qt_meta_stringdata_NewWindow = {
    {
QT_MOC_LITERAL(0, 0, 9), // "NewWindow"
QT_MOC_LITERAL(1, 10, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 23), // "on_pushButton_3_clicked"
QT_MOC_LITERAL(4, 57, 14), // "on_yes_clicked"
QT_MOC_LITERAL(5, 72, 13), // "on_no_clicked"
QT_MOC_LITERAL(6, 86, 15), // "mousePressEvent"
QT_MOC_LITERAL(7, 102, 12), // "QMouseEvent*"
QT_MOC_LITERAL(8, 115, 23), // "on_pushButton_4_clicked"
QT_MOC_LITERAL(9, 139, 13), // "on_ok_clicked"
QT_MOC_LITERAL(10, 153, 19), // "on_interior_clicked"
QT_MOC_LITERAL(11, 173, 19), // "on_exterior_clicked"
QT_MOC_LITERAL(12, 193, 23) // "on_pushButton_2_clicked"

    },
    "NewWindow\0on_pushButton_clicked\0\0"
    "on_pushButton_3_clicked\0on_yes_clicked\0"
    "on_no_clicked\0mousePressEvent\0"
    "QMouseEvent*\0on_pushButton_4_clicked\0"
    "on_ok_clicked\0on_interior_clicked\0"
    "on_exterior_clicked\0on_pushButton_2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NewWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    1,   68,    2, 0x08 /* Private */,
       8,    0,   71,    2, 0x08 /* Private */,
       9,    0,   72,    2, 0x08 /* Private */,
      10,    0,   73,    2, 0x08 /* Private */,
      11,    0,   74,    2, 0x08 /* Private */,
      12,    0,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NewWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NewWindow *_t = static_cast<NewWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_clicked(); break;
        case 1: _t->on_pushButton_3_clicked(); break;
        case 2: _t->on_yes_clicked(); break;
        case 3: _t->on_no_clicked(); break;
        case 4: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: _t->on_pushButton_4_clicked(); break;
        case 6: _t->on_ok_clicked(); break;
        case 7: _t->on_interior_clicked(); break;
        case 8: _t->on_exterior_clicked(); break;
        case 9: _t->on_pushButton_2_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject NewWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_NewWindow.data,
      qt_meta_data_NewWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NewWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NewWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NewWindow.stringdata0))
        return static_cast<void*>(const_cast< NewWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int NewWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
