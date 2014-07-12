#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>
#include <QAbstractButton>
#include "addtag.h"
#include "book.h"
#include "sqlclient.h"

namespace Ui {
class AddBook;
}
class AddBook : public QDialog
{
    Q_OBJECT

public:
    explicit AddBook(SqlClient *sql,QWidget *parent = 0);
    ~AddBook();

signals:
    void newBook(book* _newBook);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_buttonBox_accepted();

    void on_pushButton_3_clicked();
    void tagsChanged();

private:
    book * _book;
    Ui::AddBook *ui;
    QStringList fileList;
    QStringList tagsList;
    SqlClient *_sql;
    AddTag *winTag;
    QList< QPair<int,QString> > _tags;
};

#endif // ADDBOOK_H
