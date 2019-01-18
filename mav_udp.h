#ifndef MAV_UDP_H
#define MAV_UDP_H

#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QMutex>
#include "mavlink/common_extra_msg/mavlink.h"
#include <iostream>
#include <QNetworkInterface>
#include <QHostInfo>

#define Narwel_pwd 0xa55a


class MAV_UDP : public QObject
{
    Q_OBJECT

public:
    explicit MAV_UDP(QObject *parent = 0);

    QUdpSocket *udp_s;
    void send_mav_msg(const mavlink_message_t &msg);
    QMutex mutexReceive;

    uint8_t sendbuffer[2048];

    QUdpSocket *udp_connect;
    bool is_connect_ok;

    QHostAddress yang_addr;
//    QHostAddress::AnyIPv4 yang_addr_;
    quint16 yang_port;

    void udp_item_setting(TEST_ITEM item_setting);
    void udp_running(int);
    void udp_going_on();
    QString getIP();

signals:
    void receive_a_mavlink(mavlink_message_t msg);

public slots:

    void readPendingDatagrams();

    void parse_a_mavlink(const mavlink_message_t &msg);

};

#endif // MAV_UDP_H
