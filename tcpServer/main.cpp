#include <QApplication>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostAddress>
#include <QString>
#include <QIODevice>

#include <iostream>
#include <stdlib.h>
#include <thread>
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

    /* Qt, instead of callbacks, uses signals and slots. Signals are the event,
     * while slots are the callback. Here we connect a signal (event) to the server
     * object, which will call echo on event
    */
    connect(this->server, SIGNAL(newConnection()), this, SLOT(echo()));

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


void tcpServer::echo()
{
    std::cout << "Connecting" << std::endl;
    qint64 maxSize = Q_INT64_C(32);
    char data[maxSize];
    QTcpSocket* socket = server->nextPendingConnection();

    socket->write("Hello");

    // TODO: Figure out how to allow echoing to work
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
