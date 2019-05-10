#include "bookmeet.h"
#include "ui_bookmeet.h"
#include <QDateTime>
#include <QDebug>
#include <cmath>    //使用round()函数
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

BookMeet::BookMeet(int uID, int mrID, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookMeet),
    userID(uID), meetRoomID(mrID)
{
    ui->setupUi(this);

    setDisplay();

    //关联信号和槽，使得每当用户改变会议开始时间或会议结束时间，客户端自动得出会议时长
    connect(ui->dteStart, SIGNAL(dateTimeChanged(const QDateTime)), this, SLOT(setMeetDura()));
    connect(ui->dteEnd, SIGNAL(dateTimeChanged(const QDateTime)), this, SLOT(setMeetDura()));
}

BookMeet::~BookMeet()
{
    delete ui;
}

void BookMeet::setMeetDura()
{
    QDateTime start = ui->dteStart->dateTime();
    QDateTime end = ui->dteEnd->dateTime();
    double temp = start.secsTo(end) / 3600.0; //获取到从会议开始到会议结束的小时数
    temp = round(temp * 100) / 100; //四舍五入保留两位小数

    if (temp < 0) {
        QMessageBox::warning(this, tr("warning"), tr("The end time must be greater than the start time."));
    }else if (temp > 4.0) {
        QMessageBox::warning(this, tr("warning"), tr("The length of the meeting should not exceed 4 hours."));
    }else {
        meetDura = temp;
        ui->labMeetDura->setText(QString::number(meetDura, 'f', 2));
    }

}

void BookMeet::setDisplay()
{
    //将提示信息的文本颜色设置为红色
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->labNote->setPalette(pa);

    //设置默认的会议开始时间和会议结束时间都为当前时间
    ui->dteStart->setDateTime(QDateTime::currentDateTime());
    ui->dteEnd->setDateTime(QDateTime::currentDateTime());

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

    qDebug() << "meetRoomID = " << meetRoomID;

    //查询并设置该会议室的最大人员容纳量
    QString sql = QString("select meetroom_num from meet_room where meetroom_id = %1").arg(meetRoomID);
    QSqlQuery query(db);
    query.exec(sql);

    if (!query.first()) {
        QMessageBox::warning(this, tr("warning"), tr("The meeting room was not found."));
    } else {
        ui->sbMeetNum->setMaximum(query.value(0).toInt());
        ui->sbMeetNum->setValue(query.value(0).toInt());
    }

    db.close();
    QSqlDatabase::removeDatabase(db.connectionName());
}
