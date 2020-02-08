#ifndef FIRSTWIN_H
#define FIRSTWIN_H

#include "includes.h"
#include "dbmanager.h"
#include "espaceclient.h"
#include "espaceemploye.h"

class FirstWin : public QWidget
{
    Q_OBJECT
private slots:
    void    ToEspaceClient();
    void    ToEspaceEmploye();
public:
    FirstWin();
    void set_path(QString p);
    void init();
private:
    QString     path;
    QVBoxLayout *Layout;
    QToolButton *Client;
    QToolButton *Employe;
    QPushButton *Exit;
};

#endif // FIRSTWIN_H
