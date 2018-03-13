#include <QtWidgets>
#include <QtGui>
#include "chatwindow.h"
#include <QtDebug>
#include <string>

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent){


    // Chat messages
    chatMessages = new QTextEdit();
    chatMessages -> setReadOnly(true);
    chatMessages -> setFocusPolicy(Qt::NoFocus);


    // Message box
    messageEdit = new QLineEdit();
    messageEdit -> setPlaceholderText("Send a Message:");
    messageEdit -> setAlignment(Qt::AlignLeft);
    connect(messageEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));

//    // Emoji button
//    emojiButton = new QPushButton();
//    emojiButton -> setText("Emoji");

    // Send button
    sendButton = new QPushButton();
    sendButton -> setText("Chat");
    connect(sendButton, SIGNAL(released()), this, SLOT(returnPressed()));

    // User List button
    userListButton = new QPushButton();
    userListButton -> setText("Users");

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
    mainLayout -> addWidget(chatMessages, 0, 0, 1, 6);
    mainLayout -> addWidget(messageEdit, 1, 0, 2, 6);
//    mainLayout -> addWidget(emojiButton, 3, 0, 1, 1);
//    mainLayout -> addWidget(userListButton, 3, 1, 1, 1);
    mainLayout -> addWidget(sendButton, 3, 2, 1, 4);
    setWindowTitle("330Chat");
    setLayout(mainLayout);

}

void ChatWindow::returnPressed() {
    QString sendText = messageEdit->text();
    if (sendText.isEmpty()) {
        return;
    }

    appendMessage(sendText);
    messageEdit -> clear();
}

//void ChatWindow::addEmoticon(QString &emoticon) {
//    messageEdit->setText(messageEdit->text() + emoticon);
//}

void ChatWindow::appendMessage(QString &message) {
    if (message.isEmpty()) {
        return;
    }
    QTextCursor cursor(chatMessages->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 1, *tableFormat);

    QMap<QString, QString>::const_iterator  i = emoteMap->constBegin();
    while (i != emoteMap->constEnd()) {
        message.replace(i.key(), QString("<img src=':/images/emotes/%1'>").arg(i.value()));
        i++;
    }
    message.insert(0, QString("<span style='font-size:20pt; vertical-align:bottom;"
                              "padding-top: 10px; padding-bottom: 10px;'>"));
    message.append(QString("</span>"));

    table->cellAt(0, 0).firstCursorPosition().insertHtml(message);
    QScrollBar *bar = chatMessages->verticalScrollBar();
    bar->setValue(bar->maximum());
}
