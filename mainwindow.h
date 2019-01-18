#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QIcon>
#include <QPushButton>
#include <mav.h>
#include <mav_udp.h>
#include <QtNetwork>
#include <QHostAddress>
#include <QTime>
#include <led.h>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string>
#include <QStringList>
#include <uart_robot.h>
#include <QMap>
#include <QVector>
#include <TestResult.h>
#include <QtXml>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <QCloseEvent>
#include <xml.h>

//class _LED;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMap< TEST_ITEM, QVector<item_result> > map_test;


    MAV_UDP *mav_udp;
    bool mav_udp_is;
    QTimer *timer_connect;
    QTimer *timer_worm;
    QSerialPort *uart_robot;
    QSerialPort *uart_rf433;
    QProcess *pro_http;
    XML *xml;

    LED_lamp *LED_connect_state;

    bool b_paint_event;

    LED_lamp *LED_module[3];
    uint8_t num_module_led;
    bool module_bool;

    LED_lamp *LED_BATT[2];
    uint8_t num_batt_led;
    bool batt_bool;

    LED_lamp *LED_WHEEL;
    bool wheel_bool;

    LED_lamp *LED_FAN;
    bool fan_bool;

    LED_lamp *LED_TOF_FRONT;
    bool tof_front_bool;

    LED_lamp *LED_TOF_BEHIND;
    bool tof_behind_bool;

    LED_lamp *LED_BUMPER[3];
    uint8_t num_bumper_led;
    bool bumper_bool;

    LED_lamp *LED_MAGNETIC[4];
    uint8_t num_magnetic_led;
    bool magnetic_bool;

    LED_lamp *LED_WORM;
    uint8_t num_worm_led;
    bool worm_bool;

    LED_lamp *LED_CLIFF[4];
    uint8_t num_cliff_led;
    bool cliff_bool;

    LED_lamp *LED_WIRE;
    bool wire_bool;

    LED_lamp *LED_WIFI_CONNECT;
    bool wifi_connect_bool;

    LED_lamp *LED_IMU[3];
    uint8_t num_imu_led;
    bool imu_bool;

    LED_lamp *LED_RF433;
    bool rf433_bool;

    bool speaker_bool;
    bool radiator_bool;

    uint8_t sendbuffer[2048];


    QVector<item_result> vector_module;
    QVector<item_result> vector_fan;
    QVector<item_result> vector_wheel;
    QVector<item_result> vector_bumper;
    QVector<item_result> vector_batt;
    QVector<item_result> vector_magnetic;
    QVector<item_result> vector_cliff;
    QVector<item_result> vector_ultrasonic;
    QVector<item_result> vector_worm;
    QVector<item_result> vector_speaker;
    QVector<item_result> vector_radiator;
    QVector<item_result> vector_tof_front;
    QVector<item_result> vector_tof_behind;
    QVector<item_result> vector_wire;
    QVector<item_result> vector_wifi_connect;
    QVector<item_result> vector_imu;
    QVector<item_result> vector_rf433;

    void paintEvent(QPaintEvent *event);

    void Painter_Circle(LED_lamp* led);
    void Painter_Picture();
    QString AsciiToHex(QString string);
    uint8_t StrToChar(const char *str);
    uint32_t strToRF433Code();
    void clear_vector();

signals:

    void test_udp_signal();

    void xml_stringlist(QStringList[]);

    void xml_map(const QMap< TEST_ITEM, QVector<item_result> >);

private slots:

    void on_pushButton_wifi_config_clicked();

    void on_pushButton_result_clicked();

    void on_pushButton_connect_robot_clicked();

    void LED_connect_show();

    void uart_robot_receive();

    void uart_rf433_receive();

    void on_pushButton_Open_Port_clicked(bool checked);

    void show_reporting_mavlink(mavlink_message_t msg);

    void on_pushButton_end_clicked();

    void closeEvent(QCloseEvent *event);

    void on_pushButton_speaker_clicked();

    void on_pushButton_test_start_clicked();

    void on_pushButton_finish_spin_clicked();

private:

    Ui::MainWindow *ui;
    QMap< TEST_ITEM, QVector<item_result> > *map_item;
    QMap< TEST_ITEM, QVector<item_result> >::iterator item_iter;
    QMap< TEST_ITEM, QVector<item_result> >::iterator iter;


};

#endif // MAINWINDOW_H
