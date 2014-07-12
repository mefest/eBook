#ifndef BOOK_H
#define BOOK_H
#include <QStringList>

struct book{
    QString author;
    QString theme;
    QList<int> tags;
    QStringList files;
};

#endif // BOOK_H
