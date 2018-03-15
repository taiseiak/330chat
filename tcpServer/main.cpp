#include <QApplication>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QString>
#include <QIODevice>
#include <QThread>
#include <QWidget>
#include <QtCore>
#include <QMap>


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
        std::cout << "Listening on: " << hostString.toStdString() << " " << this-sPort << std::endl;
    }
    catch(int ExNum)
    {
        std::cout << "Could not listen. Check host and port -- Exit Num: " << ExNum << std::endl;
    }
}


void tcpServer::handleClient()
{
    QTcpSocket* socket = this->server->nextPendingConnection();

    qDebug() << "Handling new client connection";
    this->connections.push_back(socket);

    // Connect all necessary events to new socket connection
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));

    // Write all current clients online to the new client
    for(auto it = this->connections.begin(); it != this->connections.end(); it++)
    {
        QByteArray currentUser;
        currentUser.append(QString("!"));
        currentUser.append((*it)->objectName());

        socket->write(currentUser);
    }
}


void tcpServer::readMessage()
{
    // On call, Qt gives access to the sender through the following method.
    // See docs at http://doc.qt.io/qt-5/qobject.html#sender
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());

    QByteArray buffer;
    QByteArray message;
    quint64 maxSize = Q_INT64_C(32);
    while(true)
    {
        buffer = socket->read(maxSize);

        if(buffer.isEmpty())
        {
            break;
        }

        message.append(buffer);
    }

    QString qStringMessage = QString(message);
    qDebug() << "Message read: " << qStringMessage;

    if(this->isUserName(qStringMessage))
    {
        QString pureUserName = qStringMessage.remove(0,1);

        // Alter the object name of the sender socket
        socket->setObjectName(pureUserName);

        // Called immediately on connection to server, so updates before any messages sent by client
        // FIXME: Try sending message immediately upon logging in. The event loop should prevent bugged performance (ie sending
        // a message before userlist updated)
        this->updateUserList(pureUserName);
    }
    else
    {
        // Send to message queue to enable sending to all clients
        this->messageQueue.push(message);

        // Emit event signal for server
        emit newMessage();
    }
}


bool tcpServer::isUserName(QString message)
{
    int index = message.indexOf("!");
    if(index != 0)
    {
        return false;
    }

    return true;
}


void tcpServer::deliverMessage()
{
    QByteArray message;
    bool sentFlag = false;

    if(!this->messageQueue.empty())
    {
        message = this->messageQueue.front();
        this->messageQueue.pop();

        for(auto it = this->connections.begin(); it != this->connections.end(); it++)
        {
            sentFlag = true;
            (*it)->write(message);
        }

        if(this->debug == true && sentFlag == true)
        {
            qDebug() << "Delivered message";
        }
    }
    else if(this->debug == true)
    {
        qDebug() << QString("No messages available to write.");
    }
}


void tcpServer::onDisconnect()
{
    // FIXME: Add new connection. Emit signal to socket that disconnect has occured, so we don't have to do string based stuff
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(QObject::sender());

    // Notify client to delete from userlist
    for(auto it = this->connections.begin(); it != this->connections.end(); it++)
    {
        QByteArray userToDelete;
        userToDelete.append(QString("-!"));
        userToDelete.append(socket->objectName());
        socket->write(userToDelete);

        (*it)->write(userToDelete);
    }

    // Delete connection from connection vector
    for(int i = 0; i < this->connections.size(); i++)
    {
        if(this->connections[i] == socket)
        {
            this->connections.erase(this->connections.begin() + i);
            qDebug() << "Socket disconnected";
        }
    }
}


void tcpServer::updateUserList(QString socketName)
{
    // Write the name of the newly connected user to each client. NOTE: must append twice, ByteArray will mess up ordering on QString concat
    QByteArray name;
    name.append("!");
    name.append(socketName);
    for(auto it = this->connections.begin(); it != this->connections.end(); it++)
    {
        (*it)->write(name);
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
