#ifndef NEWACCOUNT_H
#define NEWACCOUNT_H

#include "includes.h"
#include "dbmanager.h"

class NewAccount : public QDialog
{
    Q_OBJECT
private slots:
    void save_data();
public:
    NewAccount(QWidget *parent);
    void    set_path(QString p);
private:
    QString   path;
    QLineEdit *Cin;
    QLineEdit *Nom;
    QLineEdit *Prenom;
    QLineEdit *Tel;
    QLineEdit *Adresse;
    QLineEdit *N_permis;
    QDateEdit *Date;
    QLineEdit *Login;
    QLineEdit *psswd;
    QPushButton *save;
    QPushButton *exit;
};

#endif // NEWACCOUNT_H
