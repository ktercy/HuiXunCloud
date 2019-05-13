#include "meetsys.h"
#include "ui_meetsys.h"
#include "lwibomeet.h"
#include "lwimeeting.h"
#include <QSqlDatabase> //连接数据库
#include <QSqlQuery>    //执行sql语句
#include <QDebug>
#include <QSqlError>    //sql语句错误
#include <QMessageBox>  //警告窗口
#include <QStringList>  //存储会议室名称
#include <QListWidgetItem>
#include <QDateTime>

MeetSys::MeetSys(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MeetSys)
{
    ui->setupUi(this);

    ui->twPerCen->expandAll();
}

MeetSys::~MeetSys()
{
    delete ui;
}

void MeetSys::displayMeetings()
{
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

    //从数据库中查询所有会议室的名称和地址
    QString sql = QString("select meetroom_id, meetroom_name, meetroom_addr from meet_room");
    QSqlQuery query(db);
    query.exec(sql);

    if (query.first()){
        //将所有会议室到名称和地址地址放到QListWidget中去
        for (int i = 0; i < query.size(); i++, query.next()){
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(QSize(500,60));
            if(i%2 == 0) {  //每隔一条会议室信息，将背景色设置为灰色，方便用户辨认
                item->setBackgroundColor(QColor(240, 240, 240));
            }
            ui->lwBoMeet->addItem(item);
            lwiBoMeet *boMeetItem = new lwiBoMeet(ui->lwBoMeet);
            //设置每一个列表项的用户id和会议室id
            boMeetItem->setUserID(userID);
            boMeetItem->setMeetRoomID(query.value(0).toInt());
            //设置每一个列表项的会议室名称和地址
            boMeetItem->getLabMeetRoomName()->setText(query.value(1).toString().trimmed());
            boMeetItem->getLabMeetRoomAddr()->setText(query.value(2).toString().trimmed());
            //关联item和boMeetItem，以在界面中显示自定义的boMeetItem
            ui->lwBoMeet->setItemWidget(item, boMeetItem);
        }
    }


    //显示所有已存在的会议预约信息
    QString curDTStr = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    sql = QString("select meet_id, meet_title, app_r_id, start_time, end_time from meet_app where pay = 1 and end_time > '%1'").arg(curDTStr);
    QSqlQuery query2(db);
    query.exec(sql);

    if (query.first()){ //当存在会议信息时才显示，否则不显示
        query2.exec(QString("select meetroom_name, meetroom_addr from meet_room where meetroom_id = %1").arg(query.value(2).toInt()));
        if (query2.first()) {   //查询会议室信息成功时才继续执行
            //将所有即将召开和正在召开的会议预约信息显示到QListWidget中
            for (int i = 0; i < query.size(); i++, query.next()) {
                QListWidgetItem *item = new QListWidgetItem();
                item->setSizeHint(QSize(680, 70));
                ui->lwAttMeet->addItem(item);
                lwiMeeting *meetingItem = new lwiMeeting(ui->lwAttMeet);
                meetingItem->getLabMeetID()->setText("会议号：" + query.value(0).toString());
                meetingItem->getLabMeetTitle()->setText(query.value(1).toString());
                meetingItem->getLabMRName()->setText(query2.value(0).toString());
                meetingItem->getLabMRAddr()->setText(query2.value(1).toString());
                meetingItem->getLabStarTime()->setText(query.value(3).toString());
                meetingItem->getLabEndTime()->setText(query.value(4).toString());
                ui->lwAttMeet->setItemWidget(item, meetingItem);
            }
        }
    }
}


void MeetSys::getUsID(int usID)
{
    userID = usID;
}
