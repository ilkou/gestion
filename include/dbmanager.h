#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "includes.h"

class DbManager
{
public:
    DbManager(const QString& path);
    void close_db();
    QString get_primary_key(int row, QString type, QString value);
    bool addClient(QString CIN, QString nom, QString prenom, int tel, QString adresse, int n_permis, QDate date_permis, QString login, QString psswd);
    bool addEmploye(QString CIN, QString nom, QString prenom, int tel, QString adresse, QString login, QString psswd, int cadre);
    bool addCar(QString matricule, float prix, QString couleur, QString photo, int nbr_passagers, int ref_contrat_ass, QString nom_marque, QString nom_categ, QString nom_modele);
    bool editProfil(QString CIN, QString nom, QString prenom, int tel, QString adresse, QString login, QString psswd, QString type);
    bool editProfil(QString CIN, QString nom, QString prenom, int tel, QString adresse,QString login, QString psswd, int cadre);
    bool editProfil(QString CIN, QString nom, QString prenom, int tel, QString adresse,int fidelite, QString login, QString psswd);
    bool connect(QString login, QString psswd, QString type);
    bool reserver(int nbr_jours, QDate date_livraison, QString lieu, QString cin, float prix, QString matricule);
    bool is_admin(QString user);
    bool is_exist(QString to_find);
    bool is_reserved(QString Matricule);
    bool delete_account(QString login, QString type);
    float get_price(QString matricule);
    int get_n_marque();
    int get_n_client();
    int get_n_employe();
    int get_n_voiture();
    int get_n_fidele();
    int get_n_cars();
    int get_ref_reserv();
    int get_loyalty(QString login);
    int get_Tel(QString login, QString type);
    QString get_categorie(QString id_marque);
    QString get_marque(QString id_marque);
    QString get_modele(QString id_marque);
    QString get_nom(QString login, QString type);
    QString get_prenom(QString login, QString type);
    QString get_Cin(QString login, QString type);
    QString get_Adresse(QString login, QString type);
    QString get_psswd(QString login, QString type);
    QString get_recu(int ref);

private:
    QSqlDatabase *my_db;
};

#endif // DBMANAGER_H
