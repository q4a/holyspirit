/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created: Sun 1. Nov 11:10:22 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      22,   11,   11,   11, 0x0a,
      39,   11,   11,   11, 0x0a,
      58,   11,   11,   11, 0x0a,
      67,   11,   11,   11, 0x0a,
      85,   11,   11,   11, 0x0a,
     101,   11,   11,   11, 0x0a,
     118,   11,   11,   11, 0x0a,
     138,   11,   11,   11, 0x0a,
     147,   11,   11,   11, 0x0a,
     158,   11,   11,   11, 0x0a,
     165,   11,   11,   11, 0x0a,
     172,   11,   11,   11, 0x0a,
     183,  181,   11,   11, 0x0a,
     219,  181,   11,   11, 0x0a,
     253,  181,   11,   11, 0x0a,
     288,  181,   11,   11, 0x0a,
     326,  181,   11,   11, 0x0a,
     359,  181,   11,   11, 0x0a,
     390,  181,   11,   11, 0x0a,
     422,  181,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0nouveau()\0s_creerNouveau()\0"
    "s_annulerNouveau()\0ouvrir()\0"
    "importerTileset()\0importerHerbe()\0"
    "importerEntite()\0importerEvenement()\0"
    "sauver()\0sauverAs()\0undo()\0redo()\0"
    "script()\0,\0selectTileset(QTreeWidgetItem*,int)\0"
    "selectHerbe(QTreeWidgetItem*,int)\0"
    "selectEntite(QTreeWidgetItem*,int)\0"
    "selectEvenement(QTreeWidgetItem*,int)\0"
    "addTileset(QTreeWidgetItem*,int)\0"
    "addHerbe(QTreeWidgetItem*,int)\0"
    "addEntite(QTreeWidgetItem*,int)\0"
    "addEvenement(QTreeWidgetItem*,int)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: nouveau(); break;
        case 1: s_creerNouveau(); break;
        case 2: s_annulerNouveau(); break;
        case 3: ouvrir(); break;
        case 4: importerTileset(); break;
        case 5: importerHerbe(); break;
        case 6: importerEntite(); break;
        case 7: importerEvenement(); break;
        case 8: sauver(); break;
        case 9: sauverAs(); break;
        case 10: undo(); break;
        case 11: redo(); break;
        case 12: script(); break;
        case 13: selectTileset((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: selectHerbe((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: selectEntite((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: selectEvenement((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: addTileset((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: addHerbe((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 19: addEntite((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 20: addEvenement((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
