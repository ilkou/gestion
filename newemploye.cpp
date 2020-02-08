#include "newemploye.h"
#include "dbmanager.h"

NewEmploye::NewEmploye(QWidget *parent) : QDialog(parent)
{
    Cin = new QLineEdit;
    Nom = new QLineEdit;
    Prenom = new QLineEdit;
    Tel = new QLineEdit;
    Adresse = new QLineEdit;
    Cadre = new QCheckBox("il s'agit d'un administrateur");
    Login = new QLineEdit;
    psswd = new QLineEdit;
    psswd->setEchoMode(QLineEdit::Password);
    save = new QPushButton("Enregistrer");
    exit = new QPushButton("Quitter");
    Cadre->setChecked(false);

    QFormLayout *page_layout = new QFormLayout;
    page_layout->addRow("Identifiant", Login);
    page_layout->addRow("Mot de passe", psswd);
    page_layout->addRow("CIN", Cin);
    page_layout->addRow("Nom", Nom);
    page_layout->addRow("Prénom", Prenom);
    page_layout->addRow("Numéro de mobile", Tel);
    page_layout->addRow("Adresse", Adresse);
    page_layout->addRow(Cadre);

    QGroupBox   *page_group = new QGroupBox("Création d'un nouveau compte employe");
    page_group->setLayout(page_layout);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addWidget(page_group);
    layoutPrincipal->addWidget(save);
    layoutPrincipal->addWidget(exit);
    resize(450, 300);
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
    setLayout(layoutPrincipal);
    QObject::connect(exit, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(save, SIGNAL(clicked()), this, SLOT(save_data()));
}
void    NewEmploye::set_path(QString p)
{
    path = p;
}
void    NewEmploye::save_data()
{
    DbManager Db(this->path);
    if (Db.addEmploye(Cin->text(), Nom->text(), Prenom->text(), Tel->text().toInt(), Adresse->text(), Login->text(), psswd->text(), Cadre->isChecked() ? 1 : 0))
        QMessageBox::information(this, "Enregistrement réussi", "<strong>Enregistrement a été effectué avec succès !</strong>");
    else
        QMessageBox::critical(this, "Erreur enregistrement échoué", "votre enregistrement a échoué !");
    this->close();
}
