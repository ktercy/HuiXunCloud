#ifndef BOOKMEET_H
#define BOOKMEET_H

#include <QDialog>

namespace Ui {
class BookMeet;
}

class BookMeet : public QDialog
{
    Q_OBJECT

public:
    explicit BookMeet(QWidget *parent = nullptr);
    ~BookMeet();

private slots:
    void setMeetDura(); //根据用户的输入自动计算出会议时长

private:
    Ui::BookMeet *ui;
    double meetDura;   //会议时长，以小时为单位，不能超过4小时
};

#endif // BOOKMEET_H
