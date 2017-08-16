/****************************************************************************
** Meta object code from reading C++ file 'tracking.h'
**
** Created: Sat Oct 15 12:06:00 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/tracking.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tracking.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Tracking[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      26,    9,    9,    9, 0x08,
      38,    9,    9,    9, 0x08,
      52,    9,    9,    9, 0x08,
      80,    9,    9,    9, 0x08,
     109,    9,    9,    9, 0x08,
     139,    9,    9,    9, 0x08,
     168,    9,    9,    9, 0x08,
     198,    9,    9,    9, 0x08,
     227,    9,    9,    9, 0x08,
     260,    9,    9,    9, 0x08,
     294,    9,    9,    9, 0x08,
     329,    9,    9,    9, 0x08,
     363,    9,    9,    9, 0x08,
     391,    9,    9,    9, 0x08,
     425,    9,    9,    9, 0x08,
     466,  446,  441,    9, 0x08,
     537,    9,  441,    9, 0x08,
     552,    9,  441,    9, 0x08,
     568,    9,  441,    9, 0x08,
     585,    9,    9,    9, 0x08,
     609,  601,    9,    9, 0x08,
     638,  601,    9,    9, 0x08,
     668,  601,    9,    9, 0x08,
     699,  601,    9,    9, 0x08,
     730,  601,    9,    9, 0x08,
     761,    9,    9,    9, 0x08,
     798,  601,    9,    9, 0x08,
     836,  601,    9,    9, 0x08,
     873,  601,    9,    9, 0x08,
     905,    9,    9,    9, 0x08,
     935,    9,    9,    9, 0x08,
     971,    9,    9,    9, 0x08,
    1006,  601,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Tracking[] = {
    "Tracking\0\0showTrackInfo()\0show_info()\0"
    "CheckStatus()\0CheckPointingORTrackingAz()\0"
    "CheckPointingORTrackingAlt()\0"
    "CheckPointingORTrackingDero()\0"
    "on_pushButton_save_clicked()\0"
    "on_pushButton_track_clicked()\0"
    "on_pushButton_stop_clicked()\0"
    "on_pushButton_AzOffset_clicked()\0"
    "on_pushButton_AltOffset_clicked()\0"
    "on_pushButton_DeroOffset_clicked()\0"
    "on_pushButton_calculate_clicked()\0"
    "calculate_thread_finished()\0"
    "on_pushButton_enableAll_clicked()\0"
    "TrackreplyOCS()\0bool\0AZ,ALT,FR,HA,DEC,dt\0"
    "saveStarTable(double[],double[],double[],double[],double[],DateTime[])\0"
    "getTrackData()\0saveTrackData()\0"
    "storeTrackData()\0CalAndShowRms()\0"
    "checked\0on_checkBox_Az_toggled(bool)\0"
    "on_checkBox_Alt_toggled(bool)\0"
    "on_checkBox_Dero_toggled(bool)\0"
    "on_checkBox_ADC1_toggled(bool)\0"
    "on_checkBox_ADC2_toggled(bool)\0"
    "on_pushButton_saveformodel_clicked()\0"
    "on_checkBox_CorrectAuto_toggled(bool)\0"
    "on_checkBox_PointModel_toggled(bool)\0"
    "on_checkBox_Pixel_toggled(bool)\0"
    "on_pushButton_paste_clicked()\0"
    "on_pushButton_ADC2_Offset_clicked()\0"
    "on_pushButton_ADC1Offset_clicked()\0"
    "on_checkBox_ALTHALFDERO_toggled(bool)\0"
};

const QMetaObject Tracking::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_Tracking,
      qt_meta_data_Tracking, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Tracking::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Tracking::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Tracking::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Tracking))
        return static_cast<void*>(const_cast< Tracking*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int Tracking::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showTrackInfo(); break;
        case 1: show_info(); break;
        case 2: CheckStatus(); break;
        case 3: CheckPointingORTrackingAz(); break;
        case 4: CheckPointingORTrackingAlt(); break;
        case 5: CheckPointingORTrackingDero(); break;
        case 6: on_pushButton_save_clicked(); break;
        case 7: on_pushButton_track_clicked(); break;
        case 8: on_pushButton_stop_clicked(); break;
        case 9: on_pushButton_AzOffset_clicked(); break;
        case 10: on_pushButton_AltOffset_clicked(); break;
        case 11: on_pushButton_DeroOffset_clicked(); break;
        case 12: on_pushButton_calculate_clicked(); break;
        case 13: calculate_thread_finished(); break;
        case 14: on_pushButton_enableAll_clicked(); break;
        case 15: TrackreplyOCS(); break;
        case 16: { bool _r = saveStarTable((*reinterpret_cast< double(*)[]>(_a[1])),(*reinterpret_cast< double(*)[]>(_a[2])),(*reinterpret_cast< double(*)[]>(_a[3])),(*reinterpret_cast< double(*)[]>(_a[4])),(*reinterpret_cast< double(*)[]>(_a[5])),(*reinterpret_cast< DateTime(*)[]>(_a[6])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 17: { bool _r = getTrackData();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 18: { bool _r = saveTrackData();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 19: { bool _r = storeTrackData();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 20: CalAndShowRms(); break;
        case 21: on_checkBox_Az_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: on_checkBox_Alt_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 23: on_checkBox_Dero_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: on_checkBox_ADC1_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: on_checkBox_ADC2_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: on_pushButton_saveformodel_clicked(); break;
        case 27: on_checkBox_CorrectAuto_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: on_checkBox_PointModel_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 29: on_checkBox_Pixel_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: on_pushButton_paste_clicked(); break;
        case 31: on_pushButton_ADC2_Offset_clicked(); break;
        case 32: on_pushButton_ADC1Offset_clicked(); break;
        case 33: on_checkBox_ALTHALFDERO_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 34;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
