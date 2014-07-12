#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "addbook.h"
#include "book.h"

namespace Ui {
class MainForm;
}

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

private slots:

    void on_bookView_pressed(const QModelIndex &index);

    void on_bt_addBook_clicked();

    void on_bookView_clicked(const QModelIndex &index);

    void accept();
    void cancelAddBook();
    void addBook(book* _newBook);

private:
    Ui::MainForm *ui;
    AddBook *addBookForm;
    book *newBook;
};

#endif // MAINFORM_H
