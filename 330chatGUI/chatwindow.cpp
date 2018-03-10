#include <QtWidgets>

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

    // Emoji button
    emojiButton = new QPushButton();
    emojiButton -> setText("Emoji");

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

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout -> addWidget(chatMessages, 0, 0, 1, 6);
    mainLayout -> addWidget(messageEdit, 1, 0, 2, 6);
    mainLayout -> addWidget(emojiButton, 3, 0, 1, 1);
    mainLayout -> addWidget(userListButton, 3, 1, 1, 1);
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

void ChatWindow::appendMessage(QString &message) {
    if (message.isEmpty()) {
        return;
    }
    QTextCursor cursor(chatMessages->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 1, *tableFormat);
//table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    message.replace(QString("FeelsAmazingMan"), QString("<img src=':/images/emotes/FeelsAmazingMan.png'>"));
    message.replace(QString("D:"), QString("<img src=':/images/emotes/DColon.png'>"));
    message.replace(QString("FeelsGoodMan"), QString("<img src=':/images/emotes/FeelsGoodMan.png'>"));
    message.replace(QString("haHAA"), QString("<img src=':/images/emotes/haHAA.png'>"));
    table->cellAt(0, 0).firstCursorPosition().insertHtml(message);
    QScrollBar *bar = chatMessages->verticalScrollBar();
    bar->setValue(bar->maximum());
}
