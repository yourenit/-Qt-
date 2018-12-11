#include "disagreewin.h"
#include "ui_disagreewin.h"

DisAgreeWin::DisAgreeWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisAgreeWin)
{
    ui->setupUi(this);
}

DisAgreeWin::~DisAgreeWin()
{
    delete ui;
}

QString DisAgreeWin::getReason()
{
    return this->reason;
}

void DisAgreeWin::on_pushButton_cancel_clicked()
{
    this->close();
}

void DisAgreeWin::on_pushButton_ensure_clicked()
{
    this->reason=ui->textEdit_reason->toPlainText();
    this->close();
}
