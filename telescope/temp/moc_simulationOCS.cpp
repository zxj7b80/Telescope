/****************************************************************************
** Meta object code from reading C++ file 'simulationOCS.h'
**
** Created: Sat Oct 15 12:06:03 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/simulationOCS.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simulationOCS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimulationOCS[] = {

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
      15,   14,   14,   14, 0x08,
      31,   14,   14,   14, 0x08,
      50,   14,   14,   14, 0x08,
      82,   14,   14,   14, 0x08,
     120,   14,   14,   14, 0x08,
     152,   14,   14,   14, 0x08,
     185,   14,   14,   14, 0x08,
     217,   14,   14,   14, 0x08,
     250,   14,   14,   14, 0x08,
     284,   14,   14,   14, 0x08,
     317,   14,   14,   14, 0x08,
     351,   14,   14,   14, 0x08,
     386,   14,   14,   14, 0x08,
     420,   14,   14,   14, 0x08,
     456,   14,   14,   14, 0x08,
     491,   14,   14,   14, 0x08,
     527,   14,   14,   14, 0x08,
     564,   14,   14,   14, 0x08,
     587,  575,   14,   14, 0x08,
     631,  627,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SimulationOCS[] = {
    "SimulationOCS\0\0showConnected()\0"
    "showDisconnected()\0on_pushButton_connect_clicked()\0"
    "on_pushButton_ocs_selfcheck_clicked()\0"
    "on_pushButton_home_az_clicked()\0"
    "on_pushButton_point_az_clicked()\0"
    "on_pushButton_stop_az_clicked()\0"
    "on_pushButton_home_alt_clicked()\0"
    "on_pushButton_point_alt_clicked()\0"
    "on_pushButton_stop_alt_clicked()\0"
    "on_pushButton_home_dero_clicked()\0"
    "on_pushButton_start_dero_clicked()\0"
    "on_pushButton_stop_dero_clicked()\0"
    "on_pushButton_start_track_clicked()\0"
    "on_pushButton_stop_track_clicked()\0"
    "on_pushButton_guidingStar_clicked()\0"
    "on_pushButton_check_status_clicked()\0"
    "readData()\0socketError\0"
    "showError(QAbstractSocket::SocketError)\0"
    "str\0refreshOCStable(QString)\0"
};

const QMetaObject SimulationOCS::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SimulationOCS,
      qt_meta_data_SimulationOCS, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SimulationOCS::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SimulationOCS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SimulationOCS::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimulationOCS))
        return static_cast<void*>(const_cast< SimulationOCS*>(this));
    return QWidget::qt_metacast(_clname);
}

int SimulationOCS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showConnected(); break;
        case 1: showDisconnected(); break;
        case 2: on_pushButton_connect_clicked(); break;
        case 3: on_pushButton_ocs_selfcheck_clicked(); break;
        case 4: on_pushButton_home_az_clicked(); break;
        case 5: on_pushButton_point_az_clicked(); break;
        case 6: on_pushButton_stop_az_clicked(); break;
        case 7: on_pushButton_home_alt_clicked(); break;
        case 8: on_pushButton_point_alt_clicked(); break;
        case 9: on_pushButton_stop_alt_clicked(); break;
        case 10: on_pushButton_home_dero_clicked(); break;
        case 11: on_pushButton_start_dero_clicked(); break;
        case 12: on_pushButton_stop_dero_clicked(); break;
        case 13: on_pushButton_start_track_clicked(); break;
        case 14: on_pushButton_stop_track_clicked(); break;
        case 15: on_pushButton_guidingStar_clicked(); break;
        case 16: on_pushButton_check_status_clicked(); break;
        case 17: readData(); break;
        case 18: showError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 19: refreshOCStable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 20;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
