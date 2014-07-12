#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T18:23:59
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = eBook
TEMPLATE = app


SOURCES += main.cpp\
        mainform.cpp \
    sqlclient.cpp \
    addbook.cpp \
    multilist.cpp

HEADERS  += mainform.h \
    sqlclient.h \
    addbook.h \
    book.h \
    multilist.h

FORMS    += mainform.ui \
    addbook.ui
