#include <client.h>
#include <reserver.h>
#include <editaccount.h>

Client::Client(QWidget *parent, QString u)
{
    user = u;
    setFixedSize(WIDTH/2, HEIGHT/2);
    setWindowTitle("Espace Client");
    n_cars = Db->get_n_cars();
    QScrollArea *Area = createCentralWidget(n_cars);
    setCentralWidget(Area);
    createToolBars();
    QObject::connect(Exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(Back, SIGNAL(clicked()), parent, SLOT(accueil()));
    QObject::connect(Back, SIGNAL(clicked()), this, SLOT(close()));
    int i = 0;
    while (i < n_cars)
    {
        QObject::connect(&Button[i], SIGNAL(clicked()), this, SLOT(reserver_voiture()));
        i++;
    }
    QObject::connect(Edit, SIGNAL(clicked()), this, SLOT(edit_client()));
    QObject::connect(Notif, SIGNAL(clicked()), this, SLOT(notification()));
    QObject::connect(ExRes, SIGNAL(clicked()), this, SLOT(ex_reservation()));
    QObject::connect(Rech_Button, SIGNAL(clicked()), this, SLOT(recherche_client()));
    QObject::connect(Recherche, SIGNAL(returnPressed()), this, SLOT(recherche_client()));
}
void Client::notification()
{
    QSqlTableModel model;
    int i = 0, isvalide = 0, ref;
    model.setTable("reservation");
    model.setFilter("CIN='"+Db->get_Cin(user, "client")+"'");
    model.select();
    while (i < model.rowCount()) {
        isvalide = model.record(i).value("valide").toInt();
        if (isvalide == 1)
            break;
        i++;
    }
    if (isvalide == 1)
    {
        ref = model.record(i).value("ref_reserv").toInt();
        QDialog *notif_widget = new QDialog(this);
        QPushButton *fermer = new QPushButton("OK!");
        QTextEdit *Recu = new QTextEdit();
        Recu->setPlainText(Db->get_recu(ref));
        Recu->setReadOnly(true);
        Recu->setFont(QFont("Courier"));
        Recu->setLineWrapMode(QTextEdit::NoWrap);

        QVBoxLayout *layoutaa = new QVBoxLayout;
        layoutaa->addWidget(Recu);
        QGroupBox *group = new QGroupBox("Vous avez un reçu !");
        group->setLayout(layoutaa);
        QVBoxLayout *layoutp = new QVBoxLayout;
        layoutp->addWidget(group);
        layoutp->addWidget(fermer);

        notif_widget->resize(400, 300);
        notif_widget->setLayout(layoutp);
        QObject::connect(fermer, SIGNAL(clicked()), notif_widget, SLOT(close()));
        notif_widget->exec();
    }
    else
        QMessageBox::information(this, "info", "<strong>Aucune notification à afficher !</strong>");
}
void Client::ex_reservation()
{
    QSqlTableModel model;
    int i = 0, isvalide = 0;
    model.setTable("consulter");
    model.setFilter("CIN='"+Db->get_Cin(user, "client")+"'");
    model.select();
    if (model.rowCount() >= 0)
        isvalide = 1;
    if (isvalide == 1)
    {
        QString Text;
        while (i < model.rowCount())
        {
            QSqlTableModel model2;
            model2.setTable("voiture");
            model2.setFilter("matricule='"+ model.record(i).value("matricule").toString() +"'");
            model2.select();

            QSqlTableModel model3;
            model3.setTable("marque");
            model3.setFilter("id_marque='"+ model2.record(0).value("id_marque").toString() +"'");
            model3.select();
            Text += "\nReservation № : " + QString::number(i + 1);
            Text += "\nmarque: " + model3.record(0).value("nom_marque").toString();
            Text += "\nNombre de jours : " + QString::number(model.record(i).value("nbr_jours").toInt());
            Text += "\nPrix par jour: "  + QString::number(model2.record(0).value("prix").toFloat());
            Text += "\nPrix Total: "  + QString::number(model2.record(0).value("prix").toFloat() * model.record(i).value("nbr_jours").toInt());
            Text += "\n--------------------------------";
            i++;
        }
        QDialog *reserv_widget = new QDialog(this);
        QPushButton *fermer = new QPushButton("OK!");
        QTextEdit *Recu = new QTextEdit();
        Recu->setPlainText(Text);
        Recu->setReadOnly(true);
        Recu->setFont(QFont("Courier"));
        Recu->setLineWrapMode(QTextEdit::NoWrap);

        QVBoxLayout *layoutaa = new QVBoxLayout;
        layoutaa->addWidget(Recu);
        QGroupBox *group = new QGroupBox("Votre Ex-reservation !");
        group->setLayout(layoutaa);
        QVBoxLayout *layoutp = new QVBoxLayout;
        layoutp->addWidget(group);
        layoutp->addWidget(fermer);

        reserv_widget->resize(400, 300);
        reserv_widget->setLayout(layoutp);
        QObject::connect(fermer, SIGNAL(clicked()), reserv_widget, SLOT(close()));
        reserv_widget->exec();
    }
    else
        QMessageBox::information(this, "info", "<strong>Aucune reservation à afficher !</strong>");
}
void Client::recherche_client()
{
    if (Recherche->text().isEmpty())
        QMessageBox::critical(this, "Erreur", "Entrer quelque chose !");
    if (Db->is_exist(Recherche->text()))
        QMessageBox::information(this, "Il est là", "<strong>Rassurez-vous, la voiture que vous cherchez est chez nous !</strong>");
    else
        QMessageBox::critical(this, "Erreur", "Malheureusement, cette voiture n'est pas parmi les nôtres actuellement. Veuillez la rechercher ultérieurement. Merci !");
}
void Client::reserver_voiture()
{
    QToolButton* button_clicked = qobject_cast<QToolButton*>(sender());
    Reserver *res = new Reserver(this, button_clicked->accessibleName());
    res->set_db(Db);
    res->set_user(user);
    res->exec();
}
void Client::edit_client()
{
    EditAccount *account = new EditAccount(this, user, "client", 0);
    account->set_db(Db);
    account->exec();
}
void Client::createToolBars()
{
    Edit = new QPushButton("Modifier votre profil");
    Back  = new QToolButton;
    Exit  = new QPushButton("Se Déconnecter");

    QLineEdit *empty = new QLineEdit[5];

    int i = 0;
    while (i < 5)
    {
        empty[i].setReadOnly(true);
        empty[i].setStyleSheet("border: transparent; background: transparent;");
        empty[i].setFixedSize(QSize(200, 25));
        i++;
    }
    Edit->setStyleSheet("border: 1px solid gray; color: #000000;background: transparent;border-radius: 4px;");
    Edit->setCursor(Qt::PointingHandCursor);
    Edit->setMaximumWidth(200);
    Edit->setFont(QFont("Gill Sans", 16));
    Edit->setCursor(Qt::PointingHandCursor);
    Exit->setStyleSheet("border: 1px solid gray; color: #000000;background: transparent;border-radius: 4px;");
    Exit->setCursor(Qt::PointingHandCursor);
    Exit->setMaximumWidth(200);
    Exit->setFont(QFont("Gill Sans", 16));
    Exit->setCursor(Qt::PointingHandCursor);
    Exit->setLayoutDirection(Qt::LeftToRight);
    Back->setCheckable(true);
    Back->setText("Accueil");
    Back->setFont(QFont("Gill Sans", 20));
    Back->setCursor(Qt::PointingHandCursor);
    Back->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    Back->setIcon(QIcon(":/icones/ressources/icons/home.png"));
    Back->setIconSize(QSize(200,200));
    Back->setFixedSize(200, 60);
    Back->setStyleSheet("border: 0px; color: #000000");

    Recherche = new QLineEdit;
    Recherche->setPlaceholderText("Rechercher une voiture (Modèle, Marque, Categorie...)");
    Recherche->resize(300, Recherche->height());
    Rech_Button = new QToolButton;
    Rech_Button->setToolTip("Rechercher");
    Rech_Button->setCursor(Qt::PointingHandCursor);
    Rech_Button->setIcon(QIcon(":/icones/ressources/icons/search.png"));
    Rech_Button->setIconSize(QSize(50,50));

    QToolBar *tool_up = addToolBar("Recherche");
    tool_up->addWidget(Recherche);
    tool_up->addWidget(Rech_Button);

    QToolButton *Profil = new QToolButton;
    Profil->setCheckable(true);
    Profil->setText(Db->get_nom(user, "client"));
    Profil->setFont(QFont("Gill Sans", 20));
    Profil->setCursor(Qt::PointingHandCursor);
    Profil->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    Profil->setIcon(QIcon(":/icones/ressources/icons/user.png"));
    Profil->setIconSize(QSize(200,200));
    Profil->setFixedSize(200, 60);
    Profil->setStyleSheet("border: 0px; color: #000000");
    QLineEdit *WithProfil = new QLineEdit(Db->get_prenom(user, "client"));
    WithProfil->setReadOnly(true);
    WithProfil->setStyleSheet("border: transparent; background: transparent;");
    WithProfil->setFont(QFont("Gill Sans", 20));
    WithProfil->setCursor(Qt::PointingHandCursor);
    WithProfil->setFixedSize(200, 25);
    WithProfil->setAlignment(Qt::AlignCenter);

    QToolBar *tool_right = addToolBar("Profil");
    tool_right->setFixedWidth(200);
    tool_right->addWidget(&empty[0]);
    tool_right->addWidget(Profil);
    tool_right->addWidget(WithProfil);
    tool_right->addWidget(&empty[1]);
    tool_right->addWidget(Edit);
    tool_right->addWidget(Exit);

    QProgressBar *fidel = new QProgressBar;
    fidel->setMaximum(FIDEL_MAX);
    fidel->setValue(Db->get_loyalty(user) % FIDEL_MAX);
    fidel->setFixedSize(QSize(195, 30));
    fidel->setStyleSheet(QString("QProgressBar { border: 2px solid grey; border-radius: 5px; text-align: center; }"+QString("QProgressBar::chunk { background: green; padding: 1px;  }")));
    QToolButton *fidelite = new QToolButton;
    fidelite->setCheckable(true);
    fidelite->setText("Fidélité");
    fidelite->setFont(QFont("Gill Sans", 20));
    fidelite->setCursor(Qt::PointingHandCursor);
    fidelite->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fidelite->setFixedSize(200, 30);
    fidelite->setStyleSheet("border: 0px; color: #000000");

    Notif = new QToolButton;
    Notif->setCheckable(true);
    Notif->setText("Notifications");
    Notif->setFont(QFont("Gill Sans", 20));
    Notif->setCursor(Qt::PointingHandCursor);
    Notif->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    Notif->setIcon(QIcon(":/icones/ressources/icons/notif.png"));
    Notif->setIconSize(QSize(200,200));
    Notif->setFixedSize(200, 80);
    Notif->setStyleSheet("border: 0px; color: #000000");
    ExRes = new QToolButton;
    ExRes->setCheckable(true);
    ExRes->setText("Ex-Réservations");
    ExRes->setFont(QFont("Gill Sans", 20));
    ExRes->setCursor(Qt::PointingHandCursor);
    ExRes->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ExRes->setIcon(QIcon(":/icones/ressources/icons/exres.png"));
    ExRes->setIconSize(QSize(200,200));
    ExRes->setFixedSize(200, 80);
    ExRes->setStyleSheet("border: 0px; color: #000000");

    QToolBar *tool_left = addToolBar("notif");
    tool_left->setFixedWidth(200);
    tool_left->addWidget(Back);
    //tool_left->addSeparator();
    tool_left->addWidget(&empty[2]);
    //tool_left->addSeparator();
    tool_left->addWidget(Notif);
    //tool_left->addSeparator();
    tool_left->addWidget(&empty[3]);
    //tool_left->addSeparator();
    tool_left->addWidget(ExRes);
    //tool_left->addSeparator();
    tool_left->addWidget(&empty[4]);
    //tool_left->addSeparator();
    tool_left->addWidget(fidel);
    tool_left->addWidget(fidelite);

    addToolBar(tool_up);
    addToolBar(Qt::RightToolBarArea, tool_right);
    addToolBar(Qt::LeftToolBarArea, tool_left);
}
QScrollArea *Client::createCentralWidget(int n_cars)
{
    int tmp_cars = n_cars;

    Button = new QToolButton[n_cars];
    int i = 0;

    QSqlQueryModel model, model2, model3;
    model2.setQuery("select matricule from voiture where id_marque in (select id_marque from marque)");
    QString nom_marque, nom_modele;
    while (i < n_cars)
    {
        model.setQuery("select nom_marque from marque where id_marque in (select id_marque from voiture where matricule='"+model2.record(i).value(0).toString()+"')");
        model3.setQuery("select nom_modele from modele where id_modele in (select id_modele from avoir where id_marque in (select id_marque from voiture where matricule='"+model2.record(i).value(0).toString()+"'))");
        nom_marque = model.record(0).value(0).toString();
        nom_modele = model3.record(0).value(0).toString();
        Button[i].setCheckable(true);
        //accessibe name is the matricule* of the car (primary key) :3
        Button[i].setAccessibleName(model2.record(i).value(0).toString());
        Button[i].setText(nom_marque+" "+nom_modele);
        Button[i].setFont(QFont("Angelina", 12));
        Button[i].setCursor(Qt::PointingHandCursor);
        Button[i].setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        Button[i].setIcon(QIcon(QCoreApplication::applicationDirPath() + "/car" + QString::number(i + 1) + ".jpg"));
        Button[i].setIconSize(QSize(800/5,533/5 + 12));
        //Button[i].setFixedSize(800/5, 533/5 + 15);
        Button[i].setStyleSheet("border: 0px; color: #000000");
        i++;
    }
    QGridLayout *Cars = new QGridLayout(this);
    n_cars = tmp_cars;
    i = 0;
    int j;
    while (n_cars != 0)
    {
        j = 0;
        while (j < 3 /*or 2 for only two columns*/ && n_cars != 0)
        {
            Cars->addWidget(Button + n_cars - 1, i,j);
            j++;
            n_cars--;
        }
        i++;
    }
    QWidget *scr = new QWidget;
    scr->setLayout(Cars);
    QScrollArea *Area  = new QScrollArea;
    Area->setBackgroundRole(QPalette::Dark);
    Area->setWidget(scr);
    return (Area);
}
