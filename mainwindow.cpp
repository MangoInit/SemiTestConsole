#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("半成品测试上位机");
    this->setWindowIcon(QIcon(":/image/logo.png"));

    mav_udp_is = false;
    LED_connect_state = new LED_lamp(115,400,35);

    LED_WIFI_CONNECT = new LED_lamp(620,60,25);
    LED_RF433 = new LED_lamp(620,105,25);
    LED_WHEEL = new LED_lamp(620,150,25);
    LED_FAN = new LED_lamp(620,195,25);
    LED_WORM = new LED_lamp(620,243,25);
    LED_WIRE = new LED_lamp(620,293,25);
    LED_BATT[0] = new LED_lamp(620,343,25);
    LED_BATT[1] = new LED_lamp(655,343,25);
    LED_BUMPER[0] = new LED_lamp(620,390,25);
    LED_BUMPER[1] = new LED_lamp(655,390,25);
    LED_BUMPER[2] = new LED_lamp(690,390,25);

    LED_TOF_FRONT = new LED_lamp(850,60,25);
    LED_TOF_BEHIND = new LED_lamp(850,105,25);
    LED_CLIFF[0] = new LED_lamp(850,385,25);
    LED_CLIFF[1] = new LED_lamp(885,385,25);
    LED_CLIFF[2] = new LED_lamp(920,385,25);
    LED_CLIFF[3] = new LED_lamp(955,385,25);
    LED_MAGNETIC[0] = new LED_lamp(850,340,25);
    LED_MAGNETIC[1] = new LED_lamp(885,340,25);
    LED_MAGNETIC[2] = new LED_lamp(920,340,25);
    LED_MAGNETIC[3] = new LED_lamp(955,340,25);
    LED_IMU[0] = new LED_lamp(850,295,25);
    LED_IMU[1] = new LED_lamp(885,295,25);
    LED_IMU[2] = new LED_lamp(920,295,25);
    LED_module[0] = new LED_lamp(850,153,25);
    LED_module[1] = new LED_lamp(850,203,25);
    LED_module[2] = new LED_lamp(850,248,25);



    timer_connect = new QTimer;
    timer_worm = new QTimer;

    uart_robot = new QSerialPort(this);
    uart_rf433 = new QSerialPort(this);

    pro_http = new QProcess;
    pro_http->start("python -m SimpleHTTPServer 8080");       //root模式下有效

    xml = new XML;
    connect(this, SIGNAL(xml_map(QMap< TEST_ITEM, QVector<item_result> >)), xml, SLOT(xml_append_map(QMap< TEST_ITEM, QVector<item_result> >)));
    xml->xml_create();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    Painter_Circle(LED_connect_state);

    if(b_paint_event == true)
    {
        Painter_Circle(LED_WIFI_CONNECT);
        Painter_Circle(LED_RF433);
        Painter_Circle(LED_WHEEL);
        Painter_Circle(LED_FAN);
        Painter_Circle(LED_WORM);
        Painter_Circle(LED_BATT[0]);
        Painter_Circle(LED_BATT[1]);
        Painter_Circle(LED_WIRE);
        Painter_Circle(LED_BUMPER[0]);
        Painter_Circle(LED_BUMPER[1]);
        Painter_Circle(LED_BUMPER[2]);

        Painter_Circle(LED_TOF_FRONT);
        Painter_Circle(LED_TOF_BEHIND);
        Painter_Circle(LED_CLIFF[0]);
        Painter_Circle(LED_CLIFF[1]);
        Painter_Circle(LED_CLIFF[2]);
        Painter_Circle(LED_CLIFF[3]);
        Painter_Circle(LED_MAGNETIC[0]);
        Painter_Circle(LED_MAGNETIC[1]);
        Painter_Circle(LED_MAGNETIC[2]);
        Painter_Circle(LED_MAGNETIC[3]);
        Painter_Circle(LED_IMU[0]);
        Painter_Circle(LED_IMU[1]);
        Painter_Circle(LED_IMU[2]);
        Painter_Circle(LED_module[0]);
        Painter_Circle(LED_module[1]);
        Painter_Circle(LED_module[2]);

        Painter_Picture();
    }

}

void MainWindow::Painter_Circle(LED_lamp *led)
{
    QPainter painter(this);
//    QPen pen(Qt::black,1);
//    painter.setPen(pen);
    painter.setBrush(led->LED_Color);
    painter.drawEllipse(led->LED_X,led->LED_Y,led->LED_R,led->LED_R);
    update();
}

void MainWindow::Painter_Picture()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPixmap pix;
    pix.load(":/image/narwel.png");
    qreal width = pix.width(); //获得以前图片的宽和高
    qreal height = pix.height();
    //将图片的宽和高都扩大两倍，并且在给定的矩形内保持宽高的比值不变
    pix = pix.scaled(width*2, height*2, Qt::KeepAspectRatio);

    painter.drawPixmap(20,470,454/2,563/2,pix);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*-----------------------------------------串口设置----------------------------------------------*/

void MainWindow::on_pushButton_Open_Port_clicked(bool checked)
{
    if(checked)
    {
        QSerialPortInfo com_info_robot;
        QSerialPortInfo com_info_station;
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            if(info.portName().at(3) == QString("U"))
            {
                qDebug() << "Name : " << info.portName();
                qDebug() << "Description : " << info.description();
                qDebug() << "serialNumber: " << info.serialNumber();
                qDebug() << "System Location: " << info.systemLocation();
                qDebug() << "Manufacturer: " << info.manufacturer();
                qDebug() << "ProductIdentifier:" << info.productIdentifier();
                qDebug() << "";
            }

            if( info.portName() == "ttyUSB0" )
//            if(info.manufacturer() == "Silicon Labs")       //目前采用"ＥＡＩ" usb转串口模块
            {
                com_info_robot = info;
                uart_robot->setPort(com_info_robot);
            }

//            if( info.portName() == "ttyUSB1")
//            {
//                com_info_station = info;
//                uart_rf433->setPort(com_info_station);
//            }
        }

        if(uart_robot->open(QIODevice::ReadWrite))
        {
            qDebug()<<"Uart_Robot is opening......\n";
            ui->textBrowser_contant->setText("机器人串口打开成功！");
            ui->pushButton_Open_Port->setText("关闭串口");
            ui->pushButton_Open_Port->setChecked(true);
            uart_robot->setBaudRate(QSerialPort::Baud115200);
            uart_robot->setParity(QSerialPort::NoParity);
            uart_robot->setDataBits(QSerialPort::Data8);
            uart_robot->setStopBits(QSerialPort::OneStop);
            uart_robot->setFlowControl(QSerialPort::NoFlowControl);

            ui->pushButton_wifi_config->setEnabled(true);

            //        connect(uart_robot,SIGNAL(readyRead()),this,SLOT(uart_robot_receive()));
        }
        else
        {
            ui->pushButton_Open_Port->setChecked(false);
            ui->pushButton_wifi_config->setEnabled(false);
            ui->textBrowser_contant->setText("机器人串口打开失败！");
        }

        if(uart_rf433->open(QIODevice::ReadWrite))
        {
            qDebug()<<"Uart_RF433 is opening......\n";
            uart_rf433->setBaudRate(QSerialPort::Baud115200);
            uart_rf433->setParity(QSerialPort::NoParity);
            uart_rf433->setDataBits(QSerialPort::Data8);
            uart_rf433->setStopBits(QSerialPort::OneStop);
            uart_rf433->setFlowControl(QSerialPort::NoFlowControl);

            connect(uart_rf433,SIGNAL(readyRead()),this,SLOT(uart_rf433_receive()));
        }
    }
    else
    {
        qDebug() << "Uart_Robot is close.";
        uart_robot->close();
        ui->pushButton_Open_Port->setChecked(false);
        ui->pushButton_Open_Port->setText("打开串口");
        ui->textBrowser_contant->setText("机器人串口已关闭！");
        ui->pushButton_wifi_config->setEnabled(false);
    }
}

void MainWindow::on_pushButton_wifi_config_clicked()
{
    if(uart_robot->isOpen() == true)
    {
        if(ui->lineEdit_AP->text().isEmpty()==false && ui->lineEdit_PWD->text().isEmpty()==false && ui->lineEdit_rf433->text().length()==11)
        {
            //设置机器人wifi
            mavlink_startr16test_t wifi_test;
            mavlink_message_t msg_wifi;
            QByteArray wifi_ap = ui->lineEdit_AP->text().toLatin1();
            QByteArray wifi_pwd = ui->lineEdit_PWD->text().toLatin1();

            strncpy(wifi_test.ssid,wifi_ap,sizeof(wifi_test.ssid));
            strncpy(wifi_test.passwd,wifi_pwd,sizeof(wifi_test.passwd));
            wifi_test.comm[0] = strToRF433Code();
            wifi_test.comm[1] = 0xfe<<8 | 0x05;
            wifi_test.mode = SEMI_MODE;
            mavlink_msg_startr16test_encode(2,2,&msg_wifi,&wifi_test);

            //串口发送
            int len = mavlink_msg_to_send_buffer(sendbuffer,&msg_wifi);
            qDebug()<<"UART  send  buffer  len: "<<len<<"sendbuffer:"<<sendbuffer<<"(char*)sendbuffer:"<<(const char*)sendbuffer;
            uart_robot->write((const char*)sendbuffer,len);

            //窗口提示
            ui->textBrowser_contant->setText("设置完成！！！\n待联网成功后连接机器人。");
        }
        else
        {
            ui->textBrowser_contant->setText("输入格式错误，请输入正确的WiFi账户密码，RF433通讯码！！！\n");
        }
    }


}

void MainWindow::uart_robot_receive()
{
    QByteArray receive_buffer;
    receive_buffer = uart_robot->readAll();
    qDebug()<<"UART receive buffer len:"<<receive_buffer.length()<<"UART receive buffer: "<<receive_buffer.data();
    mavlink_message_t msg;
    mavlink_status_t status;
    for(int i=0;i<receive_buffer.length();i++)
    {
        int stas = mavlink_parse_char(1,receive_buffer.at(i),&msg,&status);
        if(stas == MAVLINK_FRAMING_OK)
        {
            qDebug()<<"Receive mavlink: "<<msg.sysid<<msg.compid<<msg.msgid<<msg.payload64;
//            emit receive_a_mavlink(msg);
        }
    }
    receive_buffer.clear();
}

void MainWindow::uart_rf433_receive()
{
    //RF433模块接收
    QByteArray receive_buffer;
    receive_buffer = uart_rf433->readAll();
    qDebug() << "Receive buffer from RF433:" << receive_buffer.toHex();
    qDebug() << "ReceiveBuffer's Length:" << receive_buffer.length();

    if(receive_buffer.length() == 7)
    {
        if(receive_buffer.at(0)==char(0xfe) && receive_buffer.at(1)==char(0xc4))
        {
            qDebug() << "Send ID of station to Robot";
        }
    }

    foreach(char ch, receive_buffer)
    {
        QString tr = QString(QChar::fromLatin1(ch));
        qDebug() << tr.toLatin1();
    }

    receive_buffer.clear();

//    char c_data;
//    QString s_data;
//    uart_rf433->getChar(&c_data);
//    s_data = QString::number(c_data,16);
}

/*------------------------------------------------------------END-----------------------------------------------------------------*/

void MainWindow::on_pushButton_connect_robot_clicked()
{
    if(false == mav_udp_is)
    {
        mav_udp = new MAV_UDP;
        connect(mav_udp,SIGNAL(receive_a_mavlink(mavlink_message_t)),this,SLOT(show_reporting_mavlink(mavlink_message_t)));


        connect(timer_connect,SIGNAL(timeout()),this,SLOT(LED_connect_show()));
        timer_connect->start(500);

        map_item = new QMap< TEST_ITEM,QVector<item_result> >;
        clear_vector();

        mav_udp_is = true;
        ui->pushButton_end->setEnabled(true);
        qDebug()<<"new a mav_udp.";

        ui->textBrowser_contant->setText("连接状态灯点绿之后，表示连接成功，可开始测试");
    }
}

void MainWindow::LED_connect_show()
{
    if(mav_udp->is_connect_ok == true)
    {
        LED_connect_state->LED_State = LED_lamp::Pass;
//        ui->pushButton_speaker->setEnabled(true);
//        ui->pushButton_finish_spin->setEnabled(true);
        ui->pushButton_test_start->setEnabled(true);
        ui->pushButton_result->setEnabled(true);
    }
    else
    {
        LED_connect_state->LED_State = LED_lamp::NoPass;
        ui->pushButton_finish_spin->setEnabled(false);
        ui->pushButton_speaker->setEnabled(false);
        ui->pushButton_test_start->setEnabled(false);
        ui->pushButton_result->setEnabled(false);
    }
}



/*------------------------------------------------------------Receive---------------------------------------------------------------*/
void MainWindow::show_reporting_mavlink(mavlink_message_t msg)
{
    if(msg.msgid == MAVLINK_MSG_ID_TEST_REPORTING)
    {
        mavlink_test_reporting_t reporting;
        mavlink_msg_test_reporting_decode(&msg,&reporting);
        qDebug()<<"Reporting Mavlink: "<<"item and index"<<reporting.item<<reporting.index<<"state and result"<<reporting.state<<reporting.result;

        switch(reporting.item)
        {

        case RF433_COMM_TEST:
            if(reporting.result == PASS)
            {
                vector_rf433[0] = result_pass;
                LED_RF433->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_rf433[0] = result_fail; LED_RF433->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(RF433_COMM_TEST,vector_rf433);
            break;

        case IMU_SENSOR_TEST:
            if(reporting.result == PASS)
            {
                vector_imu[reporting.index - 1] = result_pass;
                LED_IMU[reporting.index - 1]->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_imu[reporting.index - 1] = result_fail;
                LED_IMU[reporting.index - 1]->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(IMU_SENSOR_TEST,vector_imu);
            break;

        case WIFI_CONNECT_TEST:
            if(reporting.result == PASS)
            {
                vector_wifi_connect[0] = result_pass;
                LED_WIFI_CONNECT->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_wifi_connect[0] = result_fail;
                LED_WIFI_CONNECT->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(WIFI_CONNECT_TEST,vector_wifi_connect);
            break;

        case WIRE_SENSOR_TEST:
            if(reporting.result == PASS)
            {
                vector_wire[0] = result_pass;
                LED_WIRE->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_wire[0] = result_fail;
                LED_WIRE->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(WIRE_SENSOR_TEST,vector_wire);
            break;

        case TOF_FRONT_TEST:
            if(reporting.result == PASS)
            {
                vector_tof_front[0] = result_pass;
                LED_TOF_FRONT->LED_State = LED_lamp::Pass;
            }
            else if(reporting.result == FAIL)
            {
                vector_tof_front[0] = result_fail;
                LED_TOF_FRONT->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(TOF_FRONT_TEST,vector_tof_front);
            break;

        case TOF_BEHIND_TEST:
            if(reporting.result == PASS)
            {
                vector_tof_behind[0] = result_pass;
                LED_TOF_BEHIND->LED_State = LED_lamp::Pass;
            }
            else if(reporting.result == FAIL)
            {
                vector_tof_behind[0] = result_fail;
                LED_TOF_BEHIND->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(TOF_BEHIND_TEST,vector_tof_behind);
            break;

        case CLIFF_SENSOR_TEST:
            if(reporting.result == PASS)
            {
                vector_cliff[reporting.index - 1] = result_pass;
                LED_CLIFF[reporting.index - 1]->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_cliff[reporting.index - 1] = result_fail;
                LED_CLIFF[reporting.index - 1]->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(CLIFF_SENSOR_TEST,vector_cliff);
            break;

        case WORM_TEST:
            if(reporting.result == PASS)
            {
                vector_worm[0] = result_pass;
                LED_WORM->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_worm[0] = result_fail;
                LED_WORM->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(WORM_TEST,vector_worm);
            break;

        case MAGNETIC_SENSOR_TEST:
            if(reporting.result == PASS)
            {
                vector_magnetic[reporting.index - 1] = result_pass;
                LED_MAGNETIC[reporting.index - 1]->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_magnetic[reporting.index - 1] = result_fail;
                LED_MAGNETIC[reporting.index - 1]->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(MAGNETIC_SENSOR_TEST,vector_magnetic);
            break;

        case CLEANING_MODULE_TEST:
            if(reporting.result == PASS)
            {
                vector_module[reporting.index - 1] = result_pass;
                LED_module[reporting.index - 1]->LED_State = LED_lamp::Pass;
            }
            else if(reporting.result == FAIL)
            {
                vector_module[reporting.index - 1] = result_fail;
                LED_module[reporting.index - 1]->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(CLEANING_MODULE_TEST,vector_module);
            break;

        case FAN_TEST:
            if(reporting.result == PASS)
            {
                vector_fan[0] = result_pass;
                LED_FAN->LED_State = LED_lamp::Pass;
            }
            else if(reporting.result == FAIL)
            {
                vector_fan[0] = result_fail;
                LED_FAN->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(FAN_TEST,vector_fan);
            break;

        case WHEEL_TEST:
            if(reporting.result == PASS)
            {
                vector_wheel[0] = result_pass;
                LED_WHEEL->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_wheel[0] = result_fail;
                LED_WHEEL->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(WHEEL_TEST,vector_wheel);
            break;

        case BUMPER_SENSOR_TEST:
            if(reporting.result == PASS)
            {
                vector_bumper[reporting.index - 1] = result_pass;
                LED_BUMPER[reporting.index - 1]->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_bumper[reporting.index - 1] = result_fail;
                LED_BUMPER[reporting.index - 1]->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(BUMPER_SENSOR_TEST,vector_bumper);
            break;

        case BATT_CHARGE_TEST:
            if(reporting.result == PASS)
            {
                vector_batt[reporting.index - 1] = result_pass;
                LED_BATT[reporting.index - 1]->LED_State = LED_lamp::Pass;
            }
            if(reporting.result == FAIL)
            {
                vector_batt[reporting.index - 1] = result_fail;
                LED_BATT[reporting.index -1]->LED_State = LED_lamp::NoPass;
            }
            map_item->insert(BATT_CHARGE_TEST,vector_batt);
            break;

        default:break;
        }
    }
}

/*--------------------------------------------------------------设置按钮----------------------------------------------------------------*/



QString MainWindow::AsciiToHex(QString string)
{
    QByteArray bytearray1 = string.toUtf8();
    QByteArray bytearray2 = bytearray1.toHex();
    const char* chArr1 = bytearray2.constData();
    return QString::fromUtf8(chArr1);
}

uint8_t MainWindow::StrToChar(const char *str)
{
//    int len = sizeof(str);
    int len = 2;
//    if(len!=2)
//        qDebug()<<len<<"str length has error\n";
    uint8_t value = 0;

    for(int i=0; i<len; i++)
    {
        if(str[i]<='9' && str[i]>='0')
            value = (value<<4) + (str[i]-'0');
        else if(str[i]>='a' && str[i]<='f')
            value = (value<<4) + (str[i]-'a') + 10;
        else if(str[i]>='A' && str[i]<='F')
            value = (value<<4) + (str[i]-'A') + 10;
    }
    return value;
}

uint32_t MainWindow::strToRF433Code()
{
    QString str = ui->lineEdit_rf433->text();
    QStringList strlist = str.split(QRegExp("[ ]"));
    qDebug()<<strlist;
    QString str0 = strlist.at(0);
    QString str1 = strlist.at(1);
    QString str2 = strlist.at(2);
    QString str3 = strlist.at(3);

    const char *ch[4];
    ch[0] = str0.toStdString().c_str();
    ch[1] = str1.toStdString().c_str();
    ch[2] = str2.toStdString().c_str();
    ch[3] = str3.toStdString().c_str();

    uchar *code = new uchar[4];

    code[0] = StrToChar(ch[0]);
    code[1] = StrToChar(ch[1]);
    code[2] = StrToChar(ch[2]);
    code[3] = StrToChar(ch[3]);

    uint32_t A = 0;
    A = code[0]<<24 | code[1]<<16 | code[2]<<8 | code[3];
//    std::cout<<std::hex<<A<<std::endl;
//    qDebug()<<A;
    return A;
}

void MainWindow::clear_vector()
{
    vector_module.fill(result_init,3);
    vector_fan.fill(result_init,1);
    vector_wheel.fill(result_init,1);
    vector_bumper.fill(result_init,3);
    vector_batt.fill(result_init,2);
    vector_magnetic.fill(result_init,4);
    vector_cliff.fill(result_init,4);
    vector_worm.fill(result_init,1);
    vector_speaker.fill(result_init,1);
    vector_tof_front.fill(result_init,1);
    vector_tof_behind.fill(result_init,1);
    vector_wire.fill(result_init,1);
    vector_wifi_connect.fill(result_init,1);
    vector_imu.fill(result_init,3);
    vector_rf433.fill(result_init,1);
    vector_radiator.fill(result_init,1);
}


/*------------------------------------------------------------XML存放测试结果----------------------------------------------------------------*/
void MainWindow::on_pushButton_end_clicked()
{
//    xml->xml_append();

    if(true == mav_udp_is)
    {
        mav_udp->udp_running(0x03);  //power off

        delete mav_udp->udp_s;
        delete mav_udp;


        timer_connect->stop();
        LED_connect_state->LED_State = LED_lamp::Init;

        mav_udp_is = false;
        qDebug()<<"kill mav_udp Class.";

        LED_RF433->LED_State = LED_lamp::Init;
        LED_WIFI_CONNECT->LED_State = LED_lamp::Init;
        LED_RF433->LED_State = LED_lamp::Init;
        LED_WHEEL->LED_State = LED_lamp::Init;
        LED_FAN->LED_State = LED_lamp::Init;
        LED_WORM->LED_State = LED_lamp::Init;
        LED_BATT[0]->LED_State = LED_lamp::Init;
        LED_BATT[1]->LED_State = LED_lamp::Init;
        LED_WIRE->LED_State = LED_lamp::Init;
        LED_BUMPER[0]->LED_State = LED_lamp::Init;
        LED_BUMPER[1]->LED_State = LED_lamp::Init;
        LED_BUMPER[2]->LED_State = LED_lamp::Init;

        LED_TOF_FRONT->LED_State = LED_lamp::Init;
        LED_TOF_BEHIND->LED_State = LED_lamp::Init;
        LED_CLIFF[0]->LED_State = LED_lamp::Init;
        LED_CLIFF[1]->LED_State = LED_lamp::Init;
        LED_CLIFF[2]->LED_State = LED_lamp::Init;
        LED_CLIFF[3]->LED_State = LED_lamp::Init;
        LED_MAGNETIC[0]->LED_State = LED_lamp::Init;
        LED_MAGNETIC[1]->LED_State = LED_lamp::Init;
        LED_MAGNETIC[2]->LED_State = LED_lamp::Init;
        LED_MAGNETIC[3]->LED_State = LED_lamp::Init;
        LED_IMU[0]->LED_State = LED_lamp::Init;
        LED_IMU[1]->LED_State = LED_lamp::Init;
        LED_IMU[2]->LED_State = LED_lamp::Init;
        LED_module[0]->LED_State = LED_lamp::Init;
        LED_module[1]->LED_State = LED_lamp::Init;
        LED_module[2]->LED_State = LED_lamp::Init;

        ui->pushButton_speaker->setEnabled(false);
        ui->pushButton_finish_spin->setEnabled(false);
        ui->pushButton_test_start->setEnabled(false);
        ui->pushButton_result->setEnabled(false);
        ui->pushButton_end->setEnabled(false);

        ui->textBrowser_contant->setText("测试完毕，请进入下一台测试");
    }
}


//遍历一下
void MainWindow::on_pushButton_result_clicked()
{
    TestResult *test_result = new TestResult(this);
    connect(test_result, SIGNAL(signal_save_result(QString)), xml, SLOT(xml_append_id(QString)));
    test_result->show();

    QStringList str_list[20];
    int8_t a = 0;
    for(iter = map_item->begin(); iter!=map_item->end(); ++iter)
    {
        str_list[a].append("\n");
        str_list[a].append(QString::number(a+1));

        switch(iter.key())
        {
        case WIFI_CONNECT_TEST:
            str_list[a].append("WiFi:\t\t");    break;
        case RF433_COMM_TEST:
            str_list[a].append("RF433:\t\t");   break;
        case BATT_CHARGE_TEST:
            str_list[a].append("Battarty:\t");  break;
        case SPEAKER_TEST:
            str_list[a].append("Speaker:\t");   break;
        case BUMPER_SENSOR_TEST:
            str_list[a].append("Bumper:\t");    break;
        case CLIFF_SENSOR_TEST:
            str_list[a].append("Cliff:\t\t");   break;
        case MAGNETIC_SENSOR_TEST:
            str_list[a].append("Magnetic:\t");  break;
        case IMU_SENSOR_TEST:
            str_list[a].append("IMU:\t\t");     break;
        case CLEANING_MODULE_TEST:
            str_list[a].append("Module:\t");    break;
        case WHEEL_TEST:
            str_list[a].append("Wheel:\t");     break;
        case FAN_TEST:
            str_list[a].append("Fan:\t\t");     break;
        case WORM_TEST:
            str_list[a].append("Worm:\t\t");    break;
        case TOF_FRONT_TEST:
            str_list[a].append("TOF_F:\t");     break;
        case TOF_BEHIND_TEST:
            str_list[a].append("TOF_B:\t");     break;
        case ULTRASONIC_SENSOR_TEST:
            str_list[a].append("Radiator:\t");  break;
        case WIRE_SENSOR_TEST:
            str_list[a].append("Wire:\t\t");    break;
        default:                                break;
        }

        for(int8_t i = 0; i!=iter.value().length(); i++)
        {
            str_list[a].append(QString::number(iter.value().at(i)));
        }
        a++;
    }

//    qDebug() << (sizeof(str_list)/sizeof(str_list[0]));
    for(uint8_t i=0; i<(sizeof(str_list)/sizeof(str_list[0])); i++)
    {
        for(QStringList::Iterator it = str_list[i].begin(); it != str_list[i].end(); it++)
        {
            test_result->ui->textBrowser->insertPlainText(" ");
            test_result->ui->textBrowser->insertPlainText(*it);
        }
//        test_result->ui->textBrowser->insertPlainText("\n");
    }

    emit xml_map(*map_item);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    // QMessageBox::StandardButton button;
    int button;
    button = QMessageBox::question(this, tr("退出程序"),
                                QString(tr("确认退出程序?")),
                                QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::No)
    {
       event->ignore();  //忽略退出信号，程序继续运行
    }
    else if (button == QMessageBox::Yes)
    {
        delete pro_http;    //kill http server
        delete ui;
        event->accept();  //接受退出信号，程序退出
    }
}


void MainWindow::on_pushButton_speaker_clicked()
{
    //弹出窗口,输入喇叭测试结果
    QMessageBox::StandardButton speaker_box;
    speaker_box = QMessageBox::question(this,tr("喇叭测试"),"是否听到喇叭声响？",QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if(speaker_box == QMessageBox::Yes)
    {
        vector_speaker[0] = result_pass;
        map_item->insert(SPEAKER_TEST,vector_speaker);
    }
    else if(speaker_box == QMessageBox::No)
    {
        vector_speaker[0] = result_fail;
        map_item->insert(SPEAKER_TEST,vector_speaker);
    }
    else{}

    //弹出窗口,输入散热风扇测试结果
    QMessageBox::StandardButton radiator_box;
    radiator_box = QMessageBox::question(this,tr("散热风扇测试"),"散热风扇是否会转？",QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if(radiator_box == QMessageBox::Yes)
    {
        vector_radiator[0] = result_pass;
        map_item->insert(ULTRASONIC_SENSOR_TEST,vector_radiator);    //超声波代替散热器
    }
    else if(radiator_box == QMessageBox::No)
    {
        vector_radiator[0] = result_fail;
        map_item->insert(ULTRASONIC_SENSOR_TEST,vector_radiator);
    }
    else{}

    ui->textBrowser_contant->setText("请将机器人从治具拿起，待所有灯点亮之后,按下'测试结果'并点击OK可保存测试结果\n"
                                     "点击'测试结束'可进行下一次测试");

}


void MainWindow::on_pushButton_test_start_clicked()
{
    mav_udp->udp_running(0x01);
    ui->pushButton_finish_spin->setEnabled(true);
    ui->textBrowser_contant->setText("1.请将电线检测、充电检测板接线，１秒之后拔开\n"
                                     "2.依次触碰３个Bumper\n"
                                     "3.将磁笔在尘盒模块检测处挥动\n"
                                     "4.请顺时针旋转机器人至180度，停顿１秒后继续旋转至０度\n(如磁传感器4个灯没有电量则再旋转一次)\n"
                                     "5.最后按下旋转完毕");
}


void MainWindow::on_pushButton_finish_spin_clicked()
{
    mav_udp->udp_running(0x02);
    ui->pushButton_speaker->setEnabled(true);
    ui->textBrowser_contant->setText("IMU检测项LED灯电量\n"
                                     "请按下“喇叭&散热器”按键");
}


