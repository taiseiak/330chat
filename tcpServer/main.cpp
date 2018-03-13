#include <QApplication>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QString>
#include <QIODevice>
#include <QThread>
#include <QWidget>
#include <QtCore>


#include <iostream>
#include <stdlib.h>
#include <queue>
#include <vector>

#include "tcpServer.h"


/**
 * Server tries to listen on any port, at the local address. If new connection
 * received, server echos back message received
 * @param QHostAddress host: see documentation http://doc.qt.io/qt-5/qhostaddress.html
 * @param quint16 port: templated integer format of the port to listen on
 */
tcpServer::tcpServer(QHostAddress host, quint16 port, bool debug) :
    sHost(host), sPort(port), debug(debug) { }


void tcpServer::run()
{
    this->server = new QTcpServer(this);

    connect(this->server, SIGNAL(newConnection()), this, SLOT(handleClient()));
    connect(this, SIGNAL(newMessage()), this, SLOT(deliverMessage()));

    try
    {
        this->server->listen(this->sHost, this->sPort);
        QString hostString = sHost.toString();
        std::cout << "Listening on: " << hostString.toStdString() <<"  with any available port" << std::endl;
    }
    catch(int ExNum)
    {
        std::cout << "Could not listen. Check host and port -- Exit Num: " << ExNum << std::endl;
    }
}


void tcpServer::handleClient()
{

    QTcpSocket* socket = this->server->nextPendingConnection();
    qintptr socketDescriptor = socket->socketDescriptor();

    qDebug() << QString("Handling new client connection: ");
    this->connections.push_back(socket);

    // Connect all necessary events to new socket connection
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}


void tcpServer::readMessage()
{
    // On call, Qt gives access to the sender through the following method.
    // See docs at http://doc.qt.io/qt-5/qobject.html#sender
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());

    QByteArray buffer;
    QByteArray message;
    quint64 maxSize = Q_INT64_C(16);
    while(true)
    {
        buffer = socket->read(maxSize);

        if(buffer.isEmpty())
        {
            break;
        }

        message.append(buffer);
    }

    qDebug() << message;

    // Send to message queue to enable sending to all clients
    this->messageQueue.push(message);

    // Emit event signal for server
    emit newMessage();
}


void tcpServer::deliverMessage()
{
    QByteArray message;

    if(this->debug == true)
    {
        qDebug() << QString("Attempting to deliver message");
    }

    if(!this->messageQueue.empty())
    {
        message = this->messageQueue.front();
        this->messageQueue.pop();

        for(auto it = this->connections.begin(); it != this->connections.end(); it++)
        {
            // TODO: Implement separate write method here
            // Also implement disconnect method to ensure no dead connections are being communicated with
            (*it)->write(message);
        }
    }
    else if(this->debug == true)
    {
        qDebug() << QString("No messages available to write.");
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // ::1 is equivalent to the host address. Port 0 listens on any avilable port
    QHostAddress host = QHostAddress("::1");
    quint16 port = 8000;
    bool debug = true;

    tcpServer* server = new tcpServer(host, port, debug);
    server->run();

    return a.exec();
}
