#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
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
    bool isUserName(QString message);
    void updateUserList(QString username);

    QHostAddress sHost;
    quint16 sPort;
    bool debug;
    std::queue<QByteArray> messageQueue;
    std::vector<QTcpSocket*> connections;
    QFile fileQueue;

public slots:
    void readMessage();
    void handleClient();
    void deliverMessage();
    void onDisconnect();
    void readFile();

signals:
    void newMessage();

private:
    QTcpServer *server;
};

#endif // SERVER_H
