#include "mainwindow.h"
#include "logindialog.h"
#include "sqlitedatabase.h"
#include <QApplication>
#include <QStyleFactory>

void initializeModel(QSqlQueryModel *model)
{
    model->setQuery("select * from tbl_logger");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("时间"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("日志"));
}

QTableView* createView(QSqlQueryModel *model, QWidget* parent,const QString &title = "")
{
    QTableView *view = new QTableView();
    view->setModel(model);
    static int offset = 0;

    view->setWindowTitle(title);
    view->move(100 + offset, 100 + offset);
    offset += 20;
    view->show();

    return view;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!createSqliteConnection())
    {
        return -1;
    }

    MainWindow w;
    LoginDialog login;
/*
    QSqlQueryModel plainModel;
    initializeModel(&plainModel);
    createView(&plainModel, &w, QObject::tr("日志列表"));
*/
    QApplication::setStyle(QStyleFactory::create("fusion")); //fusion windows

    if (login.exec() == QDialog::Accepted)
    {
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
