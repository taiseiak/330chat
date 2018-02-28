#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include "ui_chatwindow.h"

class ChatWindow : public QDialog, private Ui::ChatWindow
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = 0): QDialog(parent) {};

public slots:

private slots:

private:
    QString myNickName;
    QTextTableFormat tableFormat;
};

#endif
