#include <espaceclient.h>
#include <newaccount.h>
#include <firstwin.h>
#include <client.h>

EspaceClient::EspaceClient(QWidget *parent, QString p)
{
    this->parent = parent;
    path = p;
    Login = new QLineEdit;
    Psswd = new QLineEdit;
    Back  = new QPushButton("Accueil");
    Exit  = new QPushButton("Quitter");
    conn  = new QPushButton("Se connecter");
    New   = new QPushButton("Créer un compte ?");
    Db    = new DbManager(path);

    Psswd->setEchoMode(QLineEdit::Password);
    New->setStyleSheet("color: #FFFFFF;background: gray;border-radius: 4px;");
    New->setCursor(Qt::PointingHandCursor);
    New->setMaximumWidth(350);
    New->setFont(QFont("Gill Sans", 20));
    New->setCursor(Qt::PointingHandCursor);
    conn->setStyleSheet("color: #FFFFFF;background: gray;border-radius: 4px;");
    conn->setCursor(Qt::PointingHandCursor);
    conn->setMaximumWidth(350);
    conn->setFont(QFont("Gill Sans", 20));
    conn->setCursor(Qt::PointingHandCursor);
    Exit->setStyleSheet("color: #FFFFFF;background: gray;border-radius: 4px;");
    Exit->setCursor(Qt::PointingHandCursor);
    Exit->setMaximumWidth(350);
    Exit->setFont(QFont("Gill Sans", 16));
    Exit->setCursor(Qt::PointingHandCursor);
    Back->setStyleSheet("color: #FFFFFF;background: gray;border-radius: 4px;");
    Back->setCursor(Qt::PointingHandCursor);
    Back->setMaximumWidth(350);
    Back->setFont(QFont("Gill Sans", 16));
    Back->setCursor(Qt::PointingHandCursor);

    Login->setFont(QFont("Gill Sans", 24));
    Psswd->setFont(QFont("Gill Sans", 24));
    Login->setMaximumWidth(300);
    Psswd->setMaximumWidth(300);
    Login->setPlaceholderText("Login");
    Psswd->setPlaceholderText("Mot de passe");
    Login->setStyleSheet("border: 1px solid gray;background: transparent; color: #F7EFFA; font-size: 20");
    Psswd->setStyleSheet("border: 1px solid gray;background: transparent; color: #F7EFFA; font-size: 20");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(Login);
    layout->addWidget(Psswd);
    layout->addWidget(conn);
    layout->addWidget(New);
    layout->addWidget(Back);
    layout->addWidget(Exit);
    layout->setAlignment(Qt::AlignCenter);

    QGroupBox   *page_group = new QGroupBox("Connexion");
    page_group->setFont(QFont("Gill Sans", 30));
    page_group->setStyleSheet("color : #FFFFFF;");
    page_group->setLayout(layout);
    page_group->setAlignment(Qt::AlignCenter);

    QVBoxLayout *principal_layout = new QVBoxLayout;

    QLabel *image = new QLabel(parent);
    image->setPixmap(QPixmap(":/icones/ressources/icons/login.png"));
    image->setAlignment(Qt::AlignCenter);

    principal_layout->addWidget(image);
    principal_layout->addWidget(page_group);

    setFixedSize(QSize(WIDTH/2,1200/2));
    setWindowTitle("Espace Clients");
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

    QPixmap bkgnd(":/icones/ressources/icons/login_bd.jpg");
    bkgnd = bkgnd.scaled(QSize(WIDTH/2,1200/2), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    setPalette(palette);

    principal_layout->setAlignment(Qt::AlignCenter);
    setLayout(principal_layout);
    QObject::connect(Exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(conn, SIGNAL(clicked()), this, SLOT(se_connecter()));
    QObject::connect(Psswd, SIGNAL(returnPressed()), this, SLOT(se_connecter()));
    QObject::connect(New, SIGNAL(clicked()), this, SLOT(new_account()));
    QObject::connect(Back, SIGNAL(clicked()), this, SLOT(accueil()));
}
EspaceClient::~EspaceClient()
{
    Db->close_db();
}
void EspaceClient::accueil()
{
    parent->show();
    this->close_me();
}
void EspaceClient::close_me()
{
    delete Login;
    delete Psswd;
    delete Back;
    delete Exit;
    delete conn;
    delete New;
    Db->close_db();
    delete Db;
    this->close();
}
void EspaceClient::se_connecter()
{
    if (Db->connect(Login->text(), Psswd->text(), "client"))
    {
        this->hide();
        Client *Em = new Client(this, Login->text());
        Em->set_db(Db);
        Em->show();
    }
    else
        QMessageBox::critical(this, "Erreur", "nom d'utilisateur ou mot de passe incorrect !");
}
void EspaceClient::new_account()
{
    NewAccount *account = new NewAccount(this);
    account->set_path(this->path);
    account->exec();
}
