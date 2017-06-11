#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent, QString strUserName, QString strPassword) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("用户注册"));
    ui->lineEditUserName->setText(strUserName);
    ui->lineEditPassword->setText(strPassword);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

QString RegisterDialog::getUserName() const
{
    return m_strUserName;
}

QString RegisterDialog::getPassword() const
{
    return m_strPassword;
}

void RegisterDialog::on_pushButton_clicked()
{
    QString strUserName = ui->lineEditUserName->text();
    QString strPassword = ui->lineEditPassword->text();
    if (!strUserName.isEmpty() && (strPassword.length() >= 6 && strPassword.length() <= 14))
    {
        m_strUserName = strUserName;
        m_strPassword = strPassword;
        accept();
    }
    else
    {
        QMessageBox::warning(this, QObject::tr("错误"), QObject::tr("用户名或密码不合法。"));
    }
}

void RegisterDialog::on_pushButton_2_clicked()
{
    close();
}
