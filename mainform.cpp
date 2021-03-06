#include "mainform.h"
#include "ui_mainform.h"
#include "sqlclient.h"
#include <QDebug>


MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);


    sql=new SqlClient();
    ui->bookView->setModel(sql->getBooks());
    ui->bookView->hideColumn(0);
    _tags=sql->getTags();
    for (int i=0; i<_tags.count();++i)
    {
        ui->ln_tags->addItem(_tags.value(i).second);
    }

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
    int id=index.model()->index(index.row(),0).data().toInt();
    ui->fileView->setModel(sql->getFiles(id));
    ui->fileView->hideColumn(0);
    ui->fileView->hideColumn(1);
}

void MainForm::accept()
{
    qDebug()<<"add book";
    ui->ln_tags->clear();
    _tags=sql->getTags();
    for (int i=0; i<_tags.count();++i)
    {
        ui->ln_tags->addItem(_tags.value(i).second);
    }
    addBookForm->deleteLater();
}

void MainForm::cancelAddBook()
{
    qDebug()<<"cancel";
    ui->ln_tags->clear();
    _tags=sql->getTags();
    for (int i=0; i<_tags.count();++i)
    {
        ui->ln_tags->addItem(_tags.value(i).second);
    }
    addBookForm->deleteLater();
}

void MainForm::addBook(book *_newBook)
{
    newBook=_newBook;
    qDebug()<<newBook->author;
    sql->addBook(_newBook);
    ui->bookView->model()->deleteLater();
    ui->bookView->setModel(sql->getBooks());

}
void MainForm::on_bt_addBook_clicked()
{
    addBookForm= new AddBook(sql,this);
    connect(addBookForm,SIGNAL(accepted()),this,SLOT(accept()));
    connect(addBookForm,SIGNAL(newBook(book*)),this,SLOT(addBook(book*)));
    connect(addBookForm,SIGNAL(rejected()),this,SLOT(cancelAddBook()));
    addBookForm->show();
}


void MainForm::on_fileView_doubleClicked(const QModelIndex &index)
{
    int id=index.model()->index(index.row(),1).data().toInt();
    sql->getFile(id);
    sql->open(index.model()->index(index.row(),2).data().toString());
}

void MainForm::on_pushButton_clicked()
{
    sql->openExplorer();
}

void MainForm::on_pushButton_4_clicked()
{
    int row= ui->fileView->currentIndex().row();
    if(row>-1)
    {
        QString name=ui->fileView->model()->index(row,2).data().toString();
        sql->getFile(ui->fileView->model()->index(row,1).data().toInt());
        sql->open(name);
    }
}

void MainForm::on_bt_find_clicked()
{
    QString tags=ui->ln_tags->getDisplayText();
    QStringList tagList=tags.split(",");
    QList<int> indexTag;
    for (int i=0; i<tagList.count();++i)
    {
        for (int j=0; j<_tags.count();++j)
            if(tagList.at(i)==_tags.at(j).second)
                indexTag.append(_tags.at(j).first);
    }
    ui->bookView->model()->deleteLater();
    ui->bookView->setModel(sql->findFiles(indexTag,ui->ln_author->text(),ui->ln_theme->text()));
}
