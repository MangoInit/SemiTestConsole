#include "mav.h"

MAV::MAV()
{
    tcp_s = new QTcpSocket;

    tcp_connect_state = false;

    connect(tcp_s,SIGNAL(readyRead()),this,SLOT(tcp_ready_read()));
    connect(tcp_s, SIGNAL(connected()), this, SLOT(tcp_connected()));
    connect(tcp_s, SIGNAL(disconnected()), this, SLOT(tcp_disconnected()));
    connect(tcp_s,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));  //连接失败或者断开

    connect(this,SIGNAL(dataReceived(mavlink_message_t)),this,SLOT(print_mavlink_msg(mavlink_message_t)));

}



void MAV::connect_to_robot(QString tcp_addr, QString tcp_port)
{
    qDebug()<<"start to connect to robot......";
    tcp_connect_state = false;
    tcp_s->abort();
    tcp_s->connectToHost(tcp_addr,tcp_port.toUInt());

    tcp_s->write(QByteArray("What are you doing?"));
}

void MAV::tcp_connected()
{
    tcp_connect_state = true;
    emit is_tcp_connect(tcp_connect_state);
    qDebug()<<"tcp to robot has been connected.";
}

void MAV::tcp_disconnected()
{
    tcp_connect_state = false;
    emit is_tcp_connect(tcp_connect_state);
    qDebug()<<"tcp to robot disconnected!!!";
}

void MAV::tcp_ready_read()
{
    QString msg = QString(tcp_s->readAll());
    if(msg!=NULL)
    {
        qDebug()<<"Receive from robot: ";
    }
    qDebug()<<msg;
}

void MAV::displayError(QAbstractSocket::SocketError)
{
   qDebug()<<tcp_s->errorString();
   qDebug()<<"can't find the right address!!!";
   tcp_connect_state = false;
}


//收
void MAV::run()
{
    int numBytes = 0;
    QByteArray data;
    qDebug()<<"coming to tcp connection run() function";
    while (tcp_connect_state)
    {
        if(readyToReadFlag)
        {
            readyToReadFlag=false;
            numBytes = tcp_s->bytesAvailable();
            if (numBytes>0)
            {
                mutexReceive.lock();
                data = tcp_s->read(numBytes);
                mutexReceive.unlock();
                //emit send_tcp_buff(data);
                mavlink_message_t msg;
                mavlink_status_t status;
                for(int i=0;i<data.length();i++)
                {
                    int stats=mavlink_parse_char(1,data.at(i),&msg,&status);
                    if(stats==MAVLINK_FRAMING_OK)
                    {
                        emit dataReceived(msg);
                    }
                }

            }
        }
        //else msleep(10);
    }
    qDebug()<<"out of tcp connection run() function";
}


void MAV::tcp_sendbuff(char *message, qint64 len)
{
    tcp_s->write(message,len);
}

void MAV::print_mavlink_msg(const mavlink_message_t &msg)
{
    switch (msg.msgid)
    {
        case 1:;

    }
}
