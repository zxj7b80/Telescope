/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created: Sat Oct 15 12:18:10 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/main_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      28,   11,   11,   11, 0x05,
      47,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      64,   11,   11,   11, 0x0a,
      77,   11,   11,   11, 0x0a,
      89,   11,   11,   11, 0x0a,
     100,   11,   11,   11, 0x0a,
     110,   11,   11,   11, 0x0a,
     126,   11,   11,   11, 0x0a,
     146,  142,   11,   11, 0x0a,
     167,   11,   11,   11, 0x0a,
     180,   11,   11,   11, 0x0a,
     201,   11,   11,   11, 0x0a,
     230,   11,   11,   11, 0x0a,
     257,   11,   11,   11, 0x0a,
     282,   11,   11,   11, 0x0a,
     307,   11,   11,   11, 0x0a,
     323,   11,   11,   11, 0x0a,
     340,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0startAzaltOCS()\0"
    "startTrackingOCS()\0JogFromAndroid()\0"
    "startAzalt()\0startDero()\0startADC()\0"
    "startM2()\0startTracking()\0startTerminal()\0"
    "Jog\0startTerminal(char*)\0startSetup()\0"
    "startSimulationOCS()\0on_actionWindows_triggered()\0"
    "on_actionMotif_triggered()\0"
    "on_actionGtk_triggered()\0"
    "on_actionCDE_triggered()\0showdata_main()\0"
    "receiveMessage()\0on_actionPloter_triggered()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: startAzaltOCS(); break;
        case 1: startTrackingOCS(); break;
        case 2: JogFromAndroid(); break;
        case 3: startAzalt(); break;
        case 4: startDero(); break;
        case 5: startADC(); break;
        case 6: startM2(); break;
        case 7: startTracking(); break;
        case 8: startTerminal(); break;
        case 9: startTerminal((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 10: startSetup(); break;
        case 11: startSimulationOCS(); break;
        case 12: on_actionWindows_triggered(); break;
        case 13: on_actionMotif_triggered(); break;
        case 14: on_actionGtk_triggered(); break;
        case 15: on_actionCDE_triggered(); break;
        case 16: showdata_main(); break;
        case 17: receiveMessage(); break;
        case 18: on_actionPloter_triggered(); break;
        default: ;
        }
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::startAzaltOCS()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MainWindow::startTrackingOCS()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void MainWindow::JogFromAndroid()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
static const uint qt_meta_data_MyApplication[] = {

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

static const char qt_meta_stringdata_MyApplication[] = {
    "MyApplication\0"
};

const QMetaObject MyApplication::staticMetaObject = {
    { &QApplication::staticMetaObject, qt_meta_stringdata_MyApplication,
      qt_meta_data_MyApplication, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MyApplication::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MyApplication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MyApplication::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MyApplication))
        return static_cast<void*>(const_cast< MyApplication*>(this));
    return QApplication::qt_metacast(_clname);
}

int MyApplication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QApplication::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
