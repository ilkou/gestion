#include <employe.h>
#include <editaccount.h>
#include <newaccount.h>
#include <newemploye.h>
#include <newcar.h>

Employe::Employe(QWidget *parent, QString u, QString p)
{
    user = u;
    path = p;
    setWindowTitle("Espace Employé");
    QPushButton *Back  = new QPushButton("Accueil");
    QPushButton *Exit  = new QPushButton("Quitter");
    QGridLayout *principal = new QGridLayout;
    QTabWidget *onglets = new QTabWidget(this);
    onglets->resize(WIDTH/2 - 10, HEIGHT/2 - 50);
    QWidget *page0 = new QWidget;
    QWidget *page1 = new QWidget;
    QWidget *page2 = new QWidget;
    QWidget *page3 = new QWidget;
    QWidget *page4 = new QWidget;
    QWidget *page5 = new QWidget;
    accueil_widget(page0);
    profil_widget(page1);
    notif_widget(page2);
    voitures_widget(page3);
    clients_widget(page4);
    onglets->addTab(page0, "Accueil");
    onglets->addTab(page1, "Profil");
    onglets->addTab(page2, "Notifications");
    onglets->addTab(page3, "Voitures");
    onglets->addTab(page4, "Clients");
    if (Db->is_admin(user))
    {
        employes_widget(page5);
        onglets->addTab(page5, "Employés");
    }
    principal->addWidget(onglets,0,0, 0, 2);
    principal->addWidget(Back, 1, 0);
    principal->addWidget(Exit, 1, 1);
    setLayout(principal);
    setFixedSize(WIDTH/2, HEIGHT/2);
    QObject::connect(Exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(Back, SIGNAL(clicked()), parent, SLOT(accueil()));
    QObject::connect(Back, SIGNAL(clicked()), this, SLOT(close()));
}
void Employe::edit_employe()
{
    EditAccount *account = new EditAccount(this, user, "employe", 1);
    account->set_db(Db);
    account->exec();
}
void Employe::ajouter_client(){
    NewAccount *account = new NewAccount(this);
    account->set_path(this->path);
    account->exec();
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM client");
    int currentRow;
    currentRow = Db->get_n_client() - 1;
    tableWidget_client->setRowCount(currentRow + 1);
    tableWidget_client->setItem(currentRow, 0, new QTableWidgetItem(model.record(currentRow).value("login").toString()));
    tableWidget_client->setItem(currentRow, 1, new QTableWidgetItem(model.record(currentRow).value("cin").toString()));
    tableWidget_client->setItem(currentRow, 2, new QTableWidgetItem(model.record(currentRow).value("nom").toString()));
    tableWidget_client->setItem(currentRow, 3, new QTableWidgetItem(model.record(currentRow).value("prenom").toString()));
    tableWidget_client->setItem(currentRow, 4, new QTableWidgetItem(QString::number(model.record(currentRow).value("tel").toInt())));
    tableWidget_client->setItem(currentRow, 5, new QTableWidgetItem(model.record(currentRow).value("adresse").toString()));
    tableWidget_client->setItem(currentRow, 6, new QTableWidgetItem(QString::number(model.record(currentRow).value("n_permis").toInt())));
    tableWidget_client->setItem(currentRow, 7, new QTableWidgetItem(model.record(currentRow).value("date_permis").toDate().toString()));
    tableWidget_client->setItem(currentRow, 8, new QTableWidgetItem(QString::number(model.record(currentRow).value("client_fidel").toInt())));
    tableWidget_client->setItem(currentRow, 9, new QTableWidgetItem("     X"));
    tableWidget_client->setItem(currentRow, 10, new QTableWidgetItem("     X"));
}
void set_style(QToolButton *button, int num, QString tooltip, QString Icon)
{
    button->setFont(QFont("Gill Sans", 30));
    button->setToolTip(tooltip);
    button->setCursor(Qt::PointingHandCursor);
    button->setText(QString::number(num));
    button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button->setIcon(QIcon(Icon));
    button->setIconSize(QSize(80,90));
    button->setFixedSize(300, 150);
    button->setStyleSheet("border: 0px;");
}
void Employe::accueil_widget(QWidget *page){
    QToolButton *Client = new QToolButton;
    QToolButton *Employe = new QToolButton;
    QToolButton *Voiture = new QToolButton;
    QToolButton *reserv = new QToolButton;
    QToolButton *fidel = new QToolButton;
    QToolButton *marque = new QToolButton;

    set_style(Client, Db->get_n_client(), "le nombre total de clients", ":/icones/ressources/icons/client.png");
    set_style(Employe, Db->get_n_employe(), "le nombre total de employés", ":/icones/ressources/icons/employe.png");
    set_style(Voiture, Db->get_n_voiture(), "le nombre total de voiture louées", ":/icones/ressources/icons/car.png");
    set_style(reserv, Db->get_ref_reserv(), "le nombre total des reservations", ":/icones/ressources/icons/res.png");
    set_style(fidel, Db->get_n_fidele(), "nombre des clients fidèles", ":/icones/ressources/icons/loyalty.png");
    set_style(marque, Db->get_n_marque(), "nombre de marques", ":/icones/ressources/icons/marque.png");

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(Employe, 0, 0);
    layout->addWidget(Client, 0, 1);
    layout->addWidget(reserv, 0, 2);
    layout->addWidget(fidel, 1, 0);
    layout->addWidget(Voiture, 1, 1);
    layout->addWidget(marque, 1, 2);
    page->setLayout(layout);
}
void Employe::profil_widget(QWidget *page){
    QPushButton *Edit = new QPushButton("Modifier votre profil");
    Edit->setStyleSheet("border: 0px;background: transparent; color: #0000FF; font-size: 40");
    QLineEdit *log = new QLineEdit;
    QLineEdit *Cin = new QLineEdit;
    QLineEdit *Nom = new QLineEdit;
    QLineEdit *Prenom = new QLineEdit;
    QLineEdit *Tel = new QLineEdit;
    QLineEdit *Adresse = new QLineEdit;
    log->setReadOnly(true);
    log->setText(user);
    Cin->setReadOnly(true);
    Cin->setText(Db->get_Cin(user, "employe"));
    Nom->setReadOnly(true);
    Nom->setText(Db->get_nom(user, "employe"));
    Prenom->setReadOnly(true);
    Prenom->setText(Db->get_prenom(user, "employe"));
    Tel->setReadOnly(true);
    Tel->setText(QString::number(Db->get_Tel(user, "employe")));
    Adresse->setReadOnly(true);
    Adresse->setText(Db->get_Adresse(user, "employe"));

    QFormLayout *page_layout = new QFormLayout;
    page_layout->addRow("Login            ", log);
    page_layout->addRow("CIN              ", Cin);
    page_layout->addRow("Nom              ", Nom);
    page_layout->addRow("Prénom           ", Prenom);
    page_layout->addRow("Numéro de mobile ", Tel);
    page_layout->addRow("Adresse          ", Adresse);
    QVBoxLayout *group = new QVBoxLayout;
    group->addLayout(page_layout);
    group->addWidget(Edit);
    group->setAlignment(Qt::AlignCenter);
    page->setLayout(group);
    QObject::connect(Edit, SIGNAL(clicked()), this, SLOT(edit_employe()));
}
void Employe::accepter_refuser_reserv(int row, int column){
    QString log = tableWidget_notif->item(row,0)->text();
    if (column == 7)
    {
        QSqlQuery query, query1, query2;
        QSqlTableModel model;
        model.setTable("reservation");
        model.setFilter("ref_reserv="+log+"");
        model.select();
        query.prepare("update reservation set valide=:valide where ref_reserv='"+log+"'");
        query.bindValue(":valide", 1);
        query2.prepare("update voiture set reserver=:reserver where matricule='"+model.record(0).value("matricule").toString()+"'");
        query2.bindValue(":reserver", 1);
        query1.prepare("insert into consulter values('"+model.record(0).value("cin").toString()+"', '"+model.record(0).value("matricule").toString()+"', :nbr_jours)");
        query1.bindValue(":nbr_jours", model.record(0).value("nbr_jours").toInt());
        if (query.exec() && query1.exec() && query2.exec())
        {
            tableWidget_notif->removeRow(row);
            QMessageBox::information(this, "reservation accepté", "<strong>la reservation a été effectué avec succès !</strong>");
        }
        else
            QMessageBox::critical(this, "Erreur reservation échoué", "la reservation a échoué !");
    }
    if (column == 8)
    {
        QSqlQuery query3;
        query3.prepare("update reservation set valide=:valide where ref_reserv='"+log+"'");
        query3.bindValue(":valide", 1);
        if (query3.exec())
        {
            tableWidget_notif->removeRow(row);
            QMessageBox::information(this, "reservation refusé", "<strong>le refus a été effectué avec succès !</strong>");
        }
        else
            QMessageBox::critical(this, "Erreur refuser échoué", "le refus a échoué !");
    }
}
void Employe::notif_widget(QWidget *page){
    tableWidget_notif = new QTableWidget(Db->get_ref_reserv(), 9, this);
    QStringList labels;
    labels << "ref reservation" << "N de jours" << "date livraison" << "lieu P.E.C" << "CIN du client" << "Voiture" << "Prix" << "Accepter" << "Refuser";
    tableWidget_notif->setHorizontalHeaderLabels(labels);

    QSqlQueryModel model, model2, model3;
    QString nom_marque, nom_modele;
    model.setQuery("SELECT * FROM reservation");
    tableWidget_notif->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int currentRow;
    for (int i = 0; i < model.rowCount(); ++i) {
        currentRow = i;
        tableWidget_notif->setRowCount(currentRow + 1);
        tableWidget_notif->setItem(currentRow, 0, new QTableWidgetItem(QString::number(model.record(i).value("ref_reserv").toInt())));
        tableWidget_notif->setItem(currentRow, 1, new QTableWidgetItem(QString::number(model.record(i).value("nbr_jours").toInt())));
        tableWidget_notif->setItem(currentRow, 2, new QTableWidgetItem(model.record(i).value("date_livraison").toDate().toString()));
        tableWidget_notif->setItem(currentRow, 3, new QTableWidgetItem(model.record(i).value("lieu_prise_en_charge").toString()));
        tableWidget_notif->setItem(currentRow, 4, new QTableWidgetItem(model.record(i).value("CIN").toString()));
        model2.setQuery("select nom_marque from marque where id_marque in (select id_marque from voiture where matricule='"+model.record(i).value("matricule").toString()+"')");
        model3.setQuery("select nom_modele from modele where id_modele in (select id_modele from avoir where id_marque in (select id_marque from voiture where matricule='"+model.record(i).value("matricule").toString()+"'))");
        nom_marque = model2.record(0).value(0).toString();
        nom_modele = model3.record(0).value(0).toString();
        tableWidget_notif->setItem(currentRow, 5, new QTableWidgetItem(nom_marque+"-"+nom_modele));
        tableWidget_notif->setItem(currentRow, 6, new QTableWidgetItem(QString::number(model.record(i).value("prix").toFloat())));
        tableWidget_notif->setItem(currentRow, 7, new QTableWidgetItem("     X"));
        tableWidget_notif->setItem(currentRow, 8, new QTableWidgetItem("      X"));

    }
    for (int i = 0; i < model.rowCount(); ++i) {
        if (model.record(i).value("valide").toInt() == 1)
            tableWidget_notif->removeRow(i);
    }
    QObject::connect(tableWidget_notif, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(accepter_refuser_reserv(int,int)));
    tableWidget_notif->resizeColumnsToContents();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tableWidget_notif);
    page->setLayout(layout);
}
void Employe::ajouter_voiture(){
    NewCar *account = new NewCar(this);
    account->set_path(this->path);
    account->exec();
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM voiture");
    int currentRow;
    currentRow = Db->get_n_cars() - 1;
    tableWidget_voiture->setRowCount(currentRow + 1);
    tableWidget_voiture->setItem(currentRow, 0, new QTableWidgetItem(model.record(currentRow).value("matricule").toString()));
    tableWidget_voiture->setItem(currentRow, 1, new QTableWidgetItem(QString::number(model.record(currentRow).value("prix").toFloat())));
    tableWidget_voiture->setItem(currentRow, 2, new QTableWidgetItem(model.record(currentRow).value("couleur").toString()));
    tableWidget_voiture->setItem(currentRow, 3, new QTableWidgetItem(model.record(currentRow).value("dispo").toInt() ? "oui" : "non"));
    tableWidget_voiture->setItem(currentRow, 4, new QTableWidgetItem(model.record(currentRow).value("photo").toString()));
    tableWidget_voiture->setItem(currentRow, 5, new QTableWidgetItem(QString::number(model.record(currentRow).value("nbr_passagers").toInt())));
    tableWidget_voiture->setItem(currentRow, 6, new QTableWidgetItem(model.record(currentRow).value("reserver").toInt() ? "oui" : "non"));
    tableWidget_voiture->setItem(currentRow, 7, new QTableWidgetItem(QString::number(model.record(currentRow).value("ref_contrat_ass").toInt())));
    tableWidget_voiture->setItem(currentRow, 8, new QTableWidgetItem(Db->get_categorie(QString::number(model.record(currentRow).value("id_marque").toInt()))));
    tableWidget_voiture->setItem(currentRow, 9, new QTableWidgetItem(Db->get_modele(QString::number(model.record(currentRow).value("id_marque").toInt()))));
    tableWidget_voiture->setItem(currentRow, 10, new QTableWidgetItem(Db->get_marque(QString::number(model.record(currentRow).value("id_marque").toInt()))));
    tableWidget_voiture->resizeColumnsToContents();
}
void Employe::voitures_widget(QWidget *page){
    QPushButton *Edit = new QPushButton("Ajouter une voiture");
    Edit->setStyleSheet("border: 0px;background: transparent; color: #0000FF; font-size: 40");
    tableWidget_voiture = new QTableWidget(Db->get_n_voiture(), 11, this);
    QStringList labels;
    labels << "Matricule" << "Prix" << "Couleur" << "Disponibilité" << "Photo" << "№ passagers" << "Reserver" << "Réf assurance" << "catégorie" << "Modèle" << "Marque";
    tableWidget_voiture->setHorizontalHeaderLabels(labels);

    QSqlQueryModel model;
    model.setQuery("SELECT * FROM voiture");
    tableWidget_voiture->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int currentRow;
    for (int i = 0; i < model.rowCount(); ++i) {
        currentRow = i;
        tableWidget_voiture->setRowCount(currentRow + 1);
        tableWidget_voiture->setItem(currentRow, 0, new QTableWidgetItem(model.record(i).value("matricule").toString()));
        tableWidget_voiture->setItem(currentRow, 1, new QTableWidgetItem(QString::number(model.record(i).value("prix").toFloat())));
        tableWidget_voiture->setItem(currentRow, 2, new QTableWidgetItem(model.record(i).value("couleur").toString()));
        tableWidget_voiture->setItem(currentRow, 3, new QTableWidgetItem(model.record(i).value("dispo").toInt() ? "oui" : "non"));
        tableWidget_voiture->setItem(currentRow, 4, new QTableWidgetItem(model.record(i).value("photo").toString()));
        tableWidget_voiture->setItem(currentRow, 5, new QTableWidgetItem(QString::number(model.record(i).value("nbr_passagers").toInt())));
        tableWidget_voiture->setItem(currentRow, 6, new QTableWidgetItem(model.record(i).value("reserver").toInt() ? "oui" : "non"));
        tableWidget_voiture->setItem(currentRow, 7, new QTableWidgetItem(QString::number(model.record(i).value("ref_contrat_ass").toInt())));
        tableWidget_voiture->setItem(currentRow, 8, new QTableWidgetItem(Db->get_categorie(QString::number(model.record(i).value("id_marque").toInt()))));
        tableWidget_voiture->setItem(currentRow, 9, new QTableWidgetItem(Db->get_modele(QString::number(model.record(i).value("id_marque").toInt()))));
        tableWidget_voiture->setItem(currentRow, 10, new QTableWidgetItem(Db->get_marque(QString::number(model.record(i).value("id_marque").toInt()))));
    }
    tableWidget_voiture->resizeColumnsToContents();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tableWidget_voiture);
    layout->addWidget(Edit);
    page->setLayout(layout);
    QObject::connect(Edit, SIGNAL(clicked()), this, SLOT(ajouter_voiture()));
}
void Employe::modifier_supprimer_client(int row, int column){
    QString log = Db->get_primary_key(row, "client", "login");
    if (column == 9)
    {
        EditAccount *account = new EditAccount(this, log, "client", 1);
        account->set_db(Db);
        account->exec();
    }
    if (column == 10)
    {
        if (Db->delete_account(log, "client"))
        {
            tableWidget_client->removeRow(row);
            QMessageBox::information(this, "supprimer avec succès", "<strong>la suppression a été effectué avec succès !</strong>");
        }
        else
            QMessageBox::critical(this, "Erreur suppression échoué", "la suppression a échoué !");
    }
}
void Employe::clients_widget(QWidget *page){
    QPushButton *Edit = new QPushButton("Ajouter un client");
    Edit->setStyleSheet("border: 0px;background: transparent; color: #0000FF; font-size: 40");
    tableWidget_client = new QTableWidget(Db->get_n_client(), 11, this);
    QStringList labels;
    labels << "Login" << "CIN" << "Nom" << "Prenom" << "Numéro de mobile" << "Adresse" << "Numéro de permis" << "Date d'obtention du permis" << "fidélité" << "Modifier" << "Supprimer";
    tableWidget_client->setHorizontalHeaderLabels(labels);

    QSqlQueryModel model;
    model.setQuery("SELECT * FROM client");
    tableWidget_client->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int currentRow;
    for (int i = 0; i < model.rowCount(); ++i) {
        currentRow = i;
        tableWidget_client->setRowCount(currentRow + 1);
        tableWidget_client->setItem(currentRow, 0, new QTableWidgetItem(model.record(i).value("login").toString()));
        tableWidget_client->setItem(currentRow, 1, new QTableWidgetItem(model.record(i).value("cin").toString()));
        tableWidget_client->setItem(currentRow, 2, new QTableWidgetItem(model.record(i).value("nom").toString()));
        tableWidget_client->setItem(currentRow, 3, new QTableWidgetItem(model.record(i).value("prenom").toString()));
        tableWidget_client->setItem(currentRow, 4, new QTableWidgetItem(QString::number(model.record(i).value("tel").toInt())));
        tableWidget_client->setItem(currentRow, 5, new QTableWidgetItem(model.record(i).value("adresse").toString()));
        tableWidget_client->setItem(currentRow, 6, new QTableWidgetItem(QString::number(model.record(i).value("n_permis").toInt())));
        tableWidget_client->setItem(currentRow, 7, new QTableWidgetItem(model.record(i).value("date_permis").toDate().toString()));
        tableWidget_client->setItem(currentRow, 8, new QTableWidgetItem(QString::number(model.record(i).value("client_fidel").toInt())));
        tableWidget_client->setItem(currentRow, 9, new QTableWidgetItem("     X"));
        tableWidget_client->setItem(currentRow, 10, new QTableWidgetItem("      X"));

    }
    QObject::connect(tableWidget_client, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(modifier_supprimer_client(int,int)));
    tableWidget_client->resizeColumnsToContents();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tableWidget_client);
    layout->addWidget(Edit);
    page->setLayout(layout);
    QObject::connect(Edit, SIGNAL(clicked()), this, SLOT(ajouter_client()));
}
void Employe::modifier_supprimer_employe(int row,int column){
    QString log = Db->get_primary_key(row, "employe", "login");
    if (column == 7)
    {
        EditAccount *account = new EditAccount(this, log, "employe", 1);
        account->set_db(Db);
        account->exec();
    }
    if (column == 8)
    {
        if (Db->delete_account(log, "employe"))
        {
            tableWidget_employe->removeRow(row);
            QMessageBox::information(this, "supprimer avec succès", "<strong>la suppression a été effectué avec succès !</strong>");
        }
        else
            QMessageBox::critical(this, "Erreur suppression échoué", "la suppression a échoué !");
    }
}
void Employe::ajouter_employe(){
    NewEmploye *account = new NewEmploye(this);
    account->set_path(this->path);
    account->exec();
    QSqlQueryModel model;
    model.setQuery("SELECT * FROM employe");
    int currentRow;
    currentRow = Db->get_n_employe() - 1;
    qDebug() << currentRow;
    tableWidget_employe->setRowCount(currentRow + 1);
    tableWidget_employe->setItem(currentRow, 0, new QTableWidgetItem(model.record(currentRow).value("login").toString()));
    tableWidget_employe->setItem(currentRow, 1, new QTableWidgetItem(model.record(currentRow).value("cin").toString()));
    tableWidget_employe->setItem(currentRow, 2, new QTableWidgetItem(model.record(currentRow).value("nom").toString()));
    tableWidget_employe->setItem(currentRow, 3, new QTableWidgetItem(model.record(currentRow).value("prenom").toString()));
    tableWidget_employe->setItem(currentRow, 4, new QTableWidgetItem(QString::number(model.record(currentRow).value("tel").toInt())));
    tableWidget_employe->setItem(currentRow, 5, new QTableWidgetItem(model.record(currentRow).value("adresse").toString()));
    tableWidget_employe->setItem(currentRow, 6, new QTableWidgetItem(QString::number(model.record(currentRow).value("cadre").toInt())));
    tableWidget_employe->setItem(currentRow, 7, new QTableWidgetItem("     X"));
    tableWidget_employe->setItem(currentRow, 8, new QTableWidgetItem("     X"));
    tableWidget_employe->resizeColumnsToContents();
}
void Employe::employes_widget(QWidget *page){
    QPushButton *Edit = new QPushButton("Ajouter un employe");
    Edit->setStyleSheet("border: 0px;background: transparent; color: #0000FF; font-size: 40");
    tableWidget_employe = new QTableWidget(Db->get_n_employe(), 9, this);
    QStringList labels;
    labels << "Login" << "CIN" << "Nom" << "Prenom" << "Numéro de mobile" << "Adresse"  << "cadre" << "Modifier" << "Supprimer";
    tableWidget_employe->setHorizontalHeaderLabels(labels);

    QSqlQueryModel model;
    model.setQuery("SELECT * FROM employe");
    tableWidget_employe->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int currentRow;
    for (int i = 0; i < model.rowCount(); ++i) {
        currentRow = i;
        tableWidget_employe->setRowCount(currentRow + 1);
        tableWidget_employe->setItem(currentRow, 0, new QTableWidgetItem(model.record(i).value("login").toString()));
        tableWidget_employe->setItem(currentRow, 1, new QTableWidgetItem(model.record(i).value("cin").toString()));
        tableWidget_employe->setItem(currentRow, 2, new QTableWidgetItem(model.record(i).value("nom").toString()));
        tableWidget_employe->setItem(currentRow, 3, new QTableWidgetItem(model.record(i).value("prenom").toString()));
        tableWidget_employe->setItem(currentRow, 4, new QTableWidgetItem(QString::number(model.record(i).value("tel").toInt())));
        tableWidget_employe->setItem(currentRow, 5, new QTableWidgetItem(model.record(i).value("adresse").toString()));
        tableWidget_employe->setItem(currentRow, 6, new QTableWidgetItem(model.record(i).value("cadre").toString()));
        tableWidget_employe->setItem(currentRow, 7, new QTableWidgetItem("     X"));
        tableWidget_employe->setItem(currentRow, 8, new QTableWidgetItem("      X"));

    }
    QObject::connect(tableWidget_employe, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(modifier_supprimer_employe(int,int)));
    tableWidget_employe->resizeColumnsToContents();
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tableWidget_employe);
    layout->addWidget(Edit);
    page->setLayout(layout);
    QObject::connect(Edit, SIGNAL(clicked()), this, SLOT(ajouter_employe()));
}
