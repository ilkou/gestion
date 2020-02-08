#include "editaccount.h"
#include "dbmanager.h"

EditAccount::EditAccount(QWidget *parent, QString log, QString t, int a) : QDialog(parent)
{
    type = t;
    this->a = a;
    login = log;
    Cin = new QLineEdit;
    Nom = new QLineEdit;
    Prenom = new QLineEdit;
    Tel = new QLineEdit;
    Adresse = new QLineEdit;
    psswd = new QLineEdit;
    psswd->setEchoMode(QLineEdit::Password);
    Fidelite = new QLineEdit;
    Cadre = new QLineEdit;
    save = new QPushButton("Enregistrer");
    exit = new QPushButton("Quitter");

    Cin->setToolTip(Db->get_Cin(login, type));
    Nom->setToolTip(Db->get_nom(login, type));
    Prenom->setToolTip(Db->get_prenom(login, type));
    Adresse->setToolTip(Db->get_Adresse(login, type));
    Tel->setToolTip(QString::number(Db->get_Tel(login, type)));
    QFormLayout *page_layout = new QFormLayout;
    page_layout->addRow("Nouveau CIN", Cin);
    page_layout->addRow("Nouveau Nom", Nom);
    page_layout->addRow("Nouveau Prénom", Prenom);
    page_layout->addRow("Nouveau Mot de passe", psswd);
    page_layout->addRow("Nouveau Numéro de mobile", Tel);
    page_layout->addRow("Nouveau Adresse", Adresse);
    if (a && t == "client")
        page_layout->addRow("Fidelete (entier)", Fidelite);
    if (t == "employe")
        page_layout->addRow("Cadre (entier)", Cadre);
    QGroupBox   *page_group = new QGroupBox("Modifier votre compte");
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

void    EditAccount::save_data()
{
    if (a == 0 && Db->editProfil(Cin->text().isEmpty() ? Db->get_Cin(login, type) : Cin->text(), Nom->text().isEmpty() ? Db->get_nom(login, type) : Nom->text(), Prenom->text().isEmpty() ? Db->get_prenom(login, type) : Prenom->text(), Tel->text().isEmpty() ? Db->get_Tel(login, type) : Tel->text().toInt(), Adresse->text().isEmpty() ? Db->get_Adresse(login, type) : Adresse->text(), login, psswd->text().isEmpty() ? Db->get_psswd(login, type) : psswd->text(), type))
        QMessageBox::information(this, "Enregistrement réussi", "<strong>Enregistrement a été effectué avec succès !</strong>");
    else if (type == "employe" && Db->editProfil(Cin->text().isEmpty() ? Db->get_Cin(login, type) : Cin->text(), Nom->text().isEmpty() ? Db->get_nom(login, type) : Nom->text(), Prenom->text().isEmpty() ? Db->get_prenom(login, type) : Prenom->text(), Tel->text().isEmpty() ? Db->get_Tel(login, type) : Tel->text().toInt(), Adresse->text().isEmpty() ? Db->get_Adresse(login, type) : Adresse->text(), login, psswd->text().isEmpty() ? Db->get_psswd(login, type) : psswd->text(), Cadre->text().toInt()))
        QMessageBox::information(this, "Enregistrement réussi", "<strong>Enregistrement a été effectué avec succès !</strong>");
    else if (type == "client" && Db->editProfil(Cin->text().isEmpty() ? Db->get_Cin(login, type) : Cin->text(), Nom->text().isEmpty() ? Db->get_nom(login, type) : Nom->text(), Prenom->text().isEmpty() ? Db->get_prenom(login, type) : Prenom->text(), Tel->text().isEmpty() ? Db->get_Tel(login, type) : Tel->text().toInt(), Adresse->text().isEmpty() ? Db->get_Adresse(login, type) : Adresse->text(),Fidelite->text().toInt(), login, psswd->text().isEmpty() ? Db->get_psswd(login, type) : psswd->text()))
        QMessageBox::information(this, "Enregistrement réussi", "<strong>Enregistrement a été effectué avec succès !</strong>");
    else
        QMessageBox::critical(this, "Erreur enregistrement échoué", "votre enregistrement a échoué !");
    this->close();
}
