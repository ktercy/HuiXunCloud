#ifndef LWIBOMEET_H
#define LWIBOMEET_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class lwiBoMeet;
}

class lwiBoMeet : public QWidget
{
    Q_OBJECT

public:
    explicit lwiBoMeet(QWidget *parent = nullptr);
    ~lwiBoMeet();

    QLabel* getLabMeetRoomName();
    QLabel* getLabMeetRoomAddr();
private slots:
    void on_btnBook_clicked();

private:
    Ui::lwiBoMeet *ui;
};

#endif // LWIBOMEET_H
