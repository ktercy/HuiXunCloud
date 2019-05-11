#include "bookmeet.h"
#include "ui_bookmeet.h"
#include <QDateTime>
#include <QDebug>
#include <cmath>    //使用round()函数
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QTableWidgetItem>

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

    ui->twMeetings->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); //根据内容自动设置列宽

    //显示正在召开的会议、将要召开的会议、已经取消的会议
    QString curDT = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");  //以字符串保存当前时间
    sql = QString("select meet_id, app_u_id, meet_title, meet_num, start_time, end_time, operate from meet_app where app_r_id = %1  and end_time > '%2'").arg(meetRoomID).arg(curDT);
    query.exec(sql);
    while (query.next()) {
        //给tableWidget添加一行，以显示会议概览
        int rowCount = ui->twMeetings->rowCount();
        ui->twMeetings->setRowCount(rowCount+1);
        int rowIndex = ui->twMeetings->rowCount() - 1;  //新增行的索引

        QSqlQuery query2(db);
        query2.exec(QString("select us_name from user_info where us_id = %1").arg(query.value(1).toInt()));
        query2.first();

        //定义这一行所拥有的单元格，并初始化单元格的内容
        QTableWidgetItem *twiUser = new QTableWidgetItem(query2.value(0).toString());    //申请人
        QTableWidgetItem *twiTitle = new QTableWidgetItem(query.value(2).toString());   //会议主题
        QTableWidgetItem *twiNum = new QTableWidgetItem(query.value(3).toString()); //参会人数
        QTableWidgetItem *twiSTime = new QTableWidgetItem(query.value(4).toString());   //开始时间
        QTableWidgetItem *twiETime = new QTableWidgetItem(query.value(5).toString());   //结束时间

        //设置这一行所有单元格的文本对齐格式为上下居中和左右居中
        twiUser->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiTitle->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiNum->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiSTime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        twiETime->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        if (query.value(4).toString() <= curDT ) { //当该会议开始时间小于等于当前时间时（即该会议正在召开），将背景色改为红色
            twiUser->setBackgroundColor(QColor(251,174, 148));
            twiTitle->setBackgroundColor(QColor(251,174, 148));
            twiNum->setBackgroundColor(QColor(251,174, 148));
            twiSTime->setBackgroundColor(QColor(251,174, 148));
            twiETime->setBackgroundColor(QColor(251,174, 148));
        } else if (query.value(4).toString() > curDT ) {   //当该会议开始时间大于于当前时间时（即该会议即将召开），将背景色改为蓝色
            twiUser->setBackgroundColor(QColor(213, 243, 244));
            twiTitle->setBackgroundColor(QColor(213, 243, 244));
            twiNum->setBackgroundColor(QColor(213, 243, 244));
            twiSTime->setBackgroundColor(QColor(213, 243, 244));
            twiETime->setBackgroundColor(QColor(213, 243, 244));
        }

        if (query.value(6).toInt() == 0) {   //当operate字段值为0时（即该会议被取消），将文字变为斜体，且加上删除线
            QFont font;
            font.setStrikeOut(true);
            font.setItalic(true);
            twiUser->setFont(font);
            twiTitle->setFont(font);
            twiNum->setFont(font);
            twiSTime->setFont(font);
            twiETime->setFont(font);
        }

        //向添加的行中填写数据
        ui->twMeetings->setItem(rowIndex, 0, twiUser);  //向新增行中写入会议申请人
        ui->twMeetings->setItem(rowIndex, 1, twiTitle);  //向新增行中写入会议主题
        ui->twMeetings->setItem(rowIndex, 2, twiNum);  //向新增行中写入参会人数
        ui->twMeetings->setItem(rowIndex, 3, twiSTime);  //向新增行中写入会议开始时间
        ui->twMeetings->setItem(rowIndex, 4, twiETime);  //向新增行中写入会议结束时间
    }


    db.close();
    QSqlDatabase::removeDatabase(db.connectionName());
}
