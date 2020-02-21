#ifndef ESPACECLIENT_H
#define ESPACECLIENT_H

#include "includes.h"
#include "dbmanager.h"

class EspaceClient : public QWidget
{
    Q_OBJECT
private slots:
    void    new_account();
    void se_connecter();
    void close_me();
public slots:
    void accueil();
public:
    EspaceClient(QWidget *parent, QString p);
    ~EspaceClient();
private:
    QWidget     *parent;
    DbManager   *Db;
    QString     path;
    QPushButton *conn;
    QLineEdit   *Login;
    QLineEdit   *Psswd;
    QPushButton *Exit;
    QPushButton *New;
    QPushButton *Back;
};

#endif // ESPACECLIENT_H
