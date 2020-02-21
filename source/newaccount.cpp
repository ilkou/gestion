#include <newaccount.h>
#include <dbmanager.h>

NewAccount::NewAccount(QWidget *parent) : QDialog(parent)
{
    Cin = new QLineEdit;
    Nom = new QLineEdit;
    Prenom = new QLineEdit;
    Tel = new QLineEdit;
    Adresse = new QLineEdit;
    N_permis = new QLineEdit;
    Date = new QDateEdit;
    Login = new QLineEdit;
    psswd = new QLineEdit;
    psswd->setEchoMode(QLineEdit::Password);
    save = new QPushButton("Enregistrer");
    exit = new QPushButton("Quitter");

    Date->setDate(QDate::currentDate());
    Date->setTime(QTime::currentTime());
    QFormLayout *page_layout = new QFormLayout;
    page_layout->addRow("Identifiant", Login);
    page_layout->addRow("Mot de passe", psswd);
    page_layout->addRow("CIN", Cin);
    page_layout->addRow("Nom", Nom);
    page_layout->addRow("Prénom", Prenom);
    page_layout->addRow("Numéro de mobile", Tel);
    page_layout->addRow("Adresse", Adresse);
    page_layout->addRow("Numéro de permis", N_permis);
    page_layout->addRow("Date d'obtention du permis", Date);

    /*QPixmap bkgnd("/Users/oouklich/Desktop/QTT/icons/login_bd_ex.jpg");
    bkgnd = bkgnd.scaled(QSize(WIDTH/2,1200/2), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    setPalette(palette);*/

    QGroupBox   *page_group = new QGroupBox("Création d'un nouveau compte client");
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
void    NewAccount::set_path(QString p)
{
    path = p;
}
void    NewAccount::save_data()
{
    DbManager Db(this->path);
    if (Db.addClient(Cin->text(), Nom->text(), Prenom->text(), Tel->text().toInt(), Adresse->text(), N_permis->text().toInt(), Date->date(), Login->text(), psswd->text()))
        QMessageBox::information(this, "Enregistrement réussi", "<strong>Enregistrement a été effectué avec succès !</strong>");
    else
        QMessageBox::critical(this, "Erreur enregistrement échoué", "votre enregistrement a échoué !");
    this->close();
}
