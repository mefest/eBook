#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>
#include <QAbstractButton>
#include "book.h"

namespace Ui {
class AddBook;
}

class AddBook : public QDialog
{
    Q_OBJECT

public:
    explicit AddBook(QWidget *parent = 0);
    ~AddBook();

signals:
    void newBook(book* _newBook);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_buttonBox_accepted();

private:
    book * _book;
    Ui::AddBook *ui;
    QStringList fileList;
};

#endif // ADDBOOK_H
