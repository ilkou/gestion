#ifndef ESPACEEMPLOYE_H
#define ESPACEEMPLOYE_H

#include "includes.h"
#include "dbmanager.h"

class EspaceEmploye : public QWidget
{
    Q_OBJECT
private slots:
    void se_connecter();
    void close_me();
public slots:
    void accueil();
public:
    EspaceEmploye(QWidget *parent, QString p);
    ~EspaceEmploye();
private:
    QWidget     *parent;
    DbManager   *Db;
    QString     path;
    QPushButton *conn;
    QLineEdit   *Login;
    QLineEdit   *Psswd;
    QPushButton *Exit;
    QPushButton *Back;
};

#endif // ESPACEEMPLOYE_H
