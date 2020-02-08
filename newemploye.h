#ifndef NEWEMPLOYE_H
#define NEWEMPLOYE_H

#include "includes.h"
#include "dbmanager.h"

class NewEmploye : public QDialog
{
    Q_OBJECT
private slots:
    void save_data();
public:
    NewEmploye(QWidget *parent);
    void    set_path(QString p);
private:
    QString   path;
    QLineEdit *Cin;
    QLineEdit *Nom;
    QLineEdit *Prenom;
    QLineEdit *Tel;
    QLineEdit *Adresse;
    QLineEdit *Login;
    QLineEdit *psswd;
    QCheckBox *Cadre;
    QPushButton *save;
    QPushButton *exit;
};

#endif // NEWEMPLOYE_H
