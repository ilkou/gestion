#ifndef RESERVER_H
#define RESERVER_H

#include "includes.h"
#include "dbmanager.h"

class Reserver : public QDialog
{
    Q_OBJECT
private slots:
    void slot_reserver();
    public:
    Reserver(QWidget *parent, QString Matricule);
    void set_db(DbManager *d){
        Db = d;
    }
    void set_user(QString u){
        user = u;
    }
private:
    QString     user;
    DbManager   *Db;
    QString     matricule;
    QTextEdit   *propriete;
    QPushButton *OpenImage;
    QPushButton *Res;
    QPushButton *Exit;
    QPushButton *Deja;
    QLineEdit   *nbr_jours;
    QDateEdit   *date_livraison;
    QLineEdit   *Lieu_prise_en_charge;
};
#endif // RESERVER_H
