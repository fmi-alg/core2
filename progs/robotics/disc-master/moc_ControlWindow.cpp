/****************************************************************************
** Meta object code from reading C++ file 'ControlWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ControlWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ControlWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ControlWindow_t {
    QByteArrayData data[19];
    char stringdata0[387];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ControlWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ControlWindow_t qt_meta_stringdata_ControlWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ControlWindow"
QT_MOC_LITERAL(1, 14, 25), // "on_pushButton_run_clicked"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 26), // "on_pushButton_exit_clicked"
QT_MOC_LITERAL(4, 68, 25), // "on_checkBox_bound_clicked"
QT_MOC_LITERAL(5, 94, 28), // "on_radioButton_1step_clicked"
QT_MOC_LITERAL(6, 123, 7), // "checked"
QT_MOC_LITERAL(7, 131, 32), // "on_radioButton_1000steps_clicked"
QT_MOC_LITERAL(8, 164, 31), // "on_radioButton_infsteps_clicked"
QT_MOC_LITERAL(9, 196, 31), // "on_radioButton_varsteps_clicked"
QT_MOC_LITERAL(10, 228, 23), // "on_spinBox_valueChanged"
QT_MOC_LITERAL(11, 252, 4), // "arg1"
QT_MOC_LITERAL(12, 257, 16), // "on_right_clicked"
QT_MOC_LITERAL(13, 274, 15), // "on_left_clicked"
QT_MOC_LITERAL(14, 290, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(15, 312, 15), // "on_anim_clicked"
QT_MOC_LITERAL(16, 328, 32), // "on_horizontalSlider_valueChanged"
QT_MOC_LITERAL(17, 361, 5), // "value"
QT_MOC_LITERAL(18, 367, 19) // "on_checkBox_clicked"

    },
    "ControlWindow\0on_pushButton_run_clicked\0"
    "\0on_pushButton_exit_clicked\0"
    "on_checkBox_bound_clicked\0"
    "on_radioButton_1step_clicked\0checked\0"
    "on_radioButton_1000steps_clicked\0"
    "on_radioButton_infsteps_clicked\0"
    "on_radioButton_varsteps_clicked\0"
    "on_spinBox_valueChanged\0arg1\0"
    "on_right_clicked\0on_left_clicked\0"
    "on_pushButton_clicked\0on_anim_clicked\0"
    "on_horizontalSlider_valueChanged\0value\0"
    "on_checkBox_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ControlWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    0,   90,    2, 0x08 /* Private */,
       4,    1,   91,    2, 0x08 /* Private */,
       5,    1,   94,    2, 0x08 /* Private */,
       7,    1,   97,    2, 0x08 /* Private */,
       8,    1,  100,    2, 0x08 /* Private */,
       9,    1,  103,    2, 0x08 /* Private */,
      10,    1,  106,    2, 0x08 /* Private */,
       9,    0,  109,    2, 0x08 /* Private */,
      12,    0,  110,    2, 0x08 /* Private */,
      13,    0,  111,    2, 0x08 /* Private */,
      14,    0,  112,    2, 0x08 /* Private */,
      15,    0,  113,    2, 0x08 /* Private */,
      16,    1,  114,    2, 0x08 /* Private */,
      18,    0,  117,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,

       0        // eod
};

void ControlWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ControlWindow *_t = static_cast<ControlWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButton_run_clicked(); break;
        case 1: _t->on_pushButton_exit_clicked(); break;
        case 2: _t->on_checkBox_bound_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_radioButton_1step_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_radioButton_1000steps_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_radioButton_infsteps_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_radioButton_varsteps_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_spinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_radioButton_varsteps_clicked(); break;
        case 9: _t->on_right_clicked(); break;
        case 10: _t->on_left_clicked(); break;
        case 11: _t->on_pushButton_clicked(); break;
        case 12: _t->on_anim_clicked(); break;
        case 13: _t->on_horizontalSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_checkBox_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject ControlWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ControlWindow.data,
      qt_meta_data_ControlWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ControlWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ControlWindow.stringdata0))
        return static_cast<void*>(const_cast< ControlWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ControlWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
