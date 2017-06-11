#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = 0, QString strUserName = "", QString strPassword = "");
    ~RegisterDialog();

    QString getUserName() const;

    QString getPassword() const;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::RegisterDialog *ui;
    QString m_strUserName;
    QString m_strPassword;
};

#endif // REGISTERDIALOG_H
