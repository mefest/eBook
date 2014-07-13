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
    model->setQuery("SELECT `id`, `idbook`, `fileName`,`size` FROM `file` WHERE `idbook`="+QString::number(idBook));
    return model;
}

QSqlQueryModel *SqlClient::findFiles(QList<int> idTags, QString author, QString theme)
{
    QSqlQuery query;
    QStringList idBooks;
    for (int i=0; i<idTags.count();++i)
    {
        query.exec("SELECT idbook FROM link WHERE idtag IN("+QString::number(idTags.at(i))+")");
        while (query.next())
            idBooks.append(query.value(0).toString());

    }
    QSqlQueryModel *model = new QSqlQueryModel();
    QString str;
    if(idTags.count()==0)
        str="SELECT book.id, author, theme, rating FROM book LEFT JOIN link on book.id=link.idbook"
                " WHERE author LIKE '%"+author+"%'  AND theme LIKE '%"+theme+"%'";
    else
        str="SELECT book.id, author, theme, rating FROM book LEFT JOIN link on book.id=link.idbook"
                " WHERE author LIKE '%"+author+"%'  AND theme LIKE '%"+theme+"%' AND book.id IN ("+idBooks.join(",")+")";
    model->setQuery(str);
    return model;
}

void SqlClient::getFile(int id)
{

    if(currentBook!=id)
    {
        clearDir();
        currentBook=id;
        QSqlQuery query;
        query.exec("SELECT `fileName`, `id` FROM `file` WHERE idbook="+QString::number(currentBook));
        while(query.next())
        {
            QString fileName=query.value(0).toString();
            QSqlQueryModel *model = new QSqlQueryModel();
            model->setQuery("SELECT  `data`, `part` FROM `partFile`"
                            " WHERE `idFile`="+query.value(1).toString()+" ORDER BY `part`");
            QByteArray buf;
            for(int i=0; i<model->rowCount();++i)
            {
                buf+= model->index(i,0).data().toByteArray();
            }
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
        QByteArray buf2;
        QFile file;
        file.setFileName(_book->files[i]);
        file.open(QIODevice::ReadOnly);
        buf=file.readAll();
        float size=(float)buf.size()/1000;
        buf=qCompress(buf,9);
        QFileInfo fileInfo;
        fileInfo.setFile(file);
        file.close();
        query.prepare("INSERT INTO `eBook`.`file` (`idbook`, `fileName`, `size`)"
                      "VALUES (:id, :name, :size)");
        query.bindValue(":id",bookId);
        query.bindValue(":name",fileInfo.fileName());
        query.bindValue(":size",size);
        query.exec();
        int fileId=query.lastInsertId().toInt();
        int lastSize=buf.size();
        while (lastSize>10000000)
            lastSize-=10000000;
        int count=buf.size()/10000000;
        count++;
        for (int i=0; i<count;++i)
        {
            if (i==count-1)
                buf2=buf.mid(i*10000000,lastSize);
            else
                buf2=buf.mid(i*10000000,10000000);
            query.prepare("INSERT INTO `eBook`.`partFile` (`idFile`, `part`, `data`)"
                          " VALUES (:id, :part , :data)");
            query.bindValue(":id",fileId);
            query.bindValue(":part",i);
            query.bindValue(":data",buf2);
            query.exec();
        }
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
    sub.start("gnome-open \"temp/"+name+"\"");
    sub.waitForFinished(-1);
}
