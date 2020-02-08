#ifndef CLIENT_H
#define CLIENT_H

#include "includes.h"
#include "dbmanager.h"

class Client : public QMainWindow
{
    Q_OBJECT
private slots:
    void reserver_voiture();
    void edit_client();
    void recherche_client();
    void notification();
    void ex_reservation();
public:
    Client(QWidget *parent, QString u);
    QScrollArea *createCentralWidget(int n_cars);
    void createToolBars();
    void set_db(DbManager *d) {
        Db = d;
    }
private:
    QToolButton *ExRes;
    QToolButton *Notif;
    QLineEdit   *Recherche;
    QToolButton *Rech_Button;
    QToolButton *Button;
    DbManager   *Db;
    QString     user;
    int         n_cars;
    QPushButton *Edit;
    QToolButton *Back;
    QPushButton *Exit;
};
#endif // CLIENT_H
