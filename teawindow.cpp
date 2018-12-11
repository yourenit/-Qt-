#include "teawindow.h"
#include "ui_teawindow.h"
#include "disagreewin.h"

#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

TeaWindow::TeaWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TeaWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    this->resize(1050,700);
    this->setAttribute(Qt::WA_TranslucentBackground, true);//设置透明2-窗体标题栏不透明,背景透明
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

    //设置表格为整行选择
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);

}

TeaWindow::~TeaWindow()
{
    delete ui;
}

void TeaWindow::setId(QString _id)
{
    this->teaid=_id;

    //初始化申请信息
    QSqlQuery query;
    query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=0 order by leaid desc limit 50"); //显示最新50条数据
    for(int i=0;query.next();i++){
        QString leaid=query.value("leaid").toString();
        QString stuid=query.value("stuid").toString();
        QString stuName=query.value("name").toString();
        QString stuClass=query.value("class").toString();
        QString reason=query.value("reason").toString();
        QDateTime lTime=QDateTime::fromString(query.value("leaTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString leaTime=lTime.toString("yyyy-MM-dd hh:mm");
        QDateTime bTime=QDateTime::fromString(query.value("backTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString backTime=bTime.toString("yyyy-MM-dd hh:mm");

        ui->tableWidget->insertRow(i);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(leaid));
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(stuName));
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(stuid));
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(stuClass));
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(reason));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(leaTime+" ~ "+backTime));
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);            //表格不可编辑
    }

    //初始化同意列表
    query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=1 order by leaid desc limit 50");    //显示最新50条记录
    for(int i=0;query.next();i++){
        QString leaid=query.value("leaid").toString();
        QString stuid=query.value("stuid").toString();
        QString stuName=query.value("name").toString();
        QString stuClass=query.value("class").toString();
        QString reason=query.value("reason").toString();
        QDateTime lTime=QDateTime::fromString(query.value("leaTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString leaTime=lTime.toString("yyyy-MM-dd hh:mm");
        QDateTime bTime=QDateTime::fromString(query.value("backTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString backTime=bTime.toString("yyyy-MM-dd hh:mm");
        QString hdTime=query.value("handelTime").toDateTime().toString("yyyy-MM-dd hh:mm");

        ui->tableWidget_2->insertRow(i);
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_2->setItem(i,0,new QTableWidgetItem(leaid));
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_2->setItem(i,1,new QTableWidgetItem(stuName));
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_2->setItem(i,2,new QTableWidgetItem(stuid));
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_2->setItem(i,3,new QTableWidgetItem(stuClass));
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_2->setItem(i,4,new QTableWidgetItem(reason));
        ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_2->setItem(i,5,new QTableWidgetItem(leaTime+" ~ "+backTime));
        ui->tableWidget_2->setItem(i,6,new QTableWidgetItem(hdTime));
        ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);            //表格不可编辑
    }
    //初始化拒绝列表
    query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=2 order by leaid desc limit 50");    //显示最新50条记录
    for(int i=0;query.next();i++){
        QString leaid=query.value("leaid").toString();
        QString stuid=query.value("stuid").toString();
        QString stuName=query.value("name").toString();
        QString stuClass=query.value("class").toString();
        QString reason=query.value("reason").toString();
        QDateTime lTime=QDateTime::fromString(query.value("leaTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString leaTime=lTime.toString("yyyy-MM-dd hh:mm");
        QDateTime bTime=QDateTime::fromString(query.value("backTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString backTime=bTime.toString("yyyy-MM-dd hh:mm");
        QString hdTime=query.value("handelTime").toDateTime().toString("yyyy-MM-dd hh:mm");
        QString DReason=query.value("noReason").toString();

        ui->tableWidget_3->insertRow(i);
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_3->setItem(i,0,new QTableWidgetItem(leaid));
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_3->setItem(i,1,new QTableWidgetItem(stuName));
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_3->setItem(i,2,new QTableWidgetItem(stuid));
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_3->setItem(i,3,new QTableWidgetItem(stuClass));
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_3->setItem(i,4,new QTableWidgetItem(reason));
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_3->setItem(i,5,new QTableWidgetItem(leaTime+" ~ "+backTime));
        ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget_3->setItem(i,6,new QTableWidgetItem(DReason));
        ui->tableWidget_3->setItem(i,7,new QTableWidgetItem(hdTime));
        ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);            //表格不可编辑
    }
}

void TeaWindow::on_pushButton_quit_clicked()
{
    this->close();
}

void TeaWindow::on_pushButton_quit_2_clicked()
{
    this->close();
}

void TeaWindow::on_pushButton_quit_3_clicked()
{
    this->close();
}

void TeaWindow::on_pushButton_agree_clicked()
{
    QList<QTableWidgetItem*> items=ui->tableWidget->selectedItems();
    if(items.empty()){
        QMessageBox::warning(NULL,"提示","未选中数据行！",QMessageBox::Ok);
        return;
    }
    QString leaid=items.at(0)->text();
    QSqlQuery query;
    QString handelTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    query.exec("update leaTable set status=1,handelTime='"+handelTime+"' where leaid="+leaid);
    int row=ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);

    //更新同意列表
    query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where leaid="+leaid);
    query.next();
    ui->tableWidget_2->insertRow(0);
    ui->tableWidget_2->setItem(0,0,new QTableWidgetItem(leaid));
    ui->tableWidget_2->setItem(0,1,new QTableWidgetItem(query.value("name").toString()));
    ui->tableWidget_2->setItem(0,2,new QTableWidgetItem(query.value("stuid").toString()));
    ui->tableWidget_2->setItem(0,3,new QTableWidgetItem(query.value("class").toString()));
    ui->tableWidget_2->setItem(0,4,new QTableWidgetItem(query.value("reason").toString()));
    ui->tableWidget_2->setItem(0,5,new QTableWidgetItem(query.value("leaTime").toDateTime().toString("yyyy-MM-dd hh:mm")
                                                        +" ~ "+
                                                        query.value("backTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
    ui->tableWidget_2->setItem(0,6,new QTableWidgetItem(handelTime));

}

void TeaWindow::on_pushButton_disagree_clicked()
{
    QList<QTableWidgetItem*> items=ui->tableWidget->selectedItems();
    if(items.empty()){
        QMessageBox::warning(NULL,"提示","未选中数据行！",QMessageBox::Ok);
        return;
    }
    DisAgreeWin d;
    d.exec();
    QString DReason=d.getReason();
    QString leaid=items.at(0)->text();
    QSqlQuery query;
    QString handelTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    query.exec("update leaTable set status=2,noReason='"+DReason+"',handelTime='"+handelTime+"' where leaid="+leaid);
    int row=ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);

    //更新拒绝列表
    query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where leaid="+leaid);
    query.next();
    ui->tableWidget_3->insertRow(0);
    ui->tableWidget_3->setItem(0,0,new QTableWidgetItem(leaid));
    ui->tableWidget_3->setItem(0,1,new QTableWidgetItem(query.value("name").toString()));
    ui->tableWidget_3->setItem(0,2,new QTableWidgetItem(query.value("stuid").toString()));
    ui->tableWidget_3->setItem(0,3,new QTableWidgetItem(query.value("class").toString()));
    ui->tableWidget_3->setItem(0,4,new QTableWidgetItem(query.value("reason").toString()));
    ui->tableWidget_3->setItem(0,5,new QTableWidgetItem(query.value("leaTime").toDateTime().toString("yyyy-MM-dd hh:mm")
                                                        +" ~ "+
                                                        query.value("backTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
    ui->tableWidget_3->setItem(0,6,new QTableWidgetItem(DReason));
    ui->tableWidget_3->setItem(0,7,new QTableWidgetItem(handelTime));
}

void TeaWindow::on_lineEdit_type_editingFinished()
{
    QSqlQuery query;
    QString stuName;
    QString stuId;
    QString stuClass;
    if(ui->lineEdit_type->text().compare("")==0){
        return;
    }
    if(ui->comboBox_type->currentText().compare("姓名")==0){
        stuName=ui->lineEdit_type->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=0 and name like '%"+stuName+"%'");
    }else if(ui->comboBox_type->currentText().compare("学号")==0){
        stuId=ui->lineEdit_type->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=0 and id="+stuId);
    }else if(ui->comboBox_type->currentText().compare("班级")==0){
        stuClass=ui->lineEdit_type->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=0 and class="+stuClass);
    }

    //清除原来数据
    int count=ui->tableWidget->rowCount();
    for(int i=0;i<count;i++){
        ui->tableWidget->removeRow(0);
    }
    //显示查找到的数据
    while(query.next()){
        ui->tableWidget->insertRow(0);
        ui->tableWidget->setItem(0,0,new QTableWidgetItem(query.value("leaid").toString()));
        ui->tableWidget->setItem(0,1,new QTableWidgetItem(query.value("name").toString()));
        ui->tableWidget->setItem(0,2,new QTableWidgetItem(query.value("stuid").toString()));
        ui->tableWidget->setItem(0,3,new QTableWidgetItem(query.value("class").toString()));
        ui->tableWidget->setItem(0,4,new QTableWidgetItem(query.value("reason").toString()));
        ui->tableWidget->setItem(0,5,new QTableWidgetItem(query.value("leaTime").toDateTime().toString("yyyy-MM-dd hh:mm")
                                                            +" ~ "+
                                                            query.value("backTime").toDateTime().toString("yyyy-MM-dd hh:mm")));

    }

}

void TeaWindow::on_lineEdit_type2_editingFinished()
{
    QSqlQuery query;
    QString stuName;
    QString stuId;
    QString stuClass;
    if(ui->lineEdit_type2->text().compare("")==0){
        return;
    }
    if(ui->comboBox_type2->currentText().compare("姓名")==0){
        stuName=ui->lineEdit_type2->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=1 and name like '%"+stuName+"%'");
    }else if(ui->comboBox_type2->currentText().compare("学号")==0){
        stuId=ui->lineEdit_type2->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=1 and id="+stuId);
    }else if(ui->comboBox_type2->currentText().compare("班级")==0){
        stuClass=ui->lineEdit_type2->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=1 and class="+stuClass);
    }

    //清除原来数据
    int count=ui->tableWidget_2->rowCount();
    for(int i=0;i<count;i++){
        ui->tableWidget_2->removeRow(0);
    }
    //显示查找到的数据
    while(query.next()){
        ui->tableWidget_2->insertRow(0);
        ui->tableWidget_2->setItem(0,0,new QTableWidgetItem(query.value("leaid").toString()));
        ui->tableWidget_2->setItem(0,1,new QTableWidgetItem(query.value("name").toString()));
        ui->tableWidget_2->setItem(0,2,new QTableWidgetItem(query.value("stuid").toString()));
        ui->tableWidget_2->setItem(0,3,new QTableWidgetItem(query.value("class").toString()));
        ui->tableWidget_2->setItem(0,4,new QTableWidgetItem(query.value("reason").toString()));
        ui->tableWidget_2->setItem(0,5,new QTableWidgetItem(query.value("leaTime").toDateTime().toString("yyyy-MM-dd hh:mm")
                                                            +" ~ "+
                                                            query.value("backTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
        ui->tableWidget_2->setItem(0,6,new QTableWidgetItem(query.value("handelTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
    }
}

void TeaWindow::on_lineEdit_type3_editingFinished()
{
    QSqlQuery query;
    QString stuName;
    QString stuId;
    QString stuClass;
    if(ui->lineEdit_type3->text().compare("")==0){
        return;
    }
    if(ui->comboBox_type3->currentText().compare("姓名")==0){
        stuName=ui->lineEdit_type3->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=2 and name like '%"+stuName+"%'");
    }else if(ui->comboBox_type3->currentText().compare("学号")==0){
        stuId=ui->lineEdit_type3->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=2 and id="+stuId);
    }else if(ui->comboBox_type3->currentText().compare("班级")==0){
        stuClass=ui->lineEdit_type3->text();
        query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=2 and class="+stuClass);
    }

    //清除原来数据
    int count=ui->tableWidget_3->rowCount();
    for(int i=0;i<count;i++){
        ui->tableWidget_3->removeRow(0);
    }
    //显示查找到的数据
    while(query.next()){
        ui->tableWidget_3->insertRow(0);
        ui->tableWidget_3->setItem(0,0,new QTableWidgetItem(query.value("leaid").toString()));
        ui->tableWidget_3->setItem(0,1,new QTableWidgetItem(query.value("name").toString()));
        ui->tableWidget_3->setItem(0,2,new QTableWidgetItem(query.value("stuid").toString()));
        ui->tableWidget_3->setItem(0,3,new QTableWidgetItem(query.value("class").toString()));
        ui->tableWidget_3->setItem(0,4,new QTableWidgetItem(query.value("reason").toString()));
        ui->tableWidget_3->setItem(0,5,new QTableWidgetItem(query.value("leaTime").toDateTime().toString("yyyy-MM-dd hh:mm")
                                                            +" ~ "+
                                                            query.value("backTime").toDateTime().toString("yyyy-MM-dd hh:mm")));
        ui->tableWidget_3->setItem(0,6,new QTableWidgetItem(query.value("noReason").toString()));
        ui->tableWidget_3->setItem(0,7,new QTableWidgetItem(query.value("handelTime").toString()));
    }
}

void TeaWindow::on_pushButton_shuaxin_clicked()
{
    //清除原来数据
    int count=ui->tableWidget->rowCount();
    for(int i=0;i<count;i++){
        ui->tableWidget->removeRow(0);
    }

    QSqlQuery query;
    query.exec("select l.*,s.* from leaTable l join student s on l.stuid=s.id where teaid="+this->teaid+" and status=0 order by leaid desc limit 50"); //显示最新50条数据
    for(int i=0;query.next();i++){
        QString leaid=query.value("leaid").toString();
        QString stuid=query.value("stuid").toString();
        QString stuName=query.value("name").toString();
        QString stuClass=query.value("class").toString();
        QString reason=query.value("reason").toString();
        QDateTime lTime=QDateTime::fromString(query.value("leaTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString leaTime=lTime.toString("yyyy-MM-dd hh:mm");
        QDateTime bTime=QDateTime::fromString(query.value("backTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString backTime=bTime.toString("yyyy-MM-dd hh:mm");

        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(leaid));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(stuName));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(stuid));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(stuClass));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(reason));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(leaTime+" ~ "+backTime));
    }
}
