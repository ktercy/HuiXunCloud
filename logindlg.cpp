#include "logindlg.h"
#include "registerdlg.h"
#include "mainwindow.h"
#include "ui_logindlg.h"
#include "meetsys.h"
#include <QString>
#include <QSqlDatabase> //连接数据库
#include <QSqlQuery>    //执行sql语句
#include <QDebug>
#include <QSqlError>    //sql语句错误
#include <QMessageBox>  //警告窗口

LogInDlg::LogInDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInDlg)
{
    ui->setupUi(this);

    ui->pwdLineEdit->setEchoMode(QLineEdit::Password); //隐藏输入的密码字符
    ui->userLineEdit->setPlaceholderText("Input user name");    //提示输入用户名
    ui->pwdLineEdit->setPlaceholderText("Input password");  //提示输入密码

    //设置默认用户名和密码，方便测试程序
    ui->userLineEdit->setText("唐俊");
    ui->pwdLineEdit->setText("123456");

    //为了进行connect，必须将choSysDlg,classSys和meetSys初始化
    choSysDlg = new ChoSysDlg(this);
    classSys = new MainWindow();    //因为classSys要在login窗口关闭后继续显示，所以不将其父窗口设为this（即login窗口）
    meetSys = new MeetSys();    //因为MeetSys要在login窗口关闭后继续显示，所以不将其父窗口设为this（即login窗口）
    connect(choSysDlg, SIGNAL(postRes(int)), this, SLOT(getRes(int)));
    connect(this, SIGNAL(posUsID(int)), classSys, SLOT(getUsID(int)));
    connect(this, SIGNAL(posUsID(int)), meetSys, SLOT(getUsID(int)));
}

LogInDlg::~LogInDlg()
{
    delete ui;
}

void LogInDlg::on_logInBtn_clicked()
{
    QString userName;
    QString pwd;
    userName = ui->userLineEdit->text().trimmed();  //获取输入的用户名
    pwd = ui->pwdLineEdit->text();  //获取输入的密码

    //判断用户名和密码是否为空,若为空，则弹出窗口警告
    if (userName == "") {
        QMessageBox::warning(this, tr("Warning"), tr("User name cann't be empty!"), QMessageBox::Yes);
        ui->userLineEdit->clear();
        ui->userLineEdit->setFocus();
        return;
    }else if (pwd == "") {
        QMessageBox::warning(this, tr("Warning"), tr("Password cann't be empty!"), QMessageBox::Yes);
        ui->pwdLineEdit->clear();
        ui->pwdLineEdit->setFocus();
        return;
    }

    //连接用户数据库（本地测试数据库）
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("mei2");
    db.setUserName("tangjun");
    db.setPassword("123456");
    if (!db.open()) {   //打开数据库，如果出错，则弹出警告窗口
       QMessageBox::warning(this, tr("Warning"), tr("Failed to connect database!"), QMessageBox::Yes);
       QSqlDatabase::removeDatabase(db.connectionName());   //移除连接
       return;
    }

    //查询数据库中此用户是否存在
    QString sql = QString("select us_pwd, us_id from user_info where us_name = '%1'").arg(userName);
    QSqlQuery query(db);
    query.exec(sql);
    if (!query.first()) {   //用户不存在
        qDebug() << query.lastError().text();
        QMessageBox::warning(this, tr("Warning"), tr("This user dosen't exist!"), QMessageBox::Yes);
        ui->userLineEdit->clear();
        ui->userLineEdit->setFocus();
    } else if (pwd != query.value(0).toString()) {  //用户存在但密码不正确
        QMessageBox::warning(this, tr("Warning"), tr("Incorrect password!"), QMessageBox::Yes);
        ui->pwdLineEdit->clear();
        ui->pwdLineEdit->setFocus();
    } else if (pwd == query.value(0).toString()) {  //用户存在且密码正确
//        accept();   //隐藏登陆界面，进入主界面
        userID = query.value(1).toInt();    //获取登录成功的用户的ID
        choSysDlg->exec();  //进入选择系统界面
        close();
    } else {    //其他未知错误
        QMessageBox::warning(this, tr("Warning"), tr("Unknown error!"), QMessageBox::Yes);
    }

    db.close(); //关闭连接
    QSqlDatabase::removeDatabase(db.connectionName());   //移除连接
}

void LogInDlg::on_exitBtn_clicked()
{
    reject(); //隐藏登录窗口
}

void LogInDlg::on_registerBtn_clicked()
{
    RegisterDlg *regDlg = new RegisterDlg(1, this);
    regDlg->exec();
}

void LogInDlg::getRes(int res){
    if (res == 1) { //用户选择课堂系统，则显示课堂系统界面
        emit posUsID(userID);
        classSys->show();
    }
    else if ( res == 2) {   //用户选择会议系统，则显示会议系统界面
        //显示会议系统
        emit posUsID(userID);
        meetSys->displayMeetings();
        meetSys->show();
        qDebug() << "show meet system window";
    }
}
