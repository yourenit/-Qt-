#include "stuwindow.h"
#include "ui_stuwindow.h"
#include "editwindow.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QMessageBox>

StuWindow::StuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StuWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window|Qt::FramelessWindowHint|Qt::WindowSystemMenuHint|Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint);
    this->resize(1050,700);
    this->setAttribute(Qt::WA_TranslucentBackground, true);//设置透明2-窗体标题栏不透明,背景透明
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应列宽
//    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自适应行高

    //设置tab_2页面操作Group布局
    QGridLayout *grid=new QGridLayout();
    grid->addWidget(ui->pushButton_modify,0,0);
    grid->addWidget(ui->pushButton_delete,1,0);
    grid->addWidget(ui->pushButton_back,2,0);
    grid->setSizeConstraint(QLayout::SetFixedSize);
    this->setLayout(grid);

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

    //初始化日期选择器
    QDateTime time=QDateTime::currentDateTime();    //获取系统当前时间
    ui->dateTimeEdit->setMinimumDateTime(time);
    ui->dateTimeEdit_2->setMinimumDateTime(time.addSecs(3600));
    ui->dateTimeEdit->setDateTime(time);
    ui->dateTimeEdit_2->setDateTime(time.addSecs(3600));

    //初始化审批老师下拉选项
    QSqlQuery query;
    query.exec("select * from teacher");
    while(query.next()){
        QString name=query.value("name").toString();
        ui->comboBox_tea->addItem(name);
    }
}

StuWindow::~StuWindow()
{
    if(db.isValid()){
        db.close();
    }
    delete ui;
}


void StuWindow::on_pushButton_quit_clicked()
{
    this->close();
}

void StuWindow::on_pushButton_reset_clicked()
{
    ui->lineEdit_class->clear();
    ui->textEdit_reson->clear();
}

void StuWindow::on_pushButton_quit2_clicked()
{
    this->close();
}

void StuWindow::setId(QString _id){
    //设置姓名，id
    this->id=_id;
    ui->lineEdit_id->setText(this->id);

    QSqlQuery query;
    query.prepare("select name,class from student where id=?");
    query.bindValue(0,this->id);
    query.exec();
    while(query.next()){
        ui->lineEdit_name->setText(query.value("name").toString());
        ui->lineEdit_class->setText(query.value("class").toString());
    }

    //初始化请假记录
    query.prepare("select * from leaTable where stuid=? order by leaid DESC limit 100");    //最新100条记录
    query.bindValue(0,this->id);
    query.exec();
    for(int i=0;query.next();i++){
        QString leaid=query.value("leaid").toString();
        QString reason=query.value("reason").toString();
        QDateTime lTime=QDateTime::fromString(query.value("leaTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString leaTime=lTime.toString("yyyy-MM-dd hh:mm");
        QDateTime bTime=QDateTime::fromString(query.value("backTime").toString(),"yyyy-MM-ddThh:mm:ss.zzz");
        QString backTime=bTime.toString("yyyy-MM-dd hh:mm");
        int s=query.value("status").toInt();
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(leaid));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(reason));
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);        //列宽自适应
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(leaTime+" ~ "+backTime));
        QString status;
        if(s==0){
            status="未审批...";
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(status));
            ui->tableWidget->item(i,3)->setForeground(QBrush(QColor(0,0,255)));
        }
        else if(s==1){
            status="已同意";
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(status));
            ui->tableWidget->item(i,3)->setForeground(QBrush(QColor(0,255,0)));
        }
        else if(s==2){
            status="已拒绝";
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(status));
            ui->tableWidget->item(i,3)->setForeground(QBrush(QColor(255,0,0)));
        }
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);            //表格不可编辑
    }
}

void StuWindow::on_pushButton_apply_clicked()
{
    //判断病假原因 与 班级是否为空
    if(ui->textEdit_reson->toPlainText()=="" || ui->lineEdit_class->text()==""){
        QMessageBox::warning(NULL,"提示","班级和病假原因不能为空！",QMessageBox::Ok);
        return;
    }
    this->name=ui->lineEdit_name->text();
    this->cla=ui->lineEdit_class->text();
    this->xueyuan=ui->comboBox_xueyuan->currentText();
    this->reason=ui->textEdit_reson->toPlainText();
    QString ltime=ui->dateTimeEdit->text();
    QString btime=ui->dateTimeEdit_2->text();
    QString teaName=ui->comboBox_tea->currentText();
    QString format="yyyy-MM-dd hh:mm";
    this->beginTime=QDateTime::fromString(ltime,format);
    this->backTime=QDateTime::fromString(btime,format);

    //申请操作
    QSqlQuery query1;
    query1.exec("select id from teacher where name='"+teaName+"'");
    query1.next();
    QString teaid=query1.value("id").toString();
    QSqlQuery query2;
    query2.prepare("insert into leaTable(stuid,teaid,leaTime,backTime,reason,status) values(?,?,?,?,?,0)");
    query2.bindValue(0,this->id);
    query2.bindValue(1,teaid);
    query2.bindValue(2,ltime);
    query2.bindValue(3,btime);
    query2.bindValue(4,this->reason);
    query2.exec();
    QMessageBox::warning(NULL,"提示","申请成功！请于请假记录页面进行查看、修改、删除！",QMessageBox::Ok);
    on_pushButton_reset_clicked();

    //更新申请记录
    QSqlQuery query3;
    query3.exec("select * from leaTable");
    query3.last();                           //返回最新的一条数据
    ui->tableWidget->insertRow(0);          //将新的一条申请插入到表格第一行
    ui->tableWidget->setItem(0,0,new QTableWidgetItem(query3.value("leaid").toString()));
    ui->tableWidget->setItem(0,1,new QTableWidgetItem(this->reason));
    ui->tableWidget->setItem(0,2,new QTableWidgetItem(ltime+" ~ "+btime));
    ui->tableWidget->setItem(0,3,new QTableWidgetItem("未审批..."));
    ui->tableWidget->item(0,3)->setForeground(QBrush(QColor(0,0,255)));
}

void StuWindow::on_pushButton_delete_clicked()
{
    QList<QTableWidgetItem*> items=ui->tableWidget->selectedItems();
    if(items.empty()){
        QMessageBox::warning(NULL,"错误","未选中数据行！",QMessageBox::Ok);
        return;
    }
    QString reason=items.at(1)->text();
    QString status=items.at(3)->text();
    if(status.compare("未审批...")==0){
        QMessageBox::critical(NULL,"错误","该申请未审核，不能进行删除!",QMessageBox::Ok);
        return;
    }else{
        int row=ui->tableWidget->currentItem()->row();      //获取当前行号
        ui->tableWidget->removeRow(row);
    }
}

void StuWindow::on_pushButton_back_clicked()
{
    QList<QTableWidgetItem*> items=ui->tableWidget->selectedItems();        //获取选中行的内容
    if(items.empty()){
        QMessageBox::critical(NULL,"错误","未选中数据行！",QMessageBox::Ok);
        return;
    }
    if(items.at(3)->text().compare("未审批...")!=0){
        QMessageBox::critical(NULL,"错误","该申请已审批，不能撤销！",QMessageBox::Ok);
        return;
    }
    QString leaid=items.at(0)->text();
    int row=ui->tableWidget->currentItem()->row();
    ui->tableWidget->removeRow(row);
    QSqlQuery query;
    query.prepare("delete from leaTable where leaid=?");
    query.bindValue(0,leaid);
    if(query.exec()){
        QMessageBox::warning(NULL,"提示","该申请已经撤销！",QMessageBox::Ok);
    }
}

void StuWindow::on_pushButton_modify_clicked()
{
    QList<QTableWidgetItem*> items=ui->tableWidget->selectedItems();        //获取选中行的内容
    if(items.empty()){
        QMessageBox::critical(NULL,"错误","未选中数据行！",QMessageBox::Ok);
        return;
    }
    QString id=items.at(0)->text();
    int row=ui->tableWidget->currentRow();
    EditWindow e;
    e.setInfo(id);
    e.exec();
    QString eReason=e.getReason();
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(eReason));
}
