#include "addbook.h"
#include "ui_addbook.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>

AddBook::AddBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBook)
{
    ui->setupUi(this);
    ui->file_tbl->setColumnWidth(0,ui->file_tbl->width()*3);
}

AddBook::~AddBook()
{
    delete ui;
}

void AddBook::on_pushButton_clicked()
{

    fileList = QFileDialog::getOpenFileNames(this);
    qDebug()<<fileList<<fileList.count();
    ui->file_tbl->setRowCount(fileList.count());
    for(int i=0; i< fileList.count();++i)
    {
        QFile file;
        file.setFileName(fileList[i]);
        ui->file_tbl->setItem(i,0,new QTableWidgetItem(file.fileName()));
        ui->file_tbl->setItem(i,1,new QTableWidgetItem(QString::number((float)file.size()/1024,'f',2)+"Kb"));
    }

}

void AddBook::on_pushButton_2_clicked()
{
    int row=ui->file_tbl->currentRow();
    if(row>-1)
        ui->file_tbl->removeRow(row);
}

void AddBook::on_buttonBox_accepted()
{
    _book=new book;
    _book->author=ui->ln_author->text();
    _book->theme=ui->ln_theme->text();
    _book->tags=ui->ln_tags->text();
    _book->files=fileList;
    emit newBook(_book);
}
