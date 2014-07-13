#include "addbook.h"
#include "ui_addbook.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>

AddBook::AddBook(SqlClient *sql, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBook)
{
    ui->setupUi(this);
    winTag=new AddTag(sql,this);
    connect(winTag,SIGNAL(addedTag()),this,SLOT(tagsChanged()));
    ui->file_tbl->setColumnWidth(0,ui->file_tbl->width()*3);
    _tags= sql->getTags();
    for (int i=0; i<_tags.count();++i)
    {
        ui->ln_tags->addItem(_tags.value(i).second);
    }

}

AddBook::~AddBook()
{
    delete ui;
    delete winTag;
}

void AddBook::on_pushButton_clicked()
{

    fileList = QFileDialog::getOpenFileNames(this);
    qDebug()<<fileList<<fileList.count();
    ui->file_tbl->setRowCount(fileList.count());
    for(int i=0; i< fileList.count();++i)
    {
        QFileInfo info;
        info.setFile(fileList[i]);
        ui->file_tbl->setItem(i,0,new QTableWidgetItem(info.fileName()));
        ui->file_tbl->setItem(i,1,new QTableWidgetItem(QString::number((float)info.size()/1024,'f',2)+"Kb"));
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
    if(ui->ln_tags->getDisplayText()=="")
    {
       QMessageBox::warning(this,"Ошибка! ","Проверьте заполненость полей",
                                    QMessageBox::Ok);
       return;
    }
    _book=new book;
    _book->author=ui->ln_author->text();
    _book->theme=ui->ln_theme->text();
    QString tags=ui->ln_tags->getDisplayText();
    QStringList tagList=tags.split(",");
    QList<int> indexTag;
    for (int i=0; i<tagList.count();++i)
    {
        for (int j=0; j<_tags.count();++j)
            if(tagList.at(i)==_tags.at(j).second)
                indexTag.append(_tags.at(j).first);
    }
    _book->tags=indexTag;
    _book->files=fileList;
    emit newBook(_book);
}

void AddBook::on_pushButton_3_clicked()
{
    winTag->show();
}

void AddBook::tagsChanged()
{
    _tags= _sql->getTags();
    ui->ln_tags->clear();
    for (int i=0; i<_tags.count();++i)
    {
        ui->ln_tags->addItem(_tags.value(i).second);
    }
}
