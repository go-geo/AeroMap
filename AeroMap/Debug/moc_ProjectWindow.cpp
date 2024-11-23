/****************************************************************************
** Meta object code from reading C++ file 'ProjectWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../ProjectWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ProjectWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ProjectWindow_t {
    QByteArrayData data[13];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProjectWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProjectWindow_t qt_meta_stringdata_ProjectWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ProjectWindow"
QT_MOC_LITERAL(1, 14, 16), // "OnProjectChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 13), // "OnItemClicked"
QT_MOC_LITERAL(4, 46, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(5, 63, 5), // "pItem"
QT_MOC_LITERAL(6, 69, 6), // "column"
QT_MOC_LITERAL(7, 76, 19), // "OnItemDoubleClicked"
QT_MOC_LITERAL(8, 96, 14), // "OnActivateView"
QT_MOC_LITERAL(9, 111, 9), // "OnAddFile"
QT_MOC_LITERAL(10, 121, 10), // "OnLoadFile"
QT_MOC_LITERAL(11, 132, 12), // "OnProperties"
QT_MOC_LITERAL(12, 145, 12) // "OnOpenFolder"

    },
    "ProjectWindow\0OnProjectChanged\0\0"
    "OnItemClicked\0QTreeWidgetItem*\0pItem\0"
    "column\0OnItemDoubleClicked\0OnActivateView\0"
    "OnAddFile\0OnLoadFile\0OnProperties\0"
    "OnOpenFolder"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProjectWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    2,   55,    2, 0x08 /* Private */,
       7,    2,   60,    2, 0x08 /* Private */,
       8,    0,   65,    2, 0x08 /* Private */,
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,
      11,    0,   68,    2, 0x08 /* Private */,
      12,    0,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ProjectWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ProjectWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnProjectChanged(); break;
        case 1: _t->OnItemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->OnItemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->OnActivateView(); break;
        case 4: _t->OnAddFile(); break;
        case 5: _t->OnLoadFile(); break;
        case 6: _t->OnProperties(); break;
        case 7: _t->OnOpenFolder(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ProjectWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ProjectWindow.data,
    qt_meta_data_ProjectWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ProjectWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProjectWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ProjectWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProjectWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
