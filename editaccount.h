#ifndef EDITACCOUNT_H
#define EDITACCOUNT_H

#include "includes.h"
#include "dbmanager.h"
#include "dbmanager.h"
class EditAccount : public QDialog
{
    Q_OBJECT
private slots:
    void save_data();
public:
    EditAccount(QWidget *parent, QString login, QString t, int a);
    void    set_db(DbManager *d)
    {
        Db = d;
    }
private:
    QString   type;
    int a;
    DbManager *Db;
    QString   login;
    QLineEdit *Cin;
    QLineEdit *Nom;
    QLineEdit *Prenom;
    QLineEdit *Tel;
    QLineEdit *Adresse;
    QLineEdit *psswd;
    QLineEdit *Cadre;
    QLineEdit *Fidelite;
    QPushButton *save;
    QPushButton *exit;
};

#endif // EDITACCOUNT_H
