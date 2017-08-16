/****************************************************************************
** Meta object code from reading C++ file 'adc.h'
**
** Created: Sat Oct 15 12:06:13 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/adc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'adc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ADC[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      21,    4,   16,    4, 0x08,
      35,    4,    4,    4, 0x08,
      52,    4,    4,    4, 0x08,
      63,    4,   16,    4, 0x08,
      79,    4,   16,    4, 0x08,
      95,    4,    4,    4, 0x08,
     131,    4,    4,    4, 0x08,
     167,    4,    4,    4, 0x08,
     204,    4,    4,    4, 0x08,
     241,    4,    4,    4, 0x08,
     275,    4,    4,    4, 0x08,
     309,    4,    4,    4, 0x08,
     341,    4,    4,    4, 0x08,
     383,    4,    4,    4, 0x08,
     433,  425,    4,    4, 0x08,
     476,  425,    4,    4, 0x08,
     519,  425,    4,    4, 0x08,
     561,  425,    4,    4, 0x08,
     603,  425,    4,    4, 0x08,
     646,  425,    4,    4, 0x08,
     689,    4,    4,    4, 0x08,
     721,    4,    4,    4, 0x08,
     755,    4,    4,    4, 0x08,
     789,    4,    4,    4, 0x08,
     820,    4,    4,    4, 0x08,
     853,    4,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ADC[] = {
    "ADC\0\0replyOCS()\0bool\0showADCInfo()\0"
    "adcCheckStatus()\0saveData()\0storeADC1Data()\0"
    "storeADC2Data()\0on_pushButton_adc1_enable_clicked()\0"
    "on_pushButton_adc2_enable_clicked()\0"
    "on_pushButton_adc1_disable_clicked()\0"
    "on_pushButton_adc2_disable_clicked()\0"
    "on_pushButton_adc1_home_clicked()\0"
    "on_pushButton_adc2_home_clicked()\0"
    "on_pushButton_adc1_go_clicked()\0"
    "on_lineEdit_adc1_vel_spec_returnPressed()\0"
    "on_lineEdit_adc2_vel_spec_returnPressed()\0"
    "checked\0on_radioButton_adc1_vel_fast_toggled(bool)\0"
    "on_radioButton_adc2_vel_fast_toggled(bool)\0"
    "on_radioButton_adc1_vel_mid_toggled(bool)\0"
    "on_radioButton_adc2_vel_mid_toggled(bool)\0"
    "on_radioButton_adc1_vel_slow_toggled(bool)\0"
    "on_radioButton_adc2_vel_slow_toggled(bool)\0"
    "on_pushButton_adc2_go_clicked()\0"
    "on_pushButton_adc1_stop_clicked()\0"
    "on_pushButton_adc2_stop_clicked()\0"
    "on_pushButton_adc_go_clicked()\0"
    "on_pushButton_adc_stop_clicked()\0"
    "on_pushButton_adc_park_clicked()\0"
};

const QMetaObject ADC::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ADC,
      qt_meta_data_ADC, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ADC::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ADC::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ADC::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ADC))
        return static_cast<void*>(const_cast< ADC*>(this));
    return QDialog::qt_metacast(_clname);
}

int ADC::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: replyOCS(); break;
        case 1: { bool _r = showADCInfo();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: adcCheckStatus(); break;
        case 3: saveData(); break;
        case 4: { bool _r = storeADC1Data();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = storeADC2Data();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: on_pushButton_adc1_enable_clicked(); break;
        case 7: on_pushButton_adc2_enable_clicked(); break;
        case 8: on_pushButton_adc1_disable_clicked(); break;
        case 9: on_pushButton_adc2_disable_clicked(); break;
        case 10: on_pushButton_adc1_home_clicked(); break;
        case 11: on_pushButton_adc2_home_clicked(); break;
        case 12: on_pushButton_adc1_go_clicked(); break;
        case 13: on_lineEdit_adc1_vel_spec_returnPressed(); break;
        case 14: on_lineEdit_adc2_vel_spec_returnPressed(); break;
        case 15: on_radioButton_adc1_vel_fast_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: on_radioButton_adc2_vel_fast_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: on_radioButton_adc1_vel_mid_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 18: on_radioButton_adc2_vel_mid_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 19: on_radioButton_adc1_vel_slow_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: on_radioButton_adc2_vel_slow_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: on_pushButton_adc2_go_clicked(); break;
        case 22: on_pushButton_adc1_stop_clicked(); break;
        case 23: on_pushButton_adc2_stop_clicked(); break;
        case 24: on_pushButton_adc_go_clicked(); break;
        case 25: on_pushButton_adc_stop_clicked(); break;
        case 26: on_pushButton_adc_park_clicked(); break;
        default: ;
        }
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
