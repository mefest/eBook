#include "sqlclient.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

SqlClient::SqlClient()
{
    db =QSqlDatabase::addDatabase( "QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("eBook");
    db.setUserName("root");
    db.setPassword("270391");
    bool ok = db.open();
    qDebug()<<ok;
    if (!ok)
    {
        QMessageBox::critical(0, db.lastError().driverText(),
                              db.lastError().text());
    }
}

QSqlQueryModel *SqlClient::getBooks()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT id, author, theme, rating FROM book");
    return model;
}
