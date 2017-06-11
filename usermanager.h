#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QDialog>
#include "logger.h"
#include "sqlitedatabase.h"
#include "registerdialog.h"

namespace Ui {
class UserManager;
}

class UserManager : public QDialog
{
    Q_OBJECT

public:
    explicit UserManager(QWidget *parent = 0);
    ~UserManager();

    void select();

signals:
    void ReadyWrite(QString strContext);

private slots:
    void on_pushButtonSearch_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonBack_clicked();

    void on_pushButtonAdd_clicked();

    void on_pushButtonUpdate_clicked();

private:
    Ui::UserManager *ui;
    Logger *logger;
};

#endif // USERMANAGER_H
