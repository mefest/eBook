#include "addtag.h"
#include "ui_addtag.h"

AddTag::AddTag(SqlClient* sql, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTag)
{
    ui->setupUi(this);
    _sql=sql;
}

AddTag::~AddTag()
{
    delete ui;
}

void AddTag::on_buttonBox_accepted()
{
    _sql->addTag(ui->lineEdit->text());
    emit addedTag();
}
