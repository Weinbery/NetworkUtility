#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QMessageBox>
#include <QtPrintSupport>
#include <QTimer>
#include <QPushButton>
//#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("网络调试助手 V1.00"));
    setWindowIcon(QIcon(tr(":/images/networkutility.png")));
    //
    createStatusBar();

    QLabel* agent = new QLabel(tr("agent"));
    agent->setToolTip(tr("Agent"));
    QPixmap pixmap(":/images/grey.png");
    agent->setPixmap(pixmap);
    ui->statusBar->addPermanentWidget(agent);

    QLabel* busRobot = new QLabel("busRobot");
    busRobot->setToolTip(tr("BusRobot"));
    QPixmap pixmap2(":/images/grey.png");
    busRobot->setPixmap(pixmap2);
    ui->statusBar->addPermanentWidget(busRobot);

    QLabel* server = new QLabel("server");
    server->setToolTip(tr("Server"));
    QPixmap pixmap3(":/images/grey.png");
    server->setPixmap(pixmap3);
    ui->statusBar->addPermanentWidget(server);

    //ui->statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}"));

    //QStringList strLists;
    //strLists<<"联机模式"<<"Agent:未知"<<"BusRobot:未知"<<"server:未知";
    createDockWindows();
    //
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateNumber()));
    timer->start(500);

    logger = new Logger(this);
    connect(this, SIGNAL(ReadyWrite(QString)), logger, SLOT(writeLog(QString)));
    connect(this, SIGNAL(ReadyWrite(QString)), this, SLOT(outputInfo(QString)));
    //
    //setStyleSheet("background-color:rgb(199, 237, 204)");
    //QApplication::setStyle(QStyleFactory::create("fusion")); //fusion windows
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton closeButton;
    closeButton = QMessageBox::question(this, tr("退出程序"),
                                   QString(tr("确认退出程序?")),
                                   QMessageBox::Yes | QMessageBox::No);
    //
    if (closeButton == QMessageBox::No)
    {
        event->ignore();  // 忽略退出信号，程序继续运行
    }
    else if (closeButton == QMessageBox::Yes)
    {
        event->accept();  // 接受退出信号，程序退出
    }
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("就绪"));
}
/**
 * @brief MainWindow::createDockWindows
 * text()	setText(const QString &text)	Qt::DisplayRole	条目显示的文本。
 * icon()	setIcon(const QIcon &icon)	Qt::DecorationRole	条目显示的图标。
 * statusTip()	setStatusTip(const QString &statusTip)	Qt::StatusTipRole	如果主界面有状态栏，鼠标悬停在该条目上时显示该状态信息到状态栏。
 * toolTip()	setToolTip(const QString &toolTip)	Qt::ToolTipRole	鼠标悬停在该条目上时显示的工具提示信息。
 * whatsThis()	setWhatsThis(const QString &whatsThis)	Qt::WhatsThisRole	如果主界面窗口标题栏有？帮助按钮，点击帮助按钮再点击该条目会显示该帮助信息。
 * font()	setFont(const QFont &font)	Qt::FontRole	显示条目文本用的字体。
 * textAlignment()	setTextAlignment(int alignment)	Qt::TextAlignmentRole	文本的对齐方式。
 * backgroundColor() setBackgroundColor(const QColor &color)	Qt::BackgroundColorRole	文本背景色。
 * textColor()	setTextColor(const QColor &color)	Qt::TextColorRole	文字颜色。
 * background()	setBackground(const QBrush &brush)	Qt::BackgroundRole	条目的背景画刷。
 * foreground()	setForeground(const QBrush &brush)	Qt::ForegroundRole	条目的前景画刷。
 * checkState()	setCheckState(Qt::CheckState state)	Qt::CheckStateRole	条目自带的复选框选中状态，可以是三态复选框。
 * sizeHint()	setSizeHint(const QSize &size)	Qt::SizeHintRole	条目显示的建议尺寸。
 */

void MainWindow::createDockWindows()
{
    QDockWidget *logWidget = new QDockWidget(tr("运行日志"), this);
    //dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    customerList = new QListWidget(logWidget);
    /// setFont(const QFont &font)
    QFont font("仿宋", 12, QFont::Normal);
    customerList->setFont(font);
    /*
    customerList->addItems(QStringList()
            << "John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"
            << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
            << "Tammy Shea, Tiblanka, 38 Sea Views, Carlton"
            << "Tim Sheen, Caraba Gifts, 48 Ocean Way, Deal"
            << "Sol Harvey, Chicos Coffee, 53 New Springs, Eccleston"
            << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
    */
    logWidget->setWidget(customerList);
    addDockWidget(Qt::BottomDockWidgetArea, logWidget);

    QDockWidget* modelWidget = new QDockWidget(tr("模型视图"), this);
    dirModel = new QDirModel();
    dirModel->setReadOnly(false);
    dirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
    ui->treeView->setModel(dirModel);
    QModelIndex index = dirModel->index("D:/");
    ui->treeView->expand(index);
    ui->treeView->scrollTo(index);
    ui->treeView->setCurrentIndex(index);
    ui->treeView->resizeColumnToContents(0);

    modelWidget->setWidget(ui->treeView);
    addDockWidget(Qt::LeftDockWidgetArea, modelWidget);

    ///
    QDockWidget* propertyWidget = new QDockWidget(tr("城市信息"), this);
    propertyWidget->setWidget(ui->tableView);
    addDockWidget(Qt::RightDockWidgetArea, propertyWidget);
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setQuery(QString("select * from City where ID < 50"));
    ui->tableView->setModel(model);

    QTableView *logView = new QTableView();
    QDockWidget* loglisWidget = new QDockWidget(tr("日志列表"), this);
    loglisWidget->setWidget(logView);
    addDockWidget(Qt::RightDockWidgetArea, loglisWidget);
    QSqlQueryModel *logModel = new QSqlQueryModel(logView);
    logModel->setQuery(QString("select * from tbl_logger"));
    logView->setModel(logModel);
}

void MainWindow::on_action_Quit_triggered()
{
    close();
}

void MainWindow::on_action_P_triggered()
{
    //QTextDocument *document = textEdit->document();
    QPrinter printer;

    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    statusBar()->showMessage(tr("准备打印..."));
    //document->print(&printer);
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this, tr("关于"),
             tr("<b>网络调试助手 V1.00</b> 主要用于开发人员进行网络程序调试。"
                "程序提供常规网络协议UDP/IP、TCP/IP数据收发功能。"));
}

void MainWindow::updateNumber()
{
    static int num = 0;
    ui->labelNumber->setText(QString::number(num));
    num++;
    QList<QLabel*> list = ui->statusBar->QStatusBar::findChildren<QLabel*>();
    for(int index = 0; index < list.size(); index++)
    {
        if(true)
        {
            if (num % 3 == 0)
            {
                QPixmap pixmap(":/images/grey.png");
                list.at(index)->setPixmap(pixmap);
            }
            else if (num % 3 == 1)
            {
                QPixmap pixmap(":/images/green.png");
                list.at(index)->setPixmap(pixmap);
            }
            else
            {
                QPixmap pixmap(":/images/red.png");
                list.at(index)->setPixmap(pixmap);
            }
        }
    }
}

void MainWindow::outputInfo(QString strContext)
{
    /// 时间戳
    QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strText = "[" + strTime + "] " + strContext;
    customerList->addItem(strText);
}

void MainWindow::on_action_WindowxXP_triggered()
{
    setStyleSheet("background-color:rgb(208, 221, 238)");
    QString strContext = "背景色变成rgb(208, 221, 238).";
    emit ReadyWrite(strContext);
}

void MainWindow::on_action_Office2007_triggered()
{
    setStyleSheet("background-color:rgb(160, 161, 255)");
    QString strContext = "背景色变成rgb(160, 161, 255).";
    emit ReadyWrite(strContext);
}

void MainWindow::on_action_U_triggered()
{
    QString strContext = "启动用户管理.";
    emit ReadyWrite(strContext);

    userMgr = new UserManager(this);
    userMgr->show();
}
