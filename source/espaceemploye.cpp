#include <espaceemploye.h>
#include <employe.h>

EspaceEmploye::EspaceEmploye(QWidget *parent, QString p)
{
    this->parent = parent;
    path = p;
    Login = new QLineEdit;
    Psswd = new QLineEdit;
    conn  = new QPushButton("Se connecter");
    Back  = new QPushButton("Accueil");
    Exit  = new QPushButton("Quitter");
    Db    = new DbManager(path);

    conn->setStyleSheet("color: #FFFFFF;background: gray;border-radius: 4px;");
    conn->setCursor(Qt::PointingHandCursor);
    conn->setMaximumWidth(350);
    conn->setFont(QFont("Gill Sans", 20));
    conn->setCursor(Qt::PointingHandCursor);
    conn->setDefault(true);
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

    Psswd->setEchoMode(QLineEdit::Password);

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
    this->setWindowTitle("Espace Employés");
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
    QObject::connect(Back, SIGNAL(clicked()), this, SLOT(accueil()));
}
void EspaceEmploye::accueil()
{
    parent->show();
    this->close_me();
}
EspaceEmploye::~EspaceEmploye()
{
    Db->close_db();
}
void EspaceEmploye::close_me()
{
    delete Psswd;
    delete Back;
    delete Exit;
    delete conn;
    Db->close_db();
    delete Db;
    this->close();
}
void EspaceEmploye::se_connecter()
{
    if (Db->connect(Login->text(), Psswd->text(), "employe"))
    {
        this->hide();
        Employe *Em = new Employe(this, Login->text(), path);
        Em->set_db(Db);
        Em->show();
    }
    else
        QMessageBox::critical(this, "Erreur", "nom d'utilisateur ou mot de passe incorrect !");}
