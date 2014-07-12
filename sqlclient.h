#ifndef SQLCLIENT_H
#define SQLCLIENT_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>

class SqlClient
{
public:
    SqlClient();
    QSqlQueryModel* getBooks();

private:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // SQLCLIENT_H
