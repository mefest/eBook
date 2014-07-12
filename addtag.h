#ifndef ADDTAG_H
#define ADDTAG_H

#include <QDialog>
#include "sqlclient.h"

namespace Ui {
class AddTag;
}

class AddTag : public QDialog
{
    Q_OBJECT

public:
    explicit AddTag(SqlClient* sql, QWidget *parent = 0);
    ~AddTag();

signals:
    void addedTag();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddTag *ui;
    SqlClient* _sql;
};

#endif // ADDTAG_H
