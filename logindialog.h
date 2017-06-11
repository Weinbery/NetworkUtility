#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "sqlitedatabase.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

    // 验证用户信息
    bool validateUserInfo(QString strUserName, QString strPassword);

private slots:
    void on_pushButtonLogin_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::LoginDialog *ui;
    quint8 m_nErrorCount;
    QStringListModel *model;
};

#endif // LOGINDIALOG_H
