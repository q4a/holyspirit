/****************************************************************************
** Meta object code from reading C++ file 'Launcher.h'
**
** Created: Fri 29. Aug 12:10:20 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Launcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Launcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Launcher[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      25,    9,    9,    9, 0x0a,
      43,    9,    9,    9, 0x0a,
      55,    9,    9,    9, 0x0a,
      83,   65,    9,    9, 0x0a,
     119,  108,    9,    9, 0x0a,
     143,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Launcher[] = {
    "Launcher\0\0downloadFile()\0downloadAll(bool)\0"
    "LancerJeu()\0Options()\0progressions,temp\0"
    "miseAJourBarre2(int,int)\0fait,total\0"
    "miseAJourBarre(int,int)\0"
    "telechargerFichier(bool)\0"
};

const QMetaObject Launcher::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Launcher,
      qt_meta_data_Launcher, 0 }
};

const QMetaObject *Launcher::metaObject() const
{
    return &staticMetaObject;
}

void *Launcher::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Launcher))
        return static_cast<void*>(const_cast< Launcher*>(this));
    return QWidget::qt_metacast(_clname);
}

int Launcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: downloadFile(); break;
        case 1: downloadAll((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: LancerJeu(); break;
        case 3: Options(); break;
        case 4: miseAJourBarre2((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: miseAJourBarre((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: telechargerFichier((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
