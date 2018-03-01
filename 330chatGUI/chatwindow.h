#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

class QTextEdit;
class QPushButton;

class ChatWindow : public QWidget {

    Q_OBJECT

public:
    ChatWindow(QWidget *parent = 0);

public slots:

private slots:

private:
    QTextEdit *chatMessages;
    QTextEdit *messageEdit;
    QPushButton *emojiButton;
    QPushButton *userListButton;
    QPushButton *sendButton;
};

#endif
