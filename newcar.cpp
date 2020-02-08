#include "newcar.h"
#include "dbmanager.h"

NewCar::NewCar(QWidget *parent) : QDialog(parent)
{
    Matricule = new QLineEdit;
    Prix = new QLineEdit;
    Color = new QLineEdit;
    Photo = new QPushButton("Ajouter une photo");
    N_passagers = new QLineEdit;
    RefContrat = new QLineEdit;
    Modele = new QLineEdit;
    Marque = new QLineEdit;
    Categorie = new QLineEdit;
    save = new QPushButton("Enregistrer");
    exit = new QPushButton("Quitter");
    QFormLayout *page_layout = new QFormLayout;
    page_layout->addRow("Matricule", Matricule);
    page_layout->addRow("Prix", Prix);
    page_layout->addRow("Couleur", Color);
    page_layout->addRow("nombre de passager", N_passagers);
    page_layout->addRow("Référence de contrat d'assurance", RefContrat);
    page_layout->addRow("Marque", Marque);
    page_layout->addRow("modèle", Modele);
    page_layout->addRow("catégorie", Categorie);

    QGroupBox   *page_group = new QGroupBox("Addition d'une voiture");
    page_group->setLayout(page_layout);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addWidget(page_group);
    layoutPrincipal->addWidget(Photo);
    layoutPrincipal->addWidget(save);
    layoutPrincipal->addWidget(exit);
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
    setLayout(layoutPrincipal);
    QObject::connect(Photo, SIGNAL(clicked()), this, SLOT(addPic()));
    QObject::connect(exit, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(save, SIGNAL(clicked()), this, SLOT(save_data()));
}
void    NewCar::addPic()
{
    DbManager Db(this->path);
    PicPath = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.gif *.jpg *.jpeg)");
    if (QFile::copy(PicPath, QCoreApplication::applicationDirPath() + "/car" + QString::number(Db.get_n_cars() + 1) + ".jpg"))
        qDebug() << "image a ete copie avec succes";
    else {
        qDebug() << "Echec : copie de l'image";
    }
}
void    NewCar::set_path(QString p)
{
    path = p;
}
void    NewCar::save_data()
{
    DbManager Db(this->path);
    if (Db.addCar(Matricule->text(), Prix->text().toFloat(), Color->text(), "car" + QString::number(Db.get_n_cars() + 1) , N_passagers->text().toInt(), RefContrat->text().toInt(), Marque->text(), Categorie->text(), Modele->text()))
        QMessageBox::information(this, "Enregistrement réussi", "<strong>Enregistrement a été effectué avec succès !</strong>");
    else
        QMessageBox::critical(this, "Erreur enregistrement échoué", "votre enregistrement a échoué !");
    this->close();
}
