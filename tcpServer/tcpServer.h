#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <stdlib.h>
#include <iostream>
#include <queue>
#include <vector>

class tcpServer : public QObject
{
    Q_OBJECT

public:
    tcpServer(QHostAddress h, quint16 p, bool debug);

    void run();

    // Message messageHandler();
    // Will return a message object with all necessary information taken from message passed from client

    QHostAddress sHost;
    quint16 sPort;
    bool debug;
    std::queue<QByteArray> messageQueue;
    std::vector<QTcpSocket*> connections;

public slots:
    void readMessage();
    void handleClient();
    void deliverMessage();

signals:
    void newMessage();

private:
    QTcpServer *server;
};

#endif // SERVER_H
