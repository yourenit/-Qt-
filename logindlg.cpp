#include "logindlg.h"
#include "login.h"

#include "ui_logindlg.h"

#include <QMessageBox>

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
}

LoginDlg::~LoginDlg()
{
    delete ui;
}

void LoginDlg::on_pushButton_login_clicked()
{
    QString id=ui->lineEdit_id->text();
    QString pwd=ui->lineEdit_password->text();
    QString type;
    if(ui->radioButton_stu->isChecked())type="stu";
    else if(ui->radioButton_tea->isChecked())type="tea";
    else if(ui->radioButton_lea->isChecked())type="lea";
    qDebug()<<type<<endl;
    Login login(id,pwd,type);
    if(login.isLogin()){
        //QWidget *widget=new QWidget();
        this->hide();
        if(type=="stu"){
            stu=new StuWindow();
            stu->setId(id);
            stu->show();
        }else if(type=="tea"){
            tea=new TeaWindow();
            tea->setId(id);
            tea->show();
        }else if(type=="lea"){
            qDebug()<<type<<endl;
            lea=new LeaWindow();
            lea->show();
        }
        //QMessageBox::warning(this,"提示","您已登录！",QMessageBox::Ok);

    }else{
        QMessageBox::warning(this,"提示","您的用户名或密码错误！",QMessageBox::Ok);
    }
}
