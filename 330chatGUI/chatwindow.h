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
<<<<<<< HEAD
    QString username;
=======
    bool isUserName(QString message);
    bool isDisconnect(QString message);
>>>>>>> 02cb4fd1c76abda3e53e7a006bd50dfc65e5a5a9

public slots:
    void appendMessage(QString &message);
    void readFromServer();
<<<<<<< HEAD
    void sendFile();
=======
    void appendUser(QString &user);
    void deleteUser(QString &user);

>>>>>>> 02cb4fd1c76abda3e53e7a006bd50dfc65e5a5a9
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

