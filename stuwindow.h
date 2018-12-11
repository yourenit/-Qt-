#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMainWindow>
#include <QDateTime>

namespace Ui {
class StuWindow;
}

class StuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StuWindow(QWidget *parent = 0);
    ~StuWindow();

private slots:
    void on_pushButton_quit_clicked();

    void on_pushButton_reset_clicked();

    void on_pushButton_quit2_clicked();

    void on_pushButton_apply_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_back_clicked();

    void on_pushButton_modify_clicked();

public:
    void setId(QString _id);
private:
    Ui::StuWindow *ui;
private:
    QSqlDatabase db;
    QString id;
    QString name;
    QString cla;
    QString xueyuan;
    QString reason;
    QDateTime beginTime;
    QDateTime backTime;
};

#endif // MAINWINDOW_H
