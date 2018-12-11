#include "login.h"
#include <QSqlQuery>

Login::Login(){}

Login::Login(QString _id,QString _password,QString _type){
    this->id=_id;
    this->password=_password;
    this->type=_type;
    this->db=QSqlDatabase::addDatabase("QMYSQL");
    this->db.setHostName("localhost");
    this->db.setPort(3306);
    this->db.setDatabaseName("leasystem");
    this->db.setUserName("root");
    this->db.setPassword("196243587");

    bool ok=db.open();
    if(ok){
        qDebug()<<"数据库连接成功！"<<endl;
    }else qDebug()<<"数据库连接失败！"<<endl;
}

bool Login::isLogin()
{
    QSqlQuery query;
    if(this->type.compare("stu")==0){
        query.prepare("select password from student where id=?");
    }else if(this->type.compare("tea")==0){
        query.prepare("select password from teacher where id=?");
    }else if(this->type.compare("lea")==0){
        query.prepare("select password from leader where id=?");
    }
    query.bindValue(0,this->id);
    query.exec();
    if(query.next()){
        if(query.value("password").toString().compare(this->password)==0)return true;
        else return false;
    }
    return false;
}
