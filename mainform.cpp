#include "mainform.h"
#include "ui_mainform.h"
#include "sqlclient.h"
#include <QDebug>


MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);



    SqlClient sql;
    sql.getBooks();
    ui->bookView->setModel(sql.getBooks());
    ui->bookView->hideColumn(0);
}

MainForm::~MainForm()
{
    delete ui;
}



void MainForm::on_bookView_pressed(const QModelIndex &index)
{
    ui->bookView->selectRow(index.row());
}

void MainForm::on_bookView_clicked(const QModelIndex &index)
{
    ui->bookView->selectRow(index.row());
}

void MainForm::accept()
{
    qDebug()<<"add book";
    addBookForm->deleteLater();
}

void MainForm::cancelAddBook()
{
    qDebug()<<"cancel";
    addBookForm->deleteLater();
}

void MainForm::addBook(book *_newBook)
{
    newBook=_newBook;
    qDebug()<<newBook->author;
}
void MainForm::on_bt_addBook_clicked()
{
    addBookForm= new AddBook(this);
    connect(addBookForm,SIGNAL(accepted()),this,SLOT(accept()));
    connect(addBookForm,SIGNAL(newBook(book*)),this,SLOT(addBook(book*)));
    connect(addBookForm,SIGNAL(rejected()),this,SLOT(cancelAddBook()));
    addBookForm->show();
}
