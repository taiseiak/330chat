#include <QtWidgets>

#include "chatwindow.h"
#include <QtDebug>

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent){


    // Chat messages
    chatMessages = new QTextEdit();
    chatMessages -> setReadOnly(true);
    chatMessages -> setPlaceholderText("monkaS");
    chatMessages -> setFocusPolicy(Qt::NoFocus);


    // Message box
    messageEdit = new QLineEdit();
    messageEdit -> setPlaceholderText("Send a Message:");
    messageEdit -> setAlignment(Qt::AlignLeft);
    connect(messageEdit, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    messageEdit -> setFocusPolicy(Qt::StrongFocus);

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

void ChatWindow::appendMessage(const QString &message) {
    if (message.isEmpty()) {
        return;
    }
    QTextCursor cursor(chatMessages->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 1, *tableFormat);
//table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 0).firstCursorPosition().insertText(message);
    QScrollBar *bar = chatMessages->verticalScrollBar();
    bar->setValue(bar->maximum());

}
