#include <QtWidgets>

#include "chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent){


    // Chat messages
    chatMessages = new QTextEdit();
    chatMessages -> setReadOnly(true);
    chatMessages -> setPlaceholderText("monkaS");


    // Message box
    messageEdit = new QTextEdit();
    messageEdit -> setPlaceholderText("Send a Message:");
    messageEdit -> setAlignment(Qt::AlignLeft);

    // Emoji button
    emojiButton = new QPushButton();
    emojiButton -> setText("Emoji");

    // Send button
    sendButton = new QPushButton();
    sendButton -> setText("Chat");

    // User List button
    userListButton = new QPushButton();
    userListButton -> setText("Users");



    QGridLayout *mainLayout = new QGridLayout;
    mainLayout -> addWidget(chatMessages, 0, 0, 1, 6);
    mainLayout -> addWidget(messageEdit, 1, 0, 2, 6);
    mainLayout -> addWidget(emojiButton, 3, 0, 1, 1);
    mainLayout -> addWidget(userListButton, 3, 1, 1, 1);
    mainLayout -> addWidget(sendButton, 3, 2, 1, 4);
    setWindowTitle("330Chat");
    setLayout(mainLayout);

}
