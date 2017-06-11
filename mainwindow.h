#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QLabel>
#include <QListWidget>
#include <QDirModel>
#include "usermanager.h"
#include "logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void ReadyWrite(QString strContext);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_action_Quit_triggered();

    void on_action_P_triggered();

    void on_action_About_triggered();

    void on_action_WindowxXP_triggered();

    void on_action_Office2007_triggered();

    void on_action_U_triggered();

public slots:
    void updateNumber();

    void outputInfo(QString strContext);

private:
    void createStatusBar();

    void createDockWindows();

private:
    Ui::MainWindow *ui;
    QLabel *label;
    QListWidget *customerList;
    UserManager *userMgr;
    QDirModel *dirModel;
    Logger *logger;
};

#endif // MAINWINDOW_H
