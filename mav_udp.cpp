#include "mav_udp.h"

MAV_UDP::MAV_UDP(QObject *parent) : QObject(parent)
{
    udp_s = new QUdpSocket;
    udp_s->bind(45454,QUdpSocket::ShareAddress);

    is_connect_ok = false;

    connect(udp_s,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));
    connect(this,SIGNAL(receive_a_mavlink(mavlink_message_t)),this,SLOT(parse_a_mavlink(mavlink_message_t)));
}


void MAV_UDP::send_mav_msg(const mavlink_message_t &msg)
{
//    qDebug()<<"send mavlink:"<<msg.sysid<<msg.compid<<msg.msgid<<msg.payload64;
    int len = mavlink_msg_to_send_buffer(sendbuffer,&msg);
    udp_s->writeDatagram((char*)sendbuffer,len,yang_addr,yang_port);

}

void MAV_UDP::udp_item_setting(TEST_ITEM item)
{
    mavlink_message_t msg_item_setting;
    mavlink_test_item_setting_t item_setting;
    item_setting.code = Narwel_pwd;
    item_setting.item = item;
    mavlink_msg_test_item_setting_encode(1,1,&msg_item_setting,&item_setting);
    send_mav_msg(msg_item_setting);
    qDebug()<<"send a item setting mavlink, number is"<<item;
}

void MAV_UDP::udp_running(int code)
{
    mavlink_message_t msg_running;
    mavlink_test_running_t running;
//    running.code = 0xa5;
    running.code = code;
    mavlink_msg_test_running_encode(1,1,&msg_running,&running);
    send_mav_msg(msg_running);
}

void MAV_UDP::udp_going_on()
{
    mavlink_message_t msg_going_on;
    mavlink_test_going_on_t going_on;
    going_on.code = 0xa5;
    mavlink_msg_test_going_on_encode(1,1,&msg_going_on,&going_on);
    send_mav_msg(msg_going_on);
}

void MAV_UDP::readPendingDatagrams()
{
    while(udp_s->hasPendingDatagrams())
    {
        QByteArray receive_buffer;
        receive_buffer.resize(udp_s->pendingDatagramSize());
        udp_s->readDatagram(receive_buffer.data(),receive_buffer.size(),&yang_addr,&yang_port);

        //shake hands
        if(is_connect_ok == false)
        {
            if(strcmp(receive_buffer,"Narwel_Robot")==0)
            {
                QByteArray send_buffer;
                send_buffer = "Narwel_Server+";
                send_buffer.append(getIP().toLatin1());
                qDebug()<<"sender's host and prot: "<<yang_addr.toString()<<yang_port;
                qDebug()<<"what's I send to Robot:"<<send_buffer;
                udp_s->writeDatagram(send_buffer,yang_addr,yang_port);
            }
            if(strcmp(receive_buffer,"Narwel_Ready")==0)
            {
                is_connect_ok = true;
                qDebug()<<"Start to semi-test.";
            }
        }
        //receive mavlink
        else
        {
            if(receive_buffer.size()>0)
            {
                mavlink_message_t msg;
                mavlink_status_t status;
                for(int i=0;i<receive_buffer.length();i++)
                {
                    int stas = mavlink_parse_char(1,receive_buffer.at(i),&msg,&status);
                    if(stas == MAVLINK_FRAMING_OK)
                    {
//                        qDebug()<<"Receive mavlink: "<<msg.sysid<<msg.compid<<msg.msgid<<msg.payload64;
                        emit receive_a_mavlink(msg);
                    }
                }
            }
        }
    }
}

void MAV_UDP::parse_a_mavlink(const mavlink_message_t &msg)
{
    if(msg.msgid == MAVLINK_MSG_ID_TEST_REPORTING)
    {
        mavlink_test_reporting_t reporting;
        mavlink_msg_test_reporting_decode(&msg,&reporting);
//        qDebug()<<"this is a reporting mavlink "<<reporting.contant<<reporting.result<<reporting.state;
    }
}

QString MAV_UDP::getIP()  //获取ip地址
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
       {
            if(address.toString().contains("192.168")==false)
                continue;
            return address.toString();
       }
    }
    return "IP Fail!";
}
