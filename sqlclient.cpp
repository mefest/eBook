#include "sqlclient.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QFile>
#include <QSqlRecord>
#include <QFileInfo>
#include <QProcess>
#include <QDir>

SqlClient::SqlClient()
{
    currentBook=0;
    db =QSqlDatabase::addDatabase( "QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("eBook");
    db.setUserName("root");
    db.setPassword("270391");
    bool ok = db.open();
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

QSqlQueryModel *SqlClient::getFiles(int idBook)
{
    //    clearDir();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT `id`, `idbook`, `fileName` FROM `data` WHERE `idbook`="+QString::number(idBook));
    return model;
}

void SqlClient::getFile(int id)
{

    if(currentBook!=id)
    {
        clearDir();
        currentBook=id;

        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT `data`, `fileName` FROM `data` WHERE `idbook`="+QString::number(id));
        for(int i=0; i<model->rowCount();++i)
        {
            QByteArray buf;
            QString fileName=model->index(i,1).data().toString();
            buf= model->index(i,0).data().toByteArray();
            buf=qUncompress(buf);
            QFile file;
            file.setFileName("temp/"+fileName);
            file.open(QIODevice::ReadWrite);
            file.write(buf);
            file.close();
        }
    }
}

QList<QPair<int, QString> > SqlClient::getTags()
{
    QSqlQuery query;
    QList< QPair<int,QString> > tagsList;
    query.exec("SELECT * FROM tag ORDER BY name;");
    while(query.next())
    {
        QPair<int ,QString> tag(query.value("id").toInt(),query.value("name").toString());
        tagsList.append(tag);
    }
    return tagsList;
}

void SqlClient::addTag(QString name)
{
    QSqlQuery query;
    query.exec("INSERT INTO `eBook`.`tag` (`name`) VALUES ('"+name+"')");
}

void SqlClient::addBook(book *_book)
{
    QSqlQuery query;
    query.exec("INSERT INTO `eBook`.`book` (`author`, `theme`)"
               "VALUES ('"+_book->author+"', '"+_book->theme+"')");
    int bookId=query.lastInsertId().toInt();
    for (int i=0;i<_book->tags.count();++i)
    {
        query.exec("INSERT INTO `eBook`.`link` (`idtag`, `idbook`)"
                   "VALUES ('"+QString::number(_book->tags.at(i))+"', '"+QString::number(bookId)+"')");
    }

    for(int i=0;i<_book->files.count();++i)
    {
        QByteArray buf;
        QFile file;
        file.setFileName(_book->files[i]);
        file.open(QIODevice::ReadOnly);
        buf=file.readAll();
        buf=qCompress(buf,6);
        QFileInfo fileInfo;
        fileInfo.setFile(file);
        file.close();
        query.prepare("INSERT INTO `eBook`.`data` (`idbook`, `fileName`, `data`)"
                      "VALUES (:id, :name, :data)");
        query.bindValue(":id",bookId);
        query.bindValue(":name",fileInfo.fileName());
        query.bindValue(":data",buf);
        query.exec();
    }
}

void SqlClient::clearDir()
{
    QDir dir;
    dir.setCurrent("temp/");
    QStringList fileLst=dir.entryList(QDir::NoDotAndDotDot|QDir::Files);
    for(int i=0; i<fileLst.count();++i)
    {
        QFile file;
        file.setFileName(fileLst[i]);
        file.remove();
    }
    dir.setCurrent("..");
}

void SqlClient::openExplorer()
{
    QProcess sub;
    sub.start("gnome-open temp/");
    sub.waitForFinished(-1);
}

void SqlClient::open(QString name)
{
    QProcess sub;
    sub.start("gnome-open temp/"+name);
    sub.waitForFinished(-1);
}
