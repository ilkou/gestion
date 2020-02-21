#ifndef EMPLOYE_H
#define EMPLOYE_H

#include "includes.h"
#include "dbmanager.h"

class Employe : public QWidget
{
    Q_OBJECT
private slots:
    void edit_employe();
    void ajouter_client();
    void ajouter_voiture();
    void modifier_supprimer_client(int row, int column);
    void modifier_supprimer_employe(int,int);
    void ajouter_employe();
    void accepter_refuser_reserv(int row, int column);
public:
    Employe(QWidget *parent, QString user, QString p);
    void accueil_widget(QWidget *page);
    void profil_widget(QWidget *page);
    void notif_widget(QWidget *page);
    void voitures_widget(QWidget *page);
    void clients_widget(QWidget *page);
    void employes_widget(QWidget *page);
    void set_db(DbManager *d){
        Db  = d;
    }
    QString get_user(){
        return user;
    }
private:
    QString path;
    QTableWidget *tableWidget_employe;
    QTableWidget *tableWidget_client;
    QTableWidget *tableWidget_voiture;
    QTableWidget *tableWidget_notif;
    DbManager   *Db;
    QString     user;
};
#endif // EMPLOYE_H
