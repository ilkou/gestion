#include <reserver.h>
#include <dbmanager.h>

Reserver::Reserver(QWidget *parent, QString Matricule) : QDialog (parent)
{
    matricule = Matricule;
    OpenImage = new QPushButton("Voir l'image");
    Res  = new QPushButton("Réserver");
    Exit      = new QPushButton("Quitter");
    nbr_jours = new QLineEdit;
    date_livraison = new QDateEdit;
    Lieu_prise_en_charge = new QLineEdit;
    Deja = new QPushButton("Déja réservé");

    Deja->setFont(QFont("Gill Sans", 24));
    Deja->setMaximumWidth(400);
    Deja->setStyleSheet(" border: 1px solid gray;background: transparent;");

    nbr_jours->setFont(QFont("Gill Sans", 24));
    nbr_jours->setStyleSheet("border: 1px solid gray;background: transparent; font-size: 20");
    date_livraison->setDate(QDate::currentDate());
    Lieu_prise_en_charge->setFont(QFont("Gill Sans", 24));
    Lieu_prise_en_charge->setStyleSheet("border: 1px solid gray;text-align: center; background: transparent; font-size: 20");

    QFormLayout *layout_reserver = new QFormLayout;
    layout_reserver->addRow("Nombre de jours", nbr_jours);
    layout_reserver->addRow("Lieu de prise en charge", Lieu_prise_en_charge);
    layout_reserver->addRow("Date de livraison", date_livraison);

    QGroupBox *group = new QGroupBox("Formulaire de réservation");
    group->setLayout(layout_reserver);

    Res->setStyleSheet("color: #FFFFFF;background: gray;border-radius: 4px;");
    Res->setCursor(Qt::PointingHandCursor);
    Res->setFont(QFont("Gill Sans", 16));
    Res->setCursor(Qt::PointingHandCursor);
    Exit->setStyleSheet("color: #FFFFFF;background: gray;border-radius: 4px;");
    Exit->setCursor(Qt::PointingHandCursor);
    Exit->setFont(QFont("Gill Sans", 16));
    Exit->setCursor(Qt::PointingHandCursor);
    QHBoxLayout *layout_down = new QHBoxLayout;
    layout_down->addWidget(Exit);
    if (Db->is_reserved(matricule) == false)
        layout_down->addWidget(Res);
    QString Text;
    QSqlQueryModel model, model0, model1, model2, model3;
    model.setQuery("select nom_modele from modele where id_modele in(select id_modele from avoir where id_marque in(select id_marque from voiture where matricule='"+Matricule+"'))");
    model0.setQuery("select nom_marque from marque where id_marque in (select id_marque from voiture where matricule='"+Matricule+"')");
    model1.setQuery("select nom_categ from categorie where id_categ in(select id_categ from avoir_b where id_marque in(select id_marque from voiture where matricule='"+Matricule+"'));");
    model2.setQuery("select * from voiture where matricule='"+Matricule+"'");
    Text += "Marque      : " + model0.record(0).value(0).toString();
    Text += "\nCatégorie    : " + model1.record(0).value(0).toString();
    Text += "\nModèle       : " + model.record(0).value(0).toString();
    Text += "\nPrix par jour : " + QString::number(model2.record(0).value("prix").toFloat()) + "Dh";
    Text += "\nCouleur      : " + model2.record(0).value("couleur").toString();
    Text += "\nNombre des passagers : " + QString::number(model2.record(0).value("nbr_passagers").toInt());
    propriete = new QTextEdit();
    propriete->setPlainText(Text);
    propriete->setReadOnly(true);
    propriete->setFont(QFont("sans-serif"));
    propriete->setLineWrapMode(QTextEdit::NoWrap);
    propriete->setFixedSize(400,110);


    model3.setQuery("select photo from voiture where matricule='"+Matricule+"'");
    QDialog *secondeFenetre =  new QDialog;
    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *image = new QLabel(secondeFenetre);
    image->setPixmap(QPixmap(QCoreApplication::applicationDirPath() + "/" + model3.record(0).value(0).toString() + ".jpg"));
    layout->addWidget(image);
    secondeFenetre->setLayout(layout);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    Db->is_reserved(matricule) == true ? layoutPrincipal->addWidget(Deja) : layoutPrincipal->addWidget(group);
    layoutPrincipal->addWidget(propriete);
    layoutPrincipal->addWidget(OpenImage);
    layoutPrincipal->addLayout(layout_down);
    setLayout(layoutPrincipal);
    QWidget::connect(OpenImage, SIGNAL(clicked()), secondeFenetre, SLOT(exec()));
    QWidget::connect(Exit, SIGNAL(clicked()), this, SLOT(close()));
    QWidget::connect(Res, SIGNAL(clicked()), this, SLOT(slot_reserver()));
}
void Reserver::slot_reserver()
{
    if (Db->reserver(nbr_jours->text().toInt(), date_livraison->date(), Lieu_prise_en_charge->text(), Db->get_Cin(user, "client"), Db->get_price(matricule), matricule))
        QMessageBox::information(this, "Réservation avec succès", "Merci pour votre reservation, Si votre demande est acceptée, vous allez recevoir un message pour tirer votre reçu; Consulter votre boite de notifications ultérieurement !");
    else
        QMessageBox::critical(this, "Erreur enregistrement échoué", "votre enregistrement a échoué !");
    this->close();
}
