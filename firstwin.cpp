#include "firstwin.h"

FirstWin::FirstWin()
{
    setWindowTitle("Gestion de location de voitures");
    init();
    Layout = new QVBoxLayout;
    Layout->addWidget(Employe);
    Layout->addWidget(Client);
    Layout->addWidget(Exit);

    setLayout(Layout);

    QPixmap bkgnd(":/icones/ressources/icons/background1337.jpg");
    bkgnd = bkgnd.scaled(QSize(WIDTH/2,HEIGHT/2), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    setPalette(palette);
    setWindowIcon(QIcon(":/icones/ressources/icons/management.png"));
    resize(QSize(WIDTH/2,HEIGHT/2));
    setFixedSize(QSize(WIDTH/2,HEIGHT/2));
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
    connect(Client, SIGNAL(clicked()), this, SLOT(ToEspaceClient()));
    connect(Employe, SIGNAL(clicked()), this, SLOT(ToEspaceEmploye()));
    connect(Exit, SIGNAL(clicked()), qApp, SLOT(quit()));

}
void clearWidgets(QLayout * layout) {
   if (! layout)
      return;
   while (auto item = layout->takeAt(0))
   {
      delete item->widget();
      clearWidgets(item->layout());
   }
}
void    FirstWin::ToEspaceClient()
{
    this->hide();
    EspaceClient *EC = new EspaceClient(this, this->path);
    EC->show();
}
void    FirstWin::ToEspaceEmploye()
{
    this->hide();
    EspaceEmploye *EE = new EspaceEmploye(this, this->path);
    EE->show();
}
void    FirstWin::set_path(QString p)
{
    path = p;
}
void    FirstWin::init()
{
    Client = new QToolButton;
    Employe = new QToolButton;
    Exit  = new QPushButton("Quitter");

    Exit->setFont(QFont("Verdana", 15));
    Exit->setIcon(QIcon(":/icones/ressources/icons/exit.png"));
    Exit->setIconSize(QSize(22,22));
    Exit->resize(QSize(50,10));
    Exit->setStyleSheet("border: 0px; color: #FFFFFF");
    Exit->setCursor(Qt::PointingHandCursor);

    Client->setFont(QFont("Gill Sans", 30));
    Client->setToolTip("Alt+C");
    Client->setShortcut(tr("Alt+C"));
    Client->setCursor(Qt::PointingHandCursor);
    Client->setText("Espace Clients");
    Client->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    Client->setIcon(QIcon(":/icones/ressources/icons/client.png"));
    Client->setIconSize(QSize(80,90));
    Client->setFixedSize(300, 150);
    Client->setStyleSheet("border: 0px; color: #FFFFFF");

    Employe->setFont(QFont("Gill Sans", 30));
    Employe->setToolTip("Alt+E");
    Employe->setShortcut(tr("Alt+E"));
    Employe->setCursor(Qt::PointingHandCursor);
    Employe->setText("Espace Employés");
    Employe->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    Employe->setIcon(QIcon(":/icones/ressources/icons/employe.png"));
    Employe->setIconSize(QSize(80,90));
    Employe->setFixedSize(300, 150);
    Employe->setStyleSheet("border: 0px; color: #FFFFFF");
}

