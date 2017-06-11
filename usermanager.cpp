#include "usermanager.h"
#include "ui_usermanager.h"

UserManager::UserManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManager)
{
    ui->setupUi(this);
    setWindowTitle(tr("用户管理"));
    //
    logger = new Logger(this);
    connect(this, SIGNAL(ReadyWrite(QString)), logger, SLOT(writeLog(QString)));
    //
    select();
}

UserManager::~UserManager()
{
    delete ui;
}

void UserManager::select()
{
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setQuery(QString("select * from tbl_user"));
    ui->tableView->setModel(model);
}

void UserManager::on_pushButtonSearch_clicked()
{
    QSqlQuery query;
    ui ->tableView->clearSpans(); //tableview清空
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    query.prepare("select * from tbl_user where name = :name");  //like模糊查询没成功
    query.bindValue(":name", ui->lineEditSearchName->text());
    query.exec();
    model->setQuery(query);
    ui->tableView->setModel(model);
    //
    emit ReadyWrite(QString("查找用户：" + ui->lineEditSearchName->text()));
}

void UserManager::on_pushButtonDelete_clicked()
{
    QSqlQuery query;
    int curRow = ui->tableView->currentIndex().row();     // 鼠标选择删除第几行
    QModelIndex index = ui->tableView->currentIndex();
    QString userName = index.sibling(curRow, 0).data().toString();
    query.prepare("delete from tbl_user where name = :name");
    query.bindValue(":name", userName);
    query.exec();
    select();
    //
    emit ReadyWrite(QString("删除用户：" + userName));
}

void UserManager::on_pushButtonBack_clicked()
{
    select();
}

void UserManager::on_pushButtonAdd_clicked()
{
    RegisterDialog user(this);
    if (user.exec() == QDialog::Accepted)
    {
        QSqlQuery query;
        query.prepare("insert into tbl_user(name, password) values(:name, :password)");
        query.bindValue(":name", user.getUserName());
        query.bindValue(":password", user.getPassword());

        bool bOk = query.exec();
        if (!bOk)
        {
            QMessageBox::warning(this, "失败", "注册失败");
        }

        select();
        //
        emit ReadyWrite(QString("添加用户：" + user.getUserName()));
    }
    else
    {
        return ;
    }
}

void UserManager::on_pushButtonUpdate_clicked()
{
    QSqlQuery query;
    int curRow = ui->tableView->currentIndex().row();     // 鼠标选择删除第几行
    if (curRow < 0)
    {
        QMessageBox::information(this, tr("提醒"), tr("请用选择用户"));
        return;
    }
    QModelIndex index = ui->tableView->currentIndex();
    QString userName = index.sibling(curRow, 0).data().toString();
    QString userPassword = index.sibling(curRow, 1).data().toString();
    RegisterDialog user(this, userName, userPassword);
    if (user.exec() == QDialog::Accepted)
    {
        //update students set tel=default where id=5;
        query.prepare("update tbl_user set password = :password where name = :name");
        query.bindValue(":name", user.getUserName());
        query.bindValue(":password", user.getPassword());
        query.exec();
        //
        emit ReadyWrite(QString("修改用户：" + user.getUserName()));
    }

    select();
}
