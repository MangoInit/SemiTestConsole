#ifndef MAV_H
#define MAV_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QTcpSocket>
#include <QMutex>
#include <QString>
#include <QDebug>
#include "mavlink/common_extra_msg/mavlink.h"


class MAV: public QWidget
{
    Q_OBJECT
public:
    MAV();

    bool tcp_connect_state;
    bool readyToReadFlag;
    QTcpSocket *tcp_s;
    QMutex mutexReceive;
    void run();

    void tcp_sendbuff(char *message ,qint64 len);

signals:

    void dataReceived(mavlink_message_t &msg);

    void is_tcp_connect(bool);


public slots:
    void print_mavlink_msg(const mavlink_message_t &msg);

    void connect_to_robot(QString tcp_addr, QString tcp_port);

    void tcp_connected();

    void tcp_disconnected();

    void tcp_ready_read();

    void displayError(QAbstractSocket::SocketError);
};

#endif // MAV_H
