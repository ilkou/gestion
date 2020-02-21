#ifndef NEWCAR_H
#define NEWCAR_H


#include "includes.h"
#include "dbmanager.h"

class NewCar : public QDialog
{
    Q_OBJECT
private slots:
    void save_data();
    void addPic();
public:
    NewCar(QWidget *parent);
    void    set_path(QString p);
private:
    QString   path;
    QLineEdit *Matricule;
    QLineEdit *Prix;
    QLineEdit *Color;
    QPushButton *Photo;
    QLineEdit *N_passagers;
    QLineEdit *RefContrat;
    QLineEdit *Modele;
    QLineEdit *Categorie;
    QLineEdit *Marque;
    QString PicPath;
    QPushButton *save;
    QPushButton *exit;
};

#endif // NEWCAR_H
