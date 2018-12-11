#ifndef TEAWINDOW_H
#define TEAWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class TeaWindow;
}

class TeaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeaWindow(QWidget *parent = 0);
    ~TeaWindow();

    void setId(QString _id);

private slots:
    void on_pushButton_quit_clicked();

    void on_pushButton_quit_2_clicked();

    void on_pushButton_quit_3_clicked();

    void on_pushButton_agree_clicked();

    void on_pushButton_disagree_clicked();

    void on_lineEdit_type_editingFinished();

    void on_lineEdit_type2_editingFinished();

    void on_lineEdit_type3_editingFinished();

    void on_pushButton_shuaxin_clicked();

private:
    Ui::TeaWindow *ui;

private:
    QString teaid;
    QSqlDatabase db;
};

#endif // TEAWINDOW_H
