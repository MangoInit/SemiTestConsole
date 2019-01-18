#ifndef XML_H
#define XML_H

#include <QObject>
#include <QFile>
#include <QtXml>
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <string>
#include "mavlink/common_extra_msg/mavlink.h"
#include <QMap>
#include <QVector>
#include <QDateTime>

enum item_result{
    result_init = 3,
    result_fail = 0,
    result_pass = 1,
};


class XML: public QObject
{
    Q_OBJECT
public:
    XML();

    QString file_name;

    QMap< TEST_ITEM, QVector<item_result> > map_xml;

    void xml_create();

private slots:

    void xml_append_map(const QMap< TEST_ITEM, QVector<item_result> > &map_result);

    void xml_append_id(QString);

};

#endif // XML_H
