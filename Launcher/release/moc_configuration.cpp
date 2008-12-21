/****************************************************************************
** Meta object code from reading C++ file 'configuration.h'
**
** Created: Tue 2. Dec 17:38:34 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../configuration.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'configuration.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_OptionsJeu[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      34,   11,   11,   11, 0x0a,
      57,   11,   11,   11, 0x0a,
      76,   11,   11,   11, 0x0a,
      96,   11,   11,   11, 0x0a,
     114,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_OptionsJeu[] = {
    "OptionsJeu\0\0ChangerContraste(int)\0"
    "ChangerLuminosite(int)\0ChangerVolume(int)\0"
    "ChangerLumiere(int)\0ChangerAlpha(int)\0"
    "Quitter()\0"
};

const QMetaObject OptionsJeu::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_OptionsJeu,
      qt_meta_data_OptionsJeu, 0 }
};

const QMetaObject *OptionsJeu::metaObject() const
{
    return &staticMetaObject;
}

void *OptionsJeu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OptionsJeu))
        return static_cast<void*>(const_cast< OptionsJeu*>(this));
    return QDialog::qt_metacast(_clname);
}

int OptionsJeu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ChangerContraste((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: ChangerLuminosite((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: ChangerVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: ChangerLumiere((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: ChangerAlpha((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: Quitter(); break;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
