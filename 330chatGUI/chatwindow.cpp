#include <QtWidgets>
#include <QtGui>
#include <QTcpSocket>
#include "chatwindow.h"
#include <QtDebug>
#include <QHostAddress>
#include <string>
#include <QRandomGenerator>


ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent){


    QStringList envVariables;
    envVariables << "USERNAME" << "USER" << "USERDOMAIN" << "HOSTNAME" << "DOMAINNAME";

    QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    foreach (QString string, envVariables) {
        if (environment.contains(string)) {
            username = environment.value(string).toUtf8();
            break;
        }
    }

    if (username.isEmpty()) {
        username = "unknown";
    }

    userColor = new QColor(QRandomGenerator::global()->bounded(256),
                           QRandomGenerator::global()->bounded(256),
                           QRandomGenerator::global()->bounded(256));

    // Chat messages
    chatMessages = new QTextEdit();
    chatMessages -> setReadOnly(true);
    chatMessages -> setFocusPolicy(Qt::NoFocus);


    // Message box
    messageEdit = new QLineEdit();
    messageEdit -> setPlaceholderText("Send a Message:");
    messageEdit -> setAlignment(Qt::AlignLeft);
    connect(messageEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));

    // Send button
    sendButton = new QPushButton();
    sendButton -> setText("Chat");
    connect(sendButton, SIGNAL(released()), this, SLOT(returnPressed()));

    // User List button
    userListButton = new QPushButton();
    userListButton -> setText("Users");
    connect(userListButton, SIGNAL(released()), this, SLOT(toggleUserLog()));

    // UserLog
    userlog = new QListWidget();
    userlog->setFocusPolicy(Qt::NoFocus);

    tableFormat = new QTextTableFormat();
    tableFormat->setBorder(0); // removes boarder from each text
    tableFormat->setAlignment(Qt::AlignVCenter);

    emoteMap = new QMap<QString, QString>();
    QFile emoteFile(":/images/emotes/emoteTable.txt");
    if (!emoteFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&emoteFile);
    while (!in.atEnd()) {
        QStringList line = in.readLine().split(" ", QString::SkipEmptyParts);
        emoteMap->insert(line[0], line[1]);
    }


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout -> addWidget(chatMessages, 1, 0, 1, -1);
    mainLayout -> addWidget(messageEdit, 2, 0, 1, -1);
    mainLayout -> addWidget(userListButton, 3, 0, 1, 1);
    mainLayout -> addWidget(sendButton, 3, 1, 1, 2);
    mainLayout -> addWidget(userlog, 0, 0, 1, 3);
    userlog->hide();
    setWindowTitle("330Chat");
    setLayout(mainLayout);
    setMinimumSize(400, 800);

    // Client-Server Init
    QHostAddress host = QHostAddress("::1");
    quint16 port = 8000;
    this->socket = new QTcpSocket();
    this->socket->connectToHost(host, port);

    connect(this->socket, SIGNAL(readyRead()), this, SLOT(readFromServer()));

    // Pass username through to server. Server will parse as username given prefix "!"
    QByteArray usernameMessage;
    usernameMessage.append("!");
    QString argString = QString("/item%1/item%2/item%3/item%4").arg(username).arg(userColor->red()).arg(userColor->green()).arg(userColor->blue());
    usernameMessage.append(argString);
    this->socket->write(usernameMessage);


    // EXAMPLE ADDING AND DELETING USERS
    // WILL NEED TO CONNECT IT UP TO SERVER
//    QString anon1 = QString("/itemANON/item255/item0/item0");
//    QString anon2 = QString("/itemANON2/item0/item255/item0");
//    appendUser(anon1);
//    appendUser(anon2);
//    QString anon2name = QString("ANON2");
//    deleteUser(anon2name);
}

void ChatWindow::returnPressed() {
    // EDIT THIS
    QString sendText = messageEdit->text();
    if (sendText.isEmpty()) {
        return;
    }
    QString finalSend = QString("/item%1/item%2/item%3/item%4/item%5").arg(username).arg(userColor->red()).arg(userColor->green()).arg(userColor->blue()).arg(sendText);

    QByteArray message;
    message.append(finalSend);
    this->socket->write(message);

    messageEdit -> clear();
}

void ChatWindow::appendUser(QString &user) {

    if (user.isEmpty()) {
        return;
    }
    QStringList line = user.split("/item", QString::SkipEmptyParts);
    QString userName = QString(line[0]);
    int userred = QString(line[1]).toInt();
    int usergreen = QString(line[2]).toInt();
    int userblue = QString(line[3]).toInt();
    QListWidgetItem *item = new QListWidgetItem(userName);
    item->setTextColor(QColor(userred, usergreen, userblue));
    userlog->addItem(item);
}


void ChatWindow::deleteUser(QString &user) {
    if (user.isEmpty()) {
        return;
    }

    QList<QListWidgetItem *> items = userlog->findItems(user, Qt::MatchExactly);
    if (items.isEmpty()) {
        return;
    }
    delete items.at(0);
}

void ChatWindow::readFromServer()
{
    QByteArray buffer;
    QByteArray message;
    quint64 maxSize = Q_INT64_C(32);

    while(true)
    {
        buffer = this->socket->read(maxSize);
        if(buffer.isEmpty())
        {
            break;
        }

        message.append(buffer);
    }

    // Check for add user commands and delete user commands
    QString stringMessage(message);
    if(this->isUserName(stringMessage))
    {
        QString pureUserName = stringMessage.remove(0,1);
        appendUser(pureUserName);
    }
    else if(this->isDisconnect(stringMessage))
    {
        QString argUser = stringMessage.remove(0,2);
        QStringList line = argUser.split("/item", QString::SkipEmptyParts);
        QString senderName = QString(line[0]);
        deleteUser(senderName);
    }
    else
    {
        QString finalMessage(message);
        appendMessage(finalMessage);
    }
}

void ChatWindow::toggleUserLog() {
    if (userlog->isVisible()) {
        userlog->hide();
    } else {
        userlog->show();
    }
}

void ChatWindow::appendMessage(QString &message) {
    if (message.isEmpty()) {
        return;
    }
    QStringList line = message.split("/item", QString::SkipEmptyParts);
    QString senderName = QString(line[0]);
    QString senderred = QString(line[1]);
    QString sendergreen = QString(line[2]);
    QString senderblue = QString(line[3]);
    QString senderMessage = QString(line[4]);


    QTextCursor cursor(chatMessages->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, *tableFormat);
    QString userFormatted = QString("<span style='font-size:20pt; vertical-align:middle;"
                              "padding-top: 10px; padding-bottom: 10px;"
                               "color:rgb(%1, %2, %3);'>%4</span>").arg(senderred).arg(sendergreen).arg(senderblue).arg(senderName);

    table->cellAt(0,0).firstCursorPosition().insertHtml(userFormatted);

    QMap<QString, QString>::const_iterator  i = emoteMap->constBegin();
    while (i != emoteMap->constEnd()) {
        senderMessage.replace(i.key(), QString("<img src=':/images/emotes/%1'>").arg(i.value()));
        i++;
    }
    senderMessage.insert(0, QString("<span style='font-size:20pt; vertical-align:middle;"
                              "padding-top: 10px; padding-bottom: 10px;'>"));
    senderMessage.append(QString("</span>"));

    table->cellAt(0, 1).firstCursorPosition().insertHtml(senderMessage);
    QScrollBar *bar = chatMessages->verticalScrollBar();
    bar->setValue(bar->maximum());
}

bool ChatWindow::isUserName(QString message)
{
    int index = message.indexOf("!");
    if(index != 0)
    {
        return false;
    }

    return true;
}

bool ChatWindow::isDisconnect(QString message)
{
    int indexDeletion = message.indexOf("-");
    int indexUser = message.indexOf("!");
    if(indexDeletion != 0 && indexUser != 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}
