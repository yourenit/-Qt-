#ifndef LEAWINDOW_H
#define LEAWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

namespace Ui {
class LeaWindow;
}

class LeaWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LeaWindow(QWidget *parent = 0);
    ~LeaWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_type_editingFinished();

    void on_pushButton_search_clicked();

private:
    Ui::LeaWindow *ui;

    QSqlDatabase db;
};

#endif // LEAWINDOW_H
