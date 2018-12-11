#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class EditWindow;
}

class EditWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = 0);
    ~EditWindow();

    void setInfo(QString id);
    QString getReason();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_ensure_clicked();

private:
    Ui::EditWindow *ui;

private:
    QSqlDatabase db;
    QString leaid;
    QString reason;
};

#endif // EDITWINDOW_H
