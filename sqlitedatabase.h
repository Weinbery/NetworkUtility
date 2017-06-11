#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QtSql>
#include <QSqlQueryModel>
#include <QTableView>
#include <QMessageBox>

static bool createSqliteConnection()
{
    //创建DB文件，创建表格
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("NetworkUtility.db");

    if (!db.open())
    {
        QMessageBox::critical(NULL, QObject::tr("失败"), QObject::tr("连接数据库失败!"));
        return false;
    }
    QSqlQuery query;
    bool bOk = query.exec("create table if not exists tbl_user(name varchar(24) PRIMARY KEY, password varchar(64))");
    if (!bOk)
    {
        QMessageBox::critical(NULL, QObject::tr("失败"), QObject::tr("创建用户注册表失败!"));
        return false;
    }
    bOk = query.exec("create table if not exists tbl_record(name varchar(24) PRIMARY KEY)");
    if (!bOk)
    {
        QMessageBox::critical(NULL, QObject::tr("失败"), QObject::tr("创建用户登录记录表失败!"));
        return false;
    }
    bOk = query.exec("create table if not exists tbl_logger(timestamp text, context text)");
    if (!bOk)
    {
        QMessageBox::critical(NULL, QObject::tr("失败"), QObject::tr("创建运行日志表失败!"));
        return false;
    }
    /// tbl_appconf 应用程序配置项，暂时不确定需要多少项
    /*
    bOk = query.exec("create table if not exists tbl_config(currentuser varchar(24),"
                     "background text,"
                     "other text)");
    if (!bOk)
    {
        QMessageBox::critical(NULL, QObject::tr("失败"), QObject::tr("创建运行日志表失败!"));
        return false;
    }
    */
    return true;
}

#endif // SQLITEDATABASE_H
