#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>

class QTextEdit;
class QPushButton;
class QTextTableFormat;
class QLineEdit;
class QWindow;

class ChatWindow : public QWidget {

    Q_OBJECT

public:
    ChatWindow(QWidget *parent = 0);

    QTcpSocket* socket;

public slots:
    void appendMessage(QString &message);
    void readFromServer();

private slots:
    void returnPressed();

private:
    QTextEdit *chatMessages;
    QLineEdit *messageEdit;
    QPushButton *emojiButton;
    QPushButton *userListButton;
    QPushButton *sendButton;
    QTextTableFormat *tableFormat;
    QMap<QString, QString> *emoteMap;
};

#endif
