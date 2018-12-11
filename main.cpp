#include "stuwindow.h"
#include "teawindow.h"
#include "leawindow.h"
#include "logindlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //LoginWindow login;
    //login.show();
    LoginDlg login;
    login.exec();
    //StuWindow stu;
    //stu.show();
    //TeaWindow tea;
    //tea.show();
    //LeaWindow lea;
    //lea.show();
    return a.exec();
}
