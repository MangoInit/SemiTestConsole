#ifndef UART_ROBOT_H
#define UART_ROBOT_H

#include <QObject>

class UART_ROBOT : public QObject
{
    Q_OBJECT
public:
    explicit UART_ROBOT(QObject *parent = 0);

signals:

public slots:
};

#endif // UART_ROBOT_H
