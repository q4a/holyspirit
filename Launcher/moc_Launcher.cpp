/****************************************************************************
** Meta object code from reading C++ file 'Launcher.h'
**
** Created: Wed Aug 19 12:00:30 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Launcher.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Launcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Launcher[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      25,    9,    9,    9, 0x0a,
      43,    9,    9,    9, 0x0a,
      55,    9,    9,    9, 0x0a,
      65,    9,    9,    9, 0x0a,
     100,   82,    9,    9, 0x0a,
     125,   82,    9,    9, 0x0a,
     168,  157,    9,    9, 0x0a,
     199,  192,    9,    9, 0x0a,
     224,    9,    9,    9, 0x2a,

       0        // eod
};

static const char qt_meta_stringdata_Launcher[] = {
    "Launcher\0\0downloadFile()\0downloadAll(bool)\0"
    "LancerJeu()\0Options()\0MettreAJourBDD()\0"
    "progressions,temp\0miseAJourBarre2(int,int)\0"
    "miseAJourBarreTotale(float,int)\0"
    "fait,total\0miseAJourBarre(int,int)\0"
    "erreur\0telechargerFichier(bool)\0"
    "telechargerFichier()\0"
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
        case 4: MettreAJourBDD(); break;
        case 5: miseAJourBarre2((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: miseAJourBarreTotale((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: miseAJourBarre((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: telechargerFichier((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: telechargerFichier(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
