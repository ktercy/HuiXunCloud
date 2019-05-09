#include "bookmeet.h"
#include "ui_bookmeet.h"
#include <QDateTime>
#include <QDebug>
#include <cmath>    //使用round()函数
#include <QMessageBox>

BookMeet::BookMeet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookMeet)
{
    ui->setupUi(this);

    //将提示信息的文本颜色设置为红色
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->labNote->setPalette(pa);

    //设置默认的会议开始时间和会议结束时间都为当前时间
    ui->dteStart->setDateTime(QDateTime::currentDateTime());
    ui->dteEnd->setDateTime(QDateTime::currentDateTime());

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

//    qDebug() << "Meeting duration is " << temp;
}
