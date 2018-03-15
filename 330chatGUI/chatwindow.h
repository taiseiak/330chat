#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QListWidget>

class QTextEdit;
class QPushButton;
class QTextTableFormat;
class QLineEdit;
class QWindow;
class userLog;
class QByteArray;
class QColor;
class QListWidget;

class ChatWindow : public QWidget {

    Q_OBJECT

public:
    ChatWindow(QWidget *parent = 0);

    QTcpSocket* socket;
    bool isUserName(QString message);
    bool isDisconnect(QString message);

public slots:
    void appendMessage(QString &message);
    void readFromServer();
    void appendUser(QString &user);
    void deleteUser(QString &user);

private slots:
    void returnPressed();
    void toggleUserLog();

private:
    QTextEdit *chatMessages;
    QLineEdit *messageEdit;
    QPushButton *emojiButton;
    QPushButton *userListButton;
    QPushButton *sendButton;
    QTextTableFormat *tableFormat;
    QMap<QString, QString> *emoteMap;
    QListWidget *userlog;
    QColor *userColor;
    QString username;
};

#endif

