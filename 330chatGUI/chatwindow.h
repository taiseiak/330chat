#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>

class QTextEdit;
class QPushButton;
class QTextTableFormat;
class QLineEdit;

class ChatWindow : public QWidget {

    Q_OBJECT

public:
    ChatWindow(QWidget *parent = 0);

public slots:
    void appendMessage(const QString &message);

private slots:
    void returnPressed();

private:
    QTextEdit *chatMessages;
    QLineEdit *messageEdit;
    QPushButton *emojiButton;
    QPushButton *userListButton;
    QPushButton *sendButton;
     QTextTableFormat *tableFormat;
};

#endif
