#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("用户登录"));
    setWindowIcon(QIcon(tr(":/images/networkutility.png")));

    QPixmap pixmap(":/images/user.png");
    ui->labelLogo->setPixmap(pixmap);

    //ui->lineEditUserName->setText(tr("Administrator"));
    //ui->lineEditPassword->setText(tr("Weinbery"));

    m_nErrorCount = 0;
    //setStyleSheet("background-color:rgb(199, 237, 204)");
    model = new QStringListModel(this);
    QStringList userList;
    QSqlQuery query;
    query.exec("select * from tbl_record");
    while (query.next())
    {
        QString strName = query.value(0).toString();
        userList << strName;
    }
    model->setStringList(userList);
    ui->comboBoxUserName->setModel(model);
}

/// 验证用户名和密码
LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::validateUserInfo(const QString strUserName, const QString strPassword)
{
    bool bPass = false;
    //
    QSqlQuery query;
    query.exec("select * from tbl_user");
    while (query.next())
    {
        QString strName = query.value(0).toString();
        QString strPasswd = query.value(1).toString();
        int nUserNameCheckResukt = QString::compare(strUserName, strName, Qt::CaseInsensitive);
        int nPasswordCheckResult = QString::compare(strPassword, strPasswd, Qt::CaseSensitive);
        if (nUserNameCheckResukt == 0 && nPasswordCheckResult == 0)
        {
            bPass = true;
            break;
        }
    }

    return bPass;
}

void LoginDialog::on_pushButtonLogin_clicked()
{
    QString strUserName = ui->comboBoxUserName->currentText();
    QString strPassword = ui->lineEditPassword->text();
    //
    //int nUserNameCheckResukt = QString::compare(strUserName, "Administrator", Qt::CaseInsensitive);
    //int nPasswordCheckResult = QString::compare(strPassword, "Weinbery", Qt::CaseSensitive);
    //
    if (validateUserInfo(strUserName, strPassword))
    {
        QSqlQuery query;
        query.prepare("insert into tbl_record(name) values(:name)");
        query.bindValue(":name", strUserName);
        query.exec();
        //
        accept();
    }
    else
    {
        if (++m_nErrorCount < 5)
        {
            QMessageBox::warning(this, tr("提醒"), tr("用户名或密码错误，请检查重新输入。"));
            ui->lineEditPassword->clear();
        }
        else
        {
            QMessageBox::information(this, tr("信息"), tr("您输入的用户名或密码错误次数已达5次。"));
            close();
        }
    }
}

void LoginDialog::on_pushButtonCancel_clicked()
{
    close();
}
