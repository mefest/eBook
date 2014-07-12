#ifndef SQLCLIENT_H
#define SQLCLIENT_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <book.h>

class SqlClient
{
public:
    SqlClient();
    QSqlQueryModel* getBooks();
    QSqlQueryModel* getFiles(int idBook);
    void getFile(int id);
    QList< QPair<int,QString> > getTags();
    void addTag(QString name);
    void addBook(book* _book);
    void clearDir();
    void openExplorer();
    void open(QString name);
    int currentBook;

private:
    QSqlDatabase db;

};

#endif // SQLCLIENT_H
