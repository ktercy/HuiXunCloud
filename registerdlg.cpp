#include "registerdlg.h"
#include "ui_registerdlg.h"
#include <Qt>
#include <QString>
#include <QFont>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDateTime>

RegisterDlg::RegisterDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDlg)
{
    ui->setupUi(this);
    ui->labReg->setAlignment(Qt::AlignHCenter);
    QFont font("Microsoft YaHei", 20, 75);
    ui->labReg->setFont(font);
    ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    ui->checPwdLineEdit->setEchoMode(QLineEdit::Password);
}

RegisterDlg::~RegisterDlg()
{
    delete ui;
}

void RegisterDlg::on_closeBtn_clicked()
{
    close();
}

void RegisterDlg::on_confirmBtn_clicked()   //没有控制用户名，密码，手机号的输入格式
{
    //从注册界面获取用户填写的各项注册信息
    QString userName = ui->userNameLineEdit->text().trimmed();
    QString userPhone = ui->userPhoneLineEdit->text().trimmed();
    QString pwd = ui->pwdLineEdit->text().trimmed();
    QString checPwd = ui->checPwdLineEdit->text().trimmed();

    //判断是否有用户没有填写的信息项
    if (userName.isEmpty() || userPhone.isEmpty()
            || pwd.isEmpty() || checPwd.isEmpty()) {
        QMessageBox::warning(this, tr("Waring"), tr("Please fill in all the information!"), QMessageBox::Yes);
        return;
    }

    //判断两次输入的密码是否一致
    if (pwd != checPwd) {
        QMessageBox::warning(this, tr("Waring"), tr("The passwords you typed don't match."), QMessageBox::Yes);
        return;
    }

    //判断用户输入的用户名和手机号已被注册
    //连接用户数据库（本地测试数据库）
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("mei2");
    db.setUserName("tangjun");
    db.setPassword("123456");
    if (!db.open()) {   //打开数据库，如果出错，则弹出警告窗口
       QMessageBox::warning(this, tr("Warning"), tr("Failed to connect database!"), QMessageBox::Yes);
       QSqlDatabase::removeDatabase("conLogIn");   //移除连接
       return;
    }

    QSqlQuery query(db);
    QString sql = QString("select us_id from user_info where us_name = '%1'").arg(userName);
    query.exec(sql);
    if (query.first()) {    //该用户名已被注册
        QMessageBox::warning(this, tr("Warning"), tr("User name existed!"), QMessageBox::Yes);
        return;
    }

    sql = QString("select us_id from user_info where us_phone = '%1'").arg(userPhone);
    query.exec(sql);
    if (query.first()) {    //电话号码已存在
        QMessageBox::warning(this, tr("Warning"), tr("Phone number existed!"), QMessageBox::Yes);
        return;
    }

    //注册信息都已填写完毕且正确，则将注册信息写入数据库
    QDateTime curDT = QDateTime::currentDateTime();
    sql = QString("insert into user_info(us_name, us_pwd, us_phone, us_rtime) value('%1', '%2', '%3', '%4')").arg(userName).arg(pwd).arg(userPhone).arg(curDT.toString("yyyy-MM-dd hh:mm:ss"));
    if (query.exec(sql)) {
        QMessageBox::information(this, tr("Infomation"), tr("Successful registration."), QMessageBox::Yes);
        close();
    }
}



