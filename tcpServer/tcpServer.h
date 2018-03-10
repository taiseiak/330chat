#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <stdlib.h>

class tcpServer : public QObject
{
    Q_OBJECT

public:
    tcpServer(QHostAddress h, quint16 p, bool debug);
    void run();

    QHostAddress sHost;
    quint16 sPort;
    bool debug;

public slots:
    void echo();

private:
    QTcpServer *server;
};

#endif // SERVER_H
