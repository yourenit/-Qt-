#ifndef LOGIN_H
#define LOGIN_H

#include <QString>
#include <QtSql>

class Login
{
public:
    Login();
    Login(QString _id,QString _password,QString _type);

public:
    bool isLogin();

private:
    QString id;
    QString password;
    QString type;
    QSqlDatabase db;
};

#endif // LOGIN_H
