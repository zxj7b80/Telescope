/****************************************************************************
** Meta object code from reading C++ file 'terminal.h'
**
** Created: Sat Oct 15 12:05:59 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/terminal.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'terminal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Terminal[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      45,    9,    9,    9, 0x08,
      81,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Terminal[] = {
    "Terminal\0\0updateInterface(std::vector<char>)\0"
    "updateInterface2(std::vector<char>)\0"
    "on_pushButton_clicked()\0"
};

const QMetaObject Terminal::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Terminal,
      qt_meta_data_Terminal, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Terminal::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Terminal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Terminal::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Terminal))
        return static_cast<void*>(const_cast< Terminal*>(this));
    return QDialog::qt_metacast(_clname);
}

int Terminal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateInterface((*reinterpret_cast< std::vector<char>(*)>(_a[1]))); break;
        case 1: updateInterface2((*reinterpret_cast< std::vector<char>(*)>(_a[1]))); break;
        case 2: on_pushButton_clicked(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
