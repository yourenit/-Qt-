#ifndef LOGINDLG_H
#define LOGINDLG_H
#include "stuwindow.h"
#include "teawindow.h"
#include "leawindow.h"
#include <QDialog>

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::LoginDlg *ui;
    StuWindow *stu;
    TeaWindow *tea;
    LeaWindow *lea;

};

#endif // LOGINDLG_H
