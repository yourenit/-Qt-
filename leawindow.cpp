#include "leawindow.h"
#include "ui_leawindow.h"

#include <QSqlQuery>
#include <QDebug>

LeaWindow::LeaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LeaWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    this->resize(1050,700);
    this->setAttribute(Qt::WA_TranslucentBackground, true);//设置透明2-窗体标题栏不透明,背景透明
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应列宽
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应列宽
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);            //表格不可编辑
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);            //表格不可编辑
    //初始化数据库
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("196243587");
    db.setDatabaseName("leasystem");
    if(db.open()){
        qDebug()<<"数据库连接成功!"<<endl;
    }else{
        qDebug()<<"数据库连接失败!"<<endl;
    }

    //设置日期选择不可见，当下拉选项选择为请假时间时，下拉选项可见
//    ui->dateEdit->setVisible(false);
//    ui->label->setVisible(false);
//    ui->dateEdit_2->setVisible(false);
//    ui->pushButton_search->setVisible(false);

    //初始化日期选择器的日期
    ui->dateEdit_2->setDate(QDate::currentDate());
    ui->dateEdit->setDate(QDate::currentDate());

    //设置表格为整行选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);

    //初始化最近100条请假记录
    QSqlQuery query;
    query.exec("select l.*,s.*,t.name as tname from leaTable l join student s on s.id=stuid join teacher t on t.id=teaid where status=1 limit 100");
    while(query.next()){
        ui->tableWidget->insertRow(0);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(query.value("leaid").toString()));
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(query.value("name").toString()));
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(query.value("class").toString()));
        ui->tableWidget->setItem(0,3,new QTableWidgetItem(query.value("stuid").toString()));
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(query.value("xueyuan").toString()));
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(query.value("leaTime").toDateTime().toString("yyyy-MM-dd hh:mm")
                                                          +" ~ "
                                                          +query.value("backTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
        ui->tableWidget->setItem(0,6,new QTableWidgetItem(query.value("reason").toString()));
        ui->tableWidget->setItem(0,7,new QTableWidgetItem(query.value("handelTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
        ui->tableWidget->setItem(0,8,new QTableWidgetItem(query.value("tname").toString()));
    }
}

LeaWindow::~LeaWindow()
{
    delete ui;
}

void LeaWindow::on_pushButton_clicked()
{
    this->close();
}

void LeaWindow::on_pushButton_2_clicked()
{
    this->close();
}

void LeaWindow::on_lineEdit_type_editingFinished()
{
    QSqlQuery query;
    QString type=ui->comboBox_type->currentText();
    //获取是否选中时间范围查找
    QString ltime1="1970-1-1",ltime2="2099-12-31";
    if(ui->checkBox_time->isChecked()){
        ltime1=ui->dateEdit->text();
        ltime2=ui->dateEdit_2->text();
    }
    QString res=ui->lineEdit_type->text();
    if(type.compare("姓名")==0){
        query.exec("select l.*,s.*,t.name as tname from leaTable l join student s on s.id=stuid join teacher t on teaid=t.id where status=1 and s.name like '%"+res+"%'"
                   +"and l.leaTime>='"+ltime1+"' and l.leaTime<='"+ltime2+"'");
    }else if(type.compare("学号")==0){
        query.exec("select l.*,s.*,t.name as tname from leaTable l join student s on s.id=stuid join teacher t on teaid=t.id where status=1 and stuid="+res
                   +"and l.leaTime>='"+ltime1+"' and l.leaTime<='"+ltime2+"'");
    }else if(type.compare("学院名称")==0){
        query.exec("select l.*,s.*,t.name as tname from leaTable l join student s on s.id=stuid join teacher t on teaid=t.id where status=1 and s.xueyuan like '%"+res+"%'"
                   +"and l.leaTime>='"+ltime1+"' and l.leaTime<='"+ltime2+"'");
    }else if(type.compare("班级")==0){
        query.exec("select l.*,s.*,t.name as tname from leaTable l join student s on s.id=stuid join teacher t on teaid=t.id where status=1 and class like '%"+res+"%'"
                   +"and l.leaTime>='"+ltime1+"' and l.leaTime<='"+ltime2+"'");
    }else if(type.compare("审批人")==0){
        query.exec("select l.*,s.*,t.name as tname from leaTable l join student s on s.id=stuid join teacher t on teaid=t.id where status=1 and t.name like '%"+res+"%'"
                   +"and l.leaTime>='"+ltime1+"' and l.leaTime<='"+ltime2+"'");
    }


    //清除原来数据
    int count=ui->tableWidget_2->rowCount();
    for(int i=0;i<count;i++){
        ui->tableWidget_2->removeRow(0);
    }

    while(query.next()){
        ui->tableWidget_2->insertRow(0);
        ui->tableWidget_2->setItem(0,0,new QTableWidgetItem(query.value("leaid").toString()));
        ui->tableWidget_2->setItem(0,1,new QTableWidgetItem(query.value("name").toString()));
        ui->tableWidget_2->setItem(0,2,new QTableWidgetItem(query.value("class").toString()));
        ui->tableWidget_2->setItem(0,3,new QTableWidgetItem(query.value("stuid").toString()));
        ui->tableWidget_2->setItem(0,4,new QTableWidgetItem(query.value("xueyuan").toString()));
        ui->tableWidget_2->setItem(0,5,new QTableWidgetItem(query.value("leaTime").toDateTime().toString("yyyy-MM-dd hh:mm")
                                                          +" ~ "
                                                          +query.value("backTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
        ui->tableWidget_2->setItem(0,6,new QTableWidgetItem(query.value("reason").toString()));
        ui->tableWidget_2->setItem(0,7,new QTableWidgetItem(query.value("handelTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
        ui->tableWidget_2->setItem(0,8,new QTableWidgetItem(query.value("tname").toString()));
    }
}

void LeaWindow::on_pushButton_search_clicked()
{
    on_lineEdit_type_editingFinished();
}
