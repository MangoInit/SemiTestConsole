#include "xml.h"

XML::XML()
{

}

void XML::xml_create()
{
    //xml文件存放测试结果
    file_name.append("Semi");
    file_name.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
    file_name.append(".xml");
    qDebug() << file_name;
    QFile test_file(file_name);
    if(!test_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug()<<"create or open write_sest.xml fault!";
    }
    QDomDocument doc;
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml","version=\'1.0\' encoding=\'UTF-8\'");
    doc.appendChild(instruction);

    QDomElement root = doc.createElement("data");
    doc.appendChild(root);

//    QDomElement robot_id = doc.createElement("robot_id");
//    robot_id.setAttribute("机器人编号",785841260);
//    doc.appendChild(robot_id);

//    QDomElement time = doc.createElement("time");
//    time.setAttribute("测试日期","2018-9-21");
//    robot_id.appendChild(time);

//    QDomElement test_man = doc.createElement("test_man");
//    test_man.setAttribute("测试员工","小明");
//    robot_id.appendChild(test_man);

//    QDomElement item_rerult = doc.createElement("item_result");

//    QDomElement item_wifi = doc.createElement("item_wifi");
//    item_wifi.setAttribute("WIFI测试结果","1 1 1");
//    item_rerult.appendChild(item_wifi);

//    QDomElement item_fan = doc.createElement("item_fan");
//    item_fan.setAttribute("风机测试结果","1 1 1");
//    item_rerult.appendChild(item_fan);

//    robot_id.appendChild(item_rerult);

//    root.appendChild(robot_id);

    QTextStream out(&test_file);
    doc.save(out,4);
    test_file.close();
}

void XML::xml_append_map(const QMap< TEST_ITEM, QVector<item_result> > &map_result)
{
    map_xml = map_result;
}

void XML::xml_append_id(QString tr_id)
{
    //read xml to domdocument
    QFile test_file(file_name);
    if(!test_file.open(QFile::ReadOnly | QFile::Text))
        QMessageBox::information(NULL ,"error","无法打开XML文件");
    QDomDocument doc;
    QString errorStr;
    int errorLine,errorColumn;
    if(!doc.setContent(&test_file,false,&errorStr,&errorLine,&errorColumn))
    {
        qDebug()<<"add setcontent error......";
        test_file.close();
    }
    test_file.close();

    QDomElement root = doc.documentElement();
    doc.appendChild(root);

    QDomElement element_robot_id = doc.createElement("robot_id");
    element_robot_id.setAttribute("机器人编号",tr_id);
    doc.appendChild(element_robot_id);


    QDomElement element_item_result = doc.createElement("item_result");

    foreach (TEST_ITEM test_item, map_xml.keys())
    {
        qDebug() << test_item << map_xml[test_item];
        QDomElement element_item;
        QDomText element_text;
        QStringList strlist;
        QString tr;

        switch(test_item)
        {
        case WIFI_CONNECT_TEST:
            element_item = doc.createElement("WIFI");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case RF433_COMM_TEST:
            element_item = doc.createElement("RF433");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case BATT_CHARGE_TEST:
            element_item = doc.createElement("BATT");
            strlist.append(QString::number(map_xml[test_item].at(0)));
            strlist.append(QString::number(map_xml[test_item].at(1)));
            tr = strlist.join(",");
            element_text = doc.createTextNode(tr);
            break;

        case SPEAKER_TEST:
            element_item = doc.createElement("Speaker");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case ULTRASONIC_SENSOR_TEST:
            element_item = doc.createElement("Radiator");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case BUMPER_SENSOR_TEST:
            element_item = doc.createElement("Bumper");
            strlist.append(QString::number(map_xml[test_item].at(0)));
            strlist.append(QString::number(map_xml[test_item].at(1)));
            strlist.append(QString::number(map_xml[test_item].at(2)));
            tr = strlist.join(",");
            element_text = doc.createTextNode(tr);
            break;

        case CLIFF_SENSOR_TEST:
            element_item = doc.createElement("Cliff");
            strlist.append(QString::number(map_xml[test_item].at(0)));
            strlist.append(QString::number(map_xml[test_item].at(1)));
            strlist.append(QString::number(map_xml[test_item].at(2)));
            strlist.append(QString::number(map_xml[test_item].at(3)));
            tr = strlist.join(",");
            element_text = doc.createTextNode(tr);
            break;

        case MAGNETIC_SENSOR_TEST:
            element_item = doc.createElement("Magnetic");
            strlist.append(QString::number(map_xml[test_item].at(0)));
            strlist.append(QString::number(map_xml[test_item].at(1)));
            strlist.append(QString::number(map_xml[test_item].at(2)));
            strlist.append(QString::number(map_xml[test_item].at(3)));
            tr = strlist.join(",");
            element_text = doc.createTextNode(tr);
            break;

        case IMU_SENSOR_TEST:
            element_item = doc.createElement("IMU");
            strlist.append(QString::number(map_xml[test_item].at(0)));
            strlist.append(QString::number(map_xml[test_item].at(1)));
            strlist.append(QString::number(map_xml[test_item].at(2)));
            tr = strlist.join(",");
            element_text = doc.createTextNode(tr);
            break;

        case CLEANING_MODULE_TEST:
            element_item = doc.createElement("Module");
            strlist.append(QString::number(map_xml[test_item].at(0)));
            strlist.append(QString::number(map_xml[test_item].at(1)));
            strlist.append(QString::number(map_xml[test_item].at(2)));
            tr = strlist.join(",");
            element_text = doc.createTextNode(tr);
            break;

        case WHEEL_TEST:
            element_item = doc.createElement("Wheel");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case FAN_TEST:
            element_item = doc.createElement("Fan");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case WORM_TEST:
            element_item = doc.createElement("Worm");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case TOF_FRONT_TEST:
            element_item = doc.createElement("TOF_Front");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case TOF_BEHIND_TEST:
            element_item = doc.createElement("TOF_Behind");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        case WIRE_SENSOR_TEST:
            element_item = doc.createElement("Wire");
            element_text = doc.createTextNode(QString::number(map_xml[test_item].at(0)));
            break;

        default:break;
        }

        element_item.appendChild(element_text);
        element_item_result.appendChild(element_item);
        element_robot_id.appendChild(element_item_result);
        root.appendChild(element_robot_id);
    }


    //write xml
    if(!test_file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug()<<"open for add error!";
    QTextStream out(&test_file);
    doc.save(out,4);
    test_file.close();
}

