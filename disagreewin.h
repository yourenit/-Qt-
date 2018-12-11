#ifndef DISAGREEWIN_H
#define DISAGREEWIN_H

#include <QDialog>

namespace Ui {
class DisAgreeWin;
}

class DisAgreeWin : public QDialog
{
    Q_OBJECT

public:
    explicit DisAgreeWin(QWidget *parent = 0);
    ~DisAgreeWin();

    QString getReason();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ensure_clicked();

private:
    Ui::DisAgreeWin *ui;
    QString reason;
};

#endif // DISAGREEWIN_H
