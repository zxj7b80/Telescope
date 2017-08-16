/****************************************************************************
** Meta object code from reading C++ file 'direct_azalt.h'
**
** Created: Sat Oct 15 12:05:58 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/direct_azalt.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'direct_azalt.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CSendMessageTCS[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_CSendMessageTCS[] = {
    "CSendMessageTCS\0"
};

const QMetaObject CSendMessageTCS::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CSendMessageTCS,
      qt_meta_data_CSendMessageTCS, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CSendMessageTCS::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CSendMessageTCS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CSendMessageTCS::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CSendMessageTCS))
        return static_cast<void*>(const_cast< CSendMessageTCS*>(this));
    return QWidget::qt_metacast(_clname);
}

int CSendMessageTCS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_DirectAzalt[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      56,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      46,   12,   12,   12, 0x08,
      79,   12,   12,   12, 0x08,
     113,   12,   12,   12, 0x08,
     150,  147,   12,   12, 0x08,
     191,  147,   12,   12, 0x08,
     231,  147,   12,   12, 0x08,
     273,  147,   12,   12, 0x08,
     315,  147,   12,   12, 0x08,
     356,  147,   12,   12, 0x08,
     399,   12,   12,   12, 0x08,
     412,   12,   12,   12, 0x08,
     426,   12,   12,   12, 0x08,
     448,   12,   12,   12, 0x08,
     459,   12,   12,   12, 0x08,
     475,   12,   12,   12, 0x08,
     492,   12,   12,   12, 0x08,
     505,   12,   12,   12, 0x08,
     518,   12,   12,   12, 0x08,
     536,   12,   12,   12, 0x08,
     552,   12,   12,   12, 0x08,
     574,   12,  569,   12, 0x08,
     588,   12,  569,   12, 0x08,
     603,   12,   12,   12, 0x08,
     620,   12,   12,   12, 0x08,
     639,   12,   12,   12, 0x08,
     673,   12,   12,   12, 0x08,
     708,   12,   12,   12, 0x08,
     740,  147,   12,   12, 0x08,
     781,  147,   12,   12, 0x08,
     821,  147,   12,   12, 0x08,
     862,   12,   12,   12, 0x08,
     902,   12,   12,   12, 0x08,
     932,   12,   12,   12, 0x08,
     964,   12,   12,   12, 0x08,
     996,   12,   12,   12, 0x08,
    1029,   12,   12,   12, 0x08,
    1064,   12,   12,   12, 0x08,
    1100,   12,   12,   12, 0x08,
    1133,  147,   12,   12, 0x08,
    1175,  147,   12,   12, 0x08,
    1216,  147,   12,   12, 0x08,
    1258,   12,   12,   12, 0x08,
    1299,   12,   12,   12, 0x08,
    1330,   12,   12,   12, 0x08,
    1363,   12,   12,   12, 0x08,
    1395,   12,   12,   12, 0x08,
    1428,   12,   12,   12, 0x08,
    1460,   12,   12,   12, 0x08,
    1493,   12,   12,   12, 0x08,
    1526,   12,   12,   12, 0x08,
    1560,   12,   12,   12, 0x08,
    1593,   12,   12,   12, 0x08,
    1627,   12,   12,   12, 0x08,
    1637,   12,   12,   12, 0x08,
    1671,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DirectAzalt[] = {
    "DirectAzalt\0\0on_pushButton_az_stepP_clicked()\0"
    "on_pushButton_az_stepM_clicked()\0"
    "on_pushButton_alt_stepP_clicked()\0"
    "on_pushButton_alt_stepM_clicked()\0on\0"
    "on_radioButton_az_stepLong_toggled(bool)\0"
    "on_radioButton_az_stepMid_toggled(bool)\0"
    "on_radioButton_az_stepShort_toggled(bool)\0"
    "on_radioButton_alt_stepLong_toggled(bool)\0"
    "on_radioButton_alt_stepMid_toggled(bool)\0"
    "on_radioButton_alt_stepShort_toggled(bool)\0"
    "AZreplyOCS()\0ALTreplyOCS()\0"
    "CheckCommandFromOCS()\0saveData()\0"
    "showMountInfo()\0CheckAmplifier()\0"
    "CheckError()\0CheckLimit()\0CheckLoopStatus()\0"
    "CheckAzHoming()\0CheckAltHoming()\0bool\0"
    "storeAzData()\0storeAltData()\0"
    "show_MountData()\0AzAltCheckStatus()\0"
    "on_pushButton_az_enable_clicked()\0"
    "on_pushButton_az_disable_clicked()\0"
    "on_pushButton_az_home_clicked()\0"
    "on_radioButton_az_vel_fast_toggled(bool)\0"
    "on_radioButton_az_vel_mid_toggled(bool)\0"
    "on_radioButton_az_vel_slow_toggled(bool)\0"
    "on_lineEdit_az_vel_spec_returnPressed()\0"
    "on_pushButton_az_go_clicked()\0"
    "on_pushButton_az_stop_clicked()\0"
    "on_pushButton_az_park_clicked()\0"
    "on_pushButton_alt_park_clicked()\0"
    "on_pushButton_alt_enable_clicked()\0"
    "on_pushButton_alt_disable_clicked()\0"
    "on_pushButton_alt_home_clicked()\0"
    "on_radioButton_alt_vel_fast_toggled(bool)\0"
    "on_radioButton_alt_vel_mid_toggled(bool)\0"
    "on_radioButton_alt_vel_slow_toggled(bool)\0"
    "on_lineEdit_alt_vel_spec_returnPressed()\0"
    "on_pushButton_alt_go_clicked()\0"
    "on_pushButton_alt_stop_clicked()\0"
    "on_pushButton_az_dirP_pressed()\0"
    "on_pushButton_az_dirP_released()\0"
    "on_pushButton_az_dirM_pressed()\0"
    "on_pushButton_az_dirM_released()\0"
    "on_pushButton_alt_dirP_pressed()\0"
    "on_pushButton_alt_dirP_released()\0"
    "on_pushButton_alt_dirM_pressed()\0"
    "on_pushButton_alt_dirM_released()\0"
    "gopoint()\0on_pushButton_alt_brake_clicked()\0"
    "CheckAltBrake()\0"
};

const QMetaObject DirectAzalt::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_DirectAzalt,
      qt_meta_data_DirectAzalt, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DirectAzalt::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DirectAzalt::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DirectAzalt::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DirectAzalt))
        return static_cast<void*>(const_cast< DirectAzalt*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int DirectAzalt::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_pushButton_az_stepP_clicked(); break;
        case 1: on_pushButton_az_stepM_clicked(); break;
        case 2: on_pushButton_alt_stepP_clicked(); break;
        case 3: on_pushButton_alt_stepM_clicked(); break;
        case 4: on_radioButton_az_stepLong_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: on_radioButton_az_stepMid_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: on_radioButton_az_stepShort_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: on_radioButton_alt_stepLong_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: on_radioButton_alt_stepMid_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: on_radioButton_alt_stepShort_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: AZreplyOCS(); break;
        case 11: ALTreplyOCS(); break;
        case 12: CheckCommandFromOCS(); break;
        case 13: saveData(); break;
        case 14: showMountInfo(); break;
        case 15: CheckAmplifier(); break;
        case 16: CheckError(); break;
        case 17: CheckLimit(); break;
        case 18: CheckLoopStatus(); break;
        case 19: CheckAzHoming(); break;
        case 20: CheckAltHoming(); break;
        case 21: { bool _r = storeAzData();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 22: { bool _r = storeAltData();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 23: show_MountData(); break;
        case 24: AzAltCheckStatus(); break;
        case 25: on_pushButton_az_enable_clicked(); break;
        case 26: on_pushButton_az_disable_clicked(); break;
        case 27: on_pushButton_az_home_clicked(); break;
        case 28: on_radioButton_az_vel_fast_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: on_radioButton_az_vel_mid_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: on_radioButton_az_vel_slow_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 31: on_lineEdit_az_vel_spec_returnPressed(); break;
        case 32: on_pushButton_az_go_clicked(); break;
        case 33: on_pushButton_az_stop_clicked(); break;
        case 34: on_pushButton_az_park_clicked(); break;
        case 35: on_pushButton_alt_park_clicked(); break;
        case 36: on_pushButton_alt_enable_clicked(); break;
        case 37: on_pushButton_alt_disable_clicked(); break;
        case 38: on_pushButton_alt_home_clicked(); break;
        case 39: on_radioButton_alt_vel_fast_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 40: on_radioButton_alt_vel_mid_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 41: on_radioButton_alt_vel_slow_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 42: on_lineEdit_alt_vel_spec_returnPressed(); break;
        case 43: on_pushButton_alt_go_clicked(); break;
        case 44: on_pushButton_alt_stop_clicked(); break;
        case 45: on_pushButton_az_dirP_pressed(); break;
        case 46: on_pushButton_az_dirP_released(); break;
        case 47: on_pushButton_az_dirM_pressed(); break;
        case 48: on_pushButton_az_dirM_released(); break;
        case 49: on_pushButton_alt_dirP_pressed(); break;
        case 50: on_pushButton_alt_dirP_released(); break;
        case 51: on_pushButton_alt_dirM_pressed(); break;
        case 52: on_pushButton_alt_dirM_released(); break;
        case 53: gopoint(); break;
        case 54: on_pushButton_alt_brake_clicked(); break;
        case 55: CheckAltBrake(); break;
        default: ;
        }
        _id -= 56;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
