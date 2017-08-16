/****************************************************************************
** Meta object code from reading C++ file 'setup.h'
**
** Created: Sat Oct 15 12:06:02 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/setup.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'setup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Setup[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x08,
      34,    6,    6,    6, 0x08,
      65,    6,    6,    6, 0x08,
     109,    6,    6,    6, 0x08,
     149,    6,    6,    6, 0x08,
     181,    6,    6,    6, 0x08,
     217,    6,    6,    6, 0x08,
     249,    6,    6,    6, 0x08,
     286,    6,    6,    6, 0x08,
     318,    6,    6,    6, 0x08,
     355,    6,    6,    6, 0x08,
     391,    6,    6,    6, 0x08,
     435,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Setup[] = {
    "Setup\0\0on_pushButton_ok_clicked()\0"
    "on_pushButton_cancel_clicked()\0"
    "on_pushButton_SelectStarTableFile_clicked()\0"
    "on_pushButton_SelectModelFile_clicked()\0"
    "on_pushButton_default_clicked()\0"
    "on_pushButton_Dero_cancel_clicked()\0"
    "on_pushButton_Dero_ok_clicked()\0"
    "on_pushButton_Dero_default_clicked()\0"
    "on_pushButton_Misc_ok_clicked()\0"
    "on_pushButton_Misc_default_clicked()\0"
    "on_pushButton_Misc_cancel_clicked()\0"
    "on_pushButton_SelectDataDirectory_clicked()\0"
    "on_pushButton_SelectLogDirectory_clicked()\0"
};

const QMetaObject Setup::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_Setup,
      qt_meta_data_Setup, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Setup::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Setup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Setup::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Setup))
        return static_cast<void*>(const_cast< Setup*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int Setup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_ok_clicked(); break;
        case 1: on_pushButton_cancel_clicked(); break;
        case 2: on_pushButton_SelectStarTableFile_clicked(); break;
        case 3: on_pushButton_SelectModelFile_clicked(); break;
        case 4: on_pushButton_default_clicked(); break;
        case 5: on_pushButton_Dero_cancel_clicked(); break;
        case 6: on_pushButton_Dero_ok_clicked(); break;
        case 7: on_pushButton_Dero_default_clicked(); break;
        case 8: on_pushButton_Misc_ok_clicked(); break;
        case 9: on_pushButton_Misc_default_clicked(); break;
        case 10: on_pushButton_Misc_cancel_clicked(); break;
        case 11: on_pushButton_SelectDataDirectory_clicked(); break;
        case 12: on_pushButton_SelectLogDirectory_clicked(); break;
        default: ;
        }
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
