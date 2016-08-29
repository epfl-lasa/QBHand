/****************************************************************************
** Meta object code from reading C++ file 'qbtools.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../handgui/qbtools.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qbtools.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_qbtools_t {
    QByteArrayData data[39];
    char stringdata0[906];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_qbtools_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_qbtools_t qt_meta_stringdata_qbtools = {
    {
QT_MOC_LITERAL(0, 0, 7), // "qbtools"
QT_MOC_LITERAL(1, 8, 22), // "on_connect_btn_clicked"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 22), // "on_getinfo_btn_clicked"
QT_MOC_LITERAL(4, 55, 23), // "on_activate_btn_clicked"
QT_MOC_LITERAL(5, 79, 20), // "on_id_set_pb_clicked"
QT_MOC_LITERAL(6, 100, 23), // "on_get_meas_btn_clicked"
QT_MOC_LITERAL(7, 124, 16), // "populate_port_cb"
QT_MOC_LITERAL(8, 141, 8), // "get_meas"
QT_MOC_LITERAL(9, 150, 8), // "get_curr"
QT_MOC_LITERAL(10, 159, 7), // "get_emg"
QT_MOC_LITERAL(11, 167, 23), // "on_get_curr_btn_clicked"
QT_MOC_LITERAL(12, 191, 21), // "on_set_pid_pb_clicked"
QT_MOC_LITERAL(13, 213, 23), // "on_set_pid_c_pb_clicked"
QT_MOC_LITERAL(14, 237, 31), // "on_set_control_mode_btn_clicked"
QT_MOC_LITERAL(15, 269, 29), // "on_set_input_mode_btn_clicked"
QT_MOC_LITERAL(16, 299, 22), // "on_set_res_btn_clicked"
QT_MOC_LITERAL(17, 322, 26), // "on_set_offsets_btn_clicked"
QT_MOC_LITERAL(18, 349, 23), // "on_set_zero_btn_clicked"
QT_MOC_LITERAL(19, 373, 24), // "on_scan_port_btn_clicked"
QT_MOC_LITERAL(20, 398, 27), // "on_calib_enable_btn_clicked"
QT_MOC_LITERAL(21, 426, 28), // "on_set_pos_limits_pb_clicked"
QT_MOC_LITERAL(22, 455, 29), // "on_set_curr_limits_pb_clicked"
QT_MOC_LITERAL(23, 485, 27), // "on_reset_default_pb_pressed"
QT_MOC_LITERAL(24, 513, 34), // "on_reset_default_limits_pb_pr..."
QT_MOC_LITERAL(25, 548, 20), // "on_cmd_id_pb_clicked"
QT_MOC_LITERAL(26, 569, 24), // "on_bootloader_pb_clicked"
QT_MOC_LITERAL(27, 594, 27), // "on_set_absolute_enc_clicked"
QT_MOC_LITERAL(28, 622, 22), // "on_set_startup_clicked"
QT_MOC_LITERAL(29, 645, 23), // "on_pos_le_returnPressed"
QT_MOC_LITERAL(30, 669, 28), // "on_set_pwm_rescaling_clicked"
QT_MOC_LITERAL(31, 698, 29), // "on_set_emg_max_val_pb_clicked"
QT_MOC_LITERAL(32, 728, 25), // "on_set_emg_thr_pb_clicked"
QT_MOC_LITERAL(33, 754, 23), // "on_emg_calib_pb_clicked"
QT_MOC_LITERAL(34, 778, 23), // "on_emg_speed_pb_clicked"
QT_MOC_LITERAL(35, 802, 27), // "on_get_emg_meas_btn_clicked"
QT_MOC_LITERAL(36, 830, 24), // "on_record_emg_pb_clicked"
QT_MOC_LITERAL(37, 855, 14), // "plot_emg_graph"
QT_MOC_LITERAL(38, 870, 35) // "on_reset_default_max_val_pb_c..."

    },
    "qbtools\0on_connect_btn_clicked\0\0"
    "on_getinfo_btn_clicked\0on_activate_btn_clicked\0"
    "on_id_set_pb_clicked\0on_get_meas_btn_clicked\0"
    "populate_port_cb\0get_meas\0get_curr\0"
    "get_emg\0on_get_curr_btn_clicked\0"
    "on_set_pid_pb_clicked\0on_set_pid_c_pb_clicked\0"
    "on_set_control_mode_btn_clicked\0"
    "on_set_input_mode_btn_clicked\0"
    "on_set_res_btn_clicked\0"
    "on_set_offsets_btn_clicked\0"
    "on_set_zero_btn_clicked\0"
    "on_scan_port_btn_clicked\0"
    "on_calib_enable_btn_clicked\0"
    "on_set_pos_limits_pb_clicked\0"
    "on_set_curr_limits_pb_clicked\0"
    "on_reset_default_pb_pressed\0"
    "on_reset_default_limits_pb_pressed\0"
    "on_cmd_id_pb_clicked\0on_bootloader_pb_clicked\0"
    "on_set_absolute_enc_clicked\0"
    "on_set_startup_clicked\0on_pos_le_returnPressed\0"
    "on_set_pwm_rescaling_clicked\0"
    "on_set_emg_max_val_pb_clicked\0"
    "on_set_emg_thr_pb_clicked\0"
    "on_emg_calib_pb_clicked\0on_emg_speed_pb_clicked\0"
    "on_get_emg_meas_btn_clicked\0"
    "on_record_emg_pb_clicked\0plot_emg_graph\0"
    "on_reset_default_max_val_pb_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_qbtools[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      37,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  199,    2, 0x08 /* Private */,
       3,    0,  200,    2, 0x08 /* Private */,
       4,    0,  201,    2, 0x08 /* Private */,
       5,    0,  202,    2, 0x08 /* Private */,
       6,    0,  203,    2, 0x08 /* Private */,
       7,    0,  204,    2, 0x08 /* Private */,
       8,    0,  205,    2, 0x08 /* Private */,
       9,    0,  206,    2, 0x08 /* Private */,
      10,    0,  207,    2, 0x08 /* Private */,
      11,    0,  208,    2, 0x08 /* Private */,
      12,    0,  209,    2, 0x08 /* Private */,
      13,    0,  210,    2, 0x08 /* Private */,
      14,    0,  211,    2, 0x08 /* Private */,
      15,    0,  212,    2, 0x08 /* Private */,
      16,    0,  213,    2, 0x08 /* Private */,
      17,    0,  214,    2, 0x08 /* Private */,
      18,    0,  215,    2, 0x08 /* Private */,
      19,    0,  216,    2, 0x08 /* Private */,
      20,    0,  217,    2, 0x08 /* Private */,
      21,    0,  218,    2, 0x08 /* Private */,
      22,    0,  219,    2, 0x08 /* Private */,
      23,    0,  220,    2, 0x08 /* Private */,
      24,    0,  221,    2, 0x08 /* Private */,
      25,    0,  222,    2, 0x08 /* Private */,
      26,    0,  223,    2, 0x08 /* Private */,
      27,    0,  224,    2, 0x08 /* Private */,
      28,    0,  225,    2, 0x08 /* Private */,
      29,    0,  226,    2, 0x08 /* Private */,
      30,    0,  227,    2, 0x08 /* Private */,
      31,    0,  228,    2, 0x08 /* Private */,
      32,    0,  229,    2, 0x08 /* Private */,
      33,    0,  230,    2, 0x08 /* Private */,
      34,    0,  231,    2, 0x08 /* Private */,
      35,    0,  232,    2, 0x08 /* Private */,
      36,    0,  233,    2, 0x08 /* Private */,
      37,    0,  234,    2, 0x08 /* Private */,
      38,    0,  235,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void qbtools::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        qbtools *_t = static_cast<qbtools *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_connect_btn_clicked(); break;
        case 1: _t->on_getinfo_btn_clicked(); break;
        case 2: _t->on_activate_btn_clicked(); break;
        case 3: _t->on_id_set_pb_clicked(); break;
        case 4: _t->on_get_meas_btn_clicked(); break;
        case 5: _t->populate_port_cb(); break;
        case 6: _t->get_meas(); break;
        case 7: _t->get_curr(); break;
        case 8: _t->get_emg(); break;
        case 9: _t->on_get_curr_btn_clicked(); break;
        case 10: _t->on_set_pid_pb_clicked(); break;
        case 11: _t->on_set_pid_c_pb_clicked(); break;
        case 12: _t->on_set_control_mode_btn_clicked(); break;
        case 13: _t->on_set_input_mode_btn_clicked(); break;
        case 14: _t->on_set_res_btn_clicked(); break;
        case 15: _t->on_set_offsets_btn_clicked(); break;
        case 16: _t->on_set_zero_btn_clicked(); break;
        case 17: _t->on_scan_port_btn_clicked(); break;
        case 18: _t->on_calib_enable_btn_clicked(); break;
        case 19: _t->on_set_pos_limits_pb_clicked(); break;
        case 20: _t->on_set_curr_limits_pb_clicked(); break;
        case 21: _t->on_reset_default_pb_pressed(); break;
        case 22: _t->on_reset_default_limits_pb_pressed(); break;
        case 23: _t->on_cmd_id_pb_clicked(); break;
        case 24: _t->on_bootloader_pb_clicked(); break;
        case 25: _t->on_set_absolute_enc_clicked(); break;
        case 26: _t->on_set_startup_clicked(); break;
        case 27: _t->on_pos_le_returnPressed(); break;
        case 28: _t->on_set_pwm_rescaling_clicked(); break;
        case 29: _t->on_set_emg_max_val_pb_clicked(); break;
        case 30: _t->on_set_emg_thr_pb_clicked(); break;
        case 31: _t->on_emg_calib_pb_clicked(); break;
        case 32: _t->on_emg_speed_pb_clicked(); break;
        case 33: _t->on_get_emg_meas_btn_clicked(); break;
        case 34: _t->on_record_emg_pb_clicked(); break;
        case 35: _t->plot_emg_graph(); break;
        case 36: _t->on_reset_default_max_val_pb_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject qbtools::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_qbtools.data,
      qt_meta_data_qbtools,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *qbtools::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *qbtools::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_qbtools.stringdata0))
        return static_cast<void*>(const_cast< qbtools*>(this));
    return QWidget::qt_metacast(_clname);
}

int qbtools::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 37)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 37;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 37)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 37;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
