#include "editwindow.h"
#include "ui_editwindow.h"

#include <QSqlQuery>
#include <QDebug>

EditWindow::EditWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    //初始化数据库
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("leasystem");
    db.setUserName("root");
    db.setPassword("196243587");

    bool ok=db.open();
    if(ok){
        qDebug()<<"数据库连接成功！"<<endl;
    }else{
        qDebug()<<"数据库连接失败！"<<endl;
    }
}

EditWindow::~EditWindow()
{
    delete ui;
}

void EditWindow::setInfo(QString id)
{
    this->leaid=id;
    QSqlQuery query;
    query.prepare("select reason,leaTime,backTime from leaTable where leaid=?");
    query.bindValue(0,id);
    query.exec();
    while(query.next()){
        QString reason=query.value("reason").toString();
        QDateTime ltime=query.value("leaTime").toDateTime();
        QDateTime btime=query.value("backTime").toDateTime();
        //QString leaTime=query.value("leaTime").toDateTime().toString("yyyy-MM-dd hh:mm");
        //QString backTime=query.value("backTime").toDateTime().toString("yyyy-MM-dd hh:mm");
        ui->textEdit_reason->setPlainText(reason);
        ui->dateTimeEdit->setDateTime(ltime);
        ui->dateTimeEdit_2->setDateTime(btime);
    }

}

QString EditWindow::getReason()
{
    return this->reason;
}

void EditWindow::on_pushButton_2_clicked()
{
    this->close();
}

void EditWindow::on_pushButton_ensure_clicked()
{
    this->reason=ui->textEdit_reason->toPlainText();
    QSqlQuery query;
    query.prepare("update leaTable set reason=?,leaTime=?,backTime=? where leaid=?");
    query.bindValue(0,ui->textEdit_reason->toPlainText());
    query.bindValue(1,ui->dateTimeEdit->text());
    query.bindValue(2,ui->dateTimeEdit_2->text());
    query.bindValue(3,this->leaid);
    query.exec();
    this->close();
}
