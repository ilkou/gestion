#include "dbmanager.h"

DbManager::DbManager(const QString& path)
{
   my_db = new QSqlDatabase;
   *my_db = QSqlDatabase::addDatabase("QSQLITE");
   my_db->setDatabaseName(path);

   if (!my_db->open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}
bool DbManager::is_exist(QString to_find)
{
    bool success = false;
    QSqlQuery model, model0, model1;
    model.prepare("select id_marque from voiture where id_marque in(select id_marque from avoir where id_modele in (select id_modele from modele where nom_modele='"+to_find+"'))");
    model0.prepare("select id_marque from voiture where id_marque in(select id_marque from marque where nom_marque='"+to_find+"')");
    model1.prepare("select id_marque from voiture where id_marque in(select id_marque from avoir_b where id_categ in (select id_categ from categorie where nom_categ='"+to_find+"'))");
    if((model.exec() && model.next()) || (model0.exec() && model0.next()) || (model1.exec() && model1.next()))
    {
        success = true;
    }
    return success;
}
bool DbManager::is_admin(QString user)
{
    QSqlTableModel model;
    model.setTable("employe");
    model.setFilter("login='"+user+"'");
    model.select();
    return model.record(0).value("cadre").toInt() == 2147483647 ? true : false;
}
bool DbManager::is_reserved(QString Matricule){
    QSqlTableModel model;
    model.setTable("voiture");
    model.setFilter("matricule='"+Matricule+"'");
    model.select();
    return model.record(0).value("reserver").toInt() == 1 ? true : false;
}
bool DbManager::connect(QString login, QString psswd, QString type)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("select * from "+type+" where login='"+login+"' AND psswd = '"+psswd+"'");
    if(query.exec() && query.next())
    {
        success = true;
    }
    else
    {
         qDebug() << "connect " + type + " error:  "
                  << query.lastError();
    }
    query.clear();
    return success;
}
QString DbManager::get_primary_key(int row, QString type, QString value){
    QSqlTableModel model;
    model.setTable(type);
    model.select();
    int i = 0;
    while (i < row && i < model.rowCount())
        i++;
    return model.record(i).value(value).toString();
}
bool DbManager::delete_account(QString login, QString type){
    bool success = false;
    QSqlQuery query;
    query.prepare("delete from "+type+" where login='"+login+"'");
    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "addPerson error:  "
                  << query.lastError();
    }
    query.clear();
    return success;
}
QString DbManager::get_recu(int ref){
    QSqlQuery model;
    model.prepare("select ref_reserv, nbr_jours, date_livraison, lieu_prise_en_charge, valide, CIN, prix from reservation where ref_reserv= (:ref_reserv)");
    model.bindValue(":ref_reserv", ref);

    QString Text;
    if (model.exec() && model.next())
    {
        QSqlQuery model2;
        model2.prepare("select nom, prenom, tel, adresse from client where CIN= (:cin)");
        model2.bindValue(":cin", model.value(5).toString());
        if (model2.exec() && model2.next())
        {
            Text += "   Reçu de location de voiture";
            Text += "\nReçu №: " + QString::number(ref);
            Text += "\nNom et prenom: " + model2.value(0).toString() + " " + model2.value(1).toString();
            Text += "\nNuméro de téléphone: " + QString::number(model2.value(2).toInt());
            Text += "\nCin : " + model.value(5).toString();
            Text += "\nAdresse: " + model2.value(3).toString();
            Text += "\nNombre de jours : " + QString::number(model.value(1).toInt());
            Text += "\nPrix par jour: "  + QString::number(model.value(6).toFloat());
            Text += "\nPrix Total: "  + QString::number(model.value(6).toFloat() * model.value(1).toInt());
            Text += "\nDate de livraison: " + model.value(2).toString();
            Text += "\nLieu prise en charge: " + model.value(3).toString();
        }
    }
    else {
        qDebug() << "recu error:  "
                 << model.lastError();
    }
    return Text;
}

float DbManager::get_price(QString matricule)
{
    QSqlTableModel model;
    model.setTable("voiture");
    model.setFilter("matricule='"+matricule+"'");
    model.select();
    return model.record(0).value("prix").toFloat();
}
bool DbManager::reserver(int nbr_jours, QDate date_livraison, QString lieu, QString cin, float prix, QString matricule)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO reservation VALUES (:ref_reserv, :nbr_jours, :date_livraison, '"+lieu+"', :valide, :CIN, :prix, '"+matricule+"')");
    query.bindValue(":ref_reserv", get_ref_reserv() + 1);
    query.bindValue(":nbr_jours", nbr_jours);
    query.bindValue(":date_livraison", date_livraison);
    query.bindValue(":valide", 0);
    query.bindValue(":CIN", cin);
    query.bindValue(":prix", prix);
    if(query.exec())
        success = true;
    else
    {
         qDebug() << "reserver error:  "
                  << query.lastError();
    }
    query.clear();
    return success;
}
bool DbManager::addEmploye(QString CIN, QString nom, QString prenom, int tel, QString adresse, QString login, QString psswd, int cadre){
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO employe VALUES ('"+CIN+"', '"+nom+"', '"+prenom+"', :tel, '"+adresse+"', '"+login+"', '"+psswd+"', :cadre)");
    query.bindValue(":tel", tel);
    query.bindValue(":cadre", cadre == 1 ? 2147483647 : 1);
    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "addClient error:  "
                  << query.lastError();
    }
    query.clear();
    return success;
}
bool DbManager::addClient(QString CIN, QString nom, QString prenom, int tel, QString adresse, int n_permis, QDate date_permis, QString login, QString psswd)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("INSERT INTO client (CIN, nom, prenom, tel, adresse, n_permis, date_permis, login, psswd, client_fidel) VALUES ('"+CIN+"', '"+nom+"', '"+prenom+"', :tel, '"+adresse+"', :n_permis, :date_permis, '"+login+"', '"+psswd+"', :client_fidel)");
    query.bindValue(":tel", tel);
    query.bindValue(":n_permis", n_permis);
    query.bindValue(":date_permis", date_permis);
    query.bindValue(":client_fidel", 0);
    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "addClient error:  "
                  << query.lastError();
    }
    query.clear();
    return success;
}

bool DbManager::addCar(QString matricule, float prix, QString couleur, QString photo, int nbr_passagers, int ref_contrat_ass, QString nom_marque, QString nom_categ, QString nom_modele){
    bool success = false;
    QSqlQuery query, query1, query2, query3;
    int avoir = 0, avoir_b = 0;
    int idcateg = 0, idmarque = 0, idmodele = 0;

    query.prepare("INSERT INTO voiture (matricule, prix, couleur, dispo, photo, nbr_passagers, reserver, ref_contrat_ass, id_marque) VALUES ('"+matricule+"', :prix, '"+couleur+"', :dispo, '"+photo+"', :nbr_passagers, :reserver, :ref_contrat_ass, :id_marque)");
    query.bindValue(":prix", prix);
    query.bindValue(":nbr_passagers", nbr_passagers);
    query.bindValue(":ref_contrat_ass", ref_contrat_ass);
    query.bindValue(":dispo", 1);
    query.bindValue(":reserver", 0);
    query1.prepare("select * from marque where nom_marque='"+nom_marque+"'");
    if(query1.exec() && query1.next())
    {
        qDebug() << "marque existe !";
        QSqlQuery queryidm("select id_marque from marque where nom_marque='"+nom_marque+"'");
        if (queryidm. exec() && queryidm.next())
            idmarque = queryidm.value(0).toInt();
        query.bindValue(":id_marque", idmarque);
    }
    else {
        qDebug() << "nouvelle marque !";
        QSqlQuery querymarque;
        querymarque.prepare("INSERT INTO marque (id_marque, nom_marque) VALUES (:id_marque, '"+nom_marque+"')");
        QSqlQuery queryid("select count(*) from voiture");
        if (queryid.next())
            idmarque = (queryid.value(0).toInt() + 1) * 100;
        else {
            qDebug() << queryid.lastError();
        }
        query.bindValue(":id_marque", idmarque);
        querymarque.bindValue(":id_marque", idmarque);
        if(querymarque.exec())
            qDebug() << "ajouter marque reussi";
        else
        {
             qDebug() << querymarque.lastError();
        }
        avoir++;avoir_b++;
    }
    query2.prepare("select * from categorie where nom_categ='"+nom_categ+"'");
    if(query2.exec() && query2.next())
    {
        qDebug() << "categorie existe !";
        QSqlQuery queryidc("select id_categ from categorie where nom_categ='"+nom_categ+"'");
        if (queryidc.next())
            idcateg = queryidc.value(0).toInt();
    }
    else {
        qDebug() << "nouvelle categorie !";
        QSqlQuery querycateg;
        querycateg.prepare("INSERT INTO categorie (id_categ, nom_categ) VALUES (:id_categ, '"+nom_categ+"')");
        QSqlQuery queryidcateg("select count(*) from categorie");
        if (queryidcateg.next())
            idcateg = queryidcateg.value(0).toInt() + 1;
        else {
            qDebug() << queryidcateg.lastError();
        }
        querycateg.bindValue(":id_categ", idcateg);
        if(querycateg.exec())
            qDebug() << "ajouter categorie reussi";
        else
        {
             qDebug() << querycateg.lastError();
        }
        avoir_b++;
    }
    query3.prepare("select * from modele where nom_modele='"+nom_modele+"'");
    if(query3.exec() && query3.next())
    {
        qDebug() << "modele existe !";
        QSqlQuery queryidma("select id_modele from categorie where nom_categ='"+nom_categ+"'");
        if (queryidma.next())
            idmodele = queryidma.value(0).toInt();
    }
    else {
        qDebug() << "nouveau modele  !";
        QSqlQuery querymodele;
        querymodele.prepare("INSERT INTO modele (id_modele, nom_modele) VALUES (:id_modele, '"+nom_modele+"')");
        QSqlQuery queryidmodele("select count(*) from modele");
        if (queryidmodele.next())
            idmodele = queryidmodele.value(0).toInt() + 1;
        else {
            qDebug() << queryidmodele.lastError();
        }
        querymodele.bindValue(":id_modele", idmodele);
        if(querymodele.exec())
            qDebug() << "ajouter modele reussi";
        else
        {
             qDebug() << querymodele.lastError();
        }
        avoir++;
    }
    if (avoir != 0){
        QSqlQuery avoirquery;
        avoirquery.prepare("INSERT INTO avoir (id_marque, id_modele) VALUES (:id_marque, :id_modele)");
        avoirquery.bindValue(":id_marque", idmarque);
        avoirquery.bindValue(":id_modele", idmodele);
        if(avoirquery.exec())
            qDebug() << "ajouter avoir reussi";
        else
        {
             qDebug() << avoirquery.lastError();
        }
    }
    if (avoir_b != 0){
        QSqlQuery avoirbquery;
        avoirbquery.prepare("INSERT INTO avoir_b (id_marque, id_categ) VALUES (:id_marque, :id_categ)");
        avoirbquery.bindValue(":id_marque", idmarque);
        avoirbquery.bindValue(":id_categ", idcateg);
        if(avoirbquery.exec())
            qDebug() << "ajouter avoirb reussi";
        else
        {
             qDebug() << avoirbquery.lastError();
        }
    }
    if(query.exec())
        success = true;
    else
    {
         qDebug() << "addCar error:  "
                  << query.lastError();
    }
    query.clear(); query1.clear(); query2.clear(); query3.clear();
    return success;
}
bool DbManager::editProfil(QString CIN, QString nom, QString prenom, int tel, QString adresse,int fidelite, QString login, QString psswd){
    bool success = false;
    QSqlQuery query;
    query.prepare("update client set cin='"+CIN+"', nom='"+nom+"', prenom='"+prenom+"', client_fidel=:client_fidel, tel=:tel, adresse='"+adresse+"', psswd='"+psswd+"' where login='"+login+"'");
    query.bindValue(":tel", tel);
    query.bindValue(":client_fidel", fidelite);
    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "editclient error:  "
                  << query.lastError();
    }
    query.clear();
    return success;
}
bool DbManager::editProfil(QString CIN, QString nom, QString prenom, int tel, QString adresse,QString login, QString psswd, int cadre)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("update employe set cin='"+CIN+"', nom='"+nom+"', prenom='"+prenom+"', cadre=:cadre, tel=:tel, adresse='"+adresse+"', psswd='"+psswd+"' where login='"+login+"'");
    query.bindValue(":tel", tel);
    query.bindValue(":cadre", cadre);
    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "editEmploye error:  "
                  << query.lastError();
    }
    query.clear();
    return success;
}
bool DbManager::editProfil(QString CIN, QString nom, QString prenom, int tel, QString adresse,QString login, QString psswd, QString type)
{
    bool success = false;
    QSqlQuery query;
    query.prepare("update "+type+" set cin='"+CIN+"', nom='"+nom+"', prenom='"+prenom+"', tel=:tel, adresse='"+adresse+"', psswd='"+psswd+"' where login='"+login+"'");
    query.bindValue(":tel", tel);
    if(query.exec())
    {
        success = true;
    }
    else
    {
         qDebug() << "edit"+type+" error:  "
                  << query.lastError();
    }
    query.clear();
    return success;
}
void DbManager::close_db()
{
    my_db->close();
    QSqlDatabase::removeDatabase("QSQLITE");
    delete my_db;
}
int DbManager::get_n_client(){
    QSqlQuery query("select count(*) from client");
    if (query.next())
        return query.value(0).toInt();
    return 0;
}
int DbManager::get_n_employe(){
    QSqlQuery query("select count(*) from employe");
    if (query.next())
        return query.value(0).toInt();
    return 0;
}
int DbManager::get_n_marque(){
    QSqlQuery query("select count(*) from marque");
    if (query.next())
        return query.value(0).toInt();
    return 0;
}
int DbManager::get_n_voiture(){
    QSqlQuery query("select count(*) from voiture where reserver = 1");
    if (query.next())
        return query.value(0).toInt();
    return 0;
}

int DbManager::get_n_fidele(){
    QSqlQuery query("select count(*) from client where client_fidel > "+QString::number(FIDEL_MAX / 2)+"");
    if (query.next())
        return query.value(0).toInt();
    return 0;
}
int DbManager::get_n_cars()
{
    QSqlQuery query("select count(*) from voiture");
    if (query.next())
        return query.value(0).toInt();
    return 0;
}
int DbManager::get_ref_reserv()
{
    QSqlQuery query("select count(*) from reservation");
    if (query.next())
        return query.value(0).toInt();
    return 0;
}
QString DbManager::get_categorie(QString id_marque)
{
    QSqlQuery query("select nom_categ from categorie where id_categ in (select id_categ from avoir_b where id_marque in ( select id_marque from voiture where id_marque = "+id_marque+" ))");
    if (query.next())
        return query.value(0).toString();
    return "error";
}
QString DbManager::get_marque(QString id_marque)
{
    QSqlQuery query("select nom_marque from marque where id_marque in ( select id_marque from voiture where id_marque = "+id_marque+" )");
    if (query.next())
        return query.value(0).toString();
    return "error";
}
QString DbManager::get_modele(QString id_marque)
{
    QSqlQuery query("select nom_modele from modele where id_modele in (select id_modele from avoir where id_marque in ( select id_marque from voiture where id_marque = "+id_marque+" ))");
    if (query.next())
        return query.value(0).toString();
    return "error";
}
int DbManager::get_loyalty(QString login)
{
    QSqlTableModel model;
    model.setTable("client");
    model.setFilter("login='"+login+"'");
    model.select();
    return model.record(0).value("client_fidel").toInt();
}
QString DbManager::get_nom(QString login, QString type)
{
    QSqlTableModel model;
    model.setTable(type);
    model.setFilter("login='"+login+"'");
    model.select();
    return model.record(0).value("nom").toString();
}
QString DbManager::get_prenom(QString login, QString type)
{
    QSqlTableModel model;
    model.setTable(type);
    model.setFilter("login='"+login+"'");
    model.select();
    return model.record(0).value("prenom").toString();
}
QString DbManager::get_Cin(QString login, QString type){
    QSqlTableModel model;
    model.setTable(type);
    model.setFilter("login='"+login+"'");
    model.select();
    return model.record(0).value("cin").toString();
}
int DbManager::get_Tel(QString login, QString type){
    QSqlTableModel model;
    model.setTable(type);
    model.setFilter("login='"+login+"'");
    model.select();
    return model.record(0).value("tel").toInt();
}
QString DbManager::get_Adresse(QString login, QString type){
    QSqlTableModel model;
    model.setTable(type);
    model.setFilter("login='"+login+"'");
    model.select();
    return model.record(0).value("adresse").toString();
}
QString DbManager::get_psswd(QString login, QString type){
    QSqlTableModel model;
    model.setTable(type);
    model.setFilter("login='"+login+"'");
    model.select();
    return model.record(0).value("psswd").toString();
}
