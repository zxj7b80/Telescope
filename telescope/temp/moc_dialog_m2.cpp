/****************************************************************************
** Meta object code from reading C++ file 'dialog_m2.h'
**
** Created: Sat Oct 15 12:06:10 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/dialog_m2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog_m2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dialog_m2[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      42,   10,   10,   10, 0x08,
      73,   10,   10,   10, 0x08,
     101,   10,   10,   10, 0x08,
     132,   10,   10,   10, 0x08,
     164,   10,   10,   10, 0x08,
     193,   10,   10,   10, 0x08,
     222,   10,   10,   10, 0x08,
     251,   10,   10,   10, 0x08,
     280,   10,   10,   10, 0x08,
     309,   10,   10,   10, 0x08,
     340,   10,   10,   10, 0x08,
     373,   10,   10,   10, 0x08,
     404,   10,   10,   10, 0x08,
     435,   10,   10,   10, 0x08,
     444,   10,   10,   10, 0x08,
     455,   10,   10,   10, 0x08,
     466,   10,   10,   10, 0x08,
     477,   10,   10,   10, 0x08,
     488,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Dialog_m2[] = {
    "Dialog_m2\0\0on_pushButton_getIDN_clicked()\0"
    "on_pushButton_getspi_clicked()\0"
    "on_pushButton_ini_clicked()\0"
    "on_pushButton_enable_clicked()\0"
    "on_pushButton_disable_clicked()\0"
    "on_pushButton_x_go_clicked()\0"
    "on_pushButton_y_go_clicked()\0"
    "on_pushButton_z_go_clicked()\0"
    "on_pushButton_u_go_clicked()\0"
    "on_pushButton_v_go_clicked()\0"
    "on_pushButton_all_go_clicked()\0"
    "on_pushButton_all_stop_clicked()\0"
    "on_pushButton_getpos_clicked()\0"
    "on_pushButton_setspi_clicked()\0setvel()\0"
    "getpos_x()\0getpos_y()\0getpos_z()\0"
    "getpos_u()\0getpos_v()\0"
};

const QMetaObject Dialog_m2::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog_m2,
      qt_meta_data_Dialog_m2, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dialog_m2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dialog_m2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dialog_m2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog_m2))
        return static_cast<void*>(const_cast< Dialog_m2*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dialog_m2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_getIDN_clicked(); break;
        case 1: on_pushButton_getspi_clicked(); break;
        case 2: on_pushButton_ini_clicked(); break;
        case 3: on_pushButton_enable_clicked(); break;
        case 4: on_pushButton_disable_clicked(); break;
        case 5: on_pushButton_x_go_clicked(); break;
        case 6: on_pushButton_y_go_clicked(); break;
        case 7: on_pushButton_z_go_clicked(); break;
        case 8: on_pushButton_u_go_clicked(); break;
        case 9: on_pushButton_v_go_clicked(); break;
        case 10: on_pushButton_all_go_clicked(); break;
        case 11: on_pushButton_all_stop_clicked(); break;
        case 12: on_pushButton_getpos_clicked(); break;
        case 13: on_pushButton_setspi_clicked(); break;
        case 14: setvel(); break;
        case 15: getpos_x(); break;
        case 16: getpos_y(); break;
        case 17: getpos_z(); break;
        case 18: getpos_u(); break;
        case 19: getpos_v(); break;
        default: ;
        }
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
