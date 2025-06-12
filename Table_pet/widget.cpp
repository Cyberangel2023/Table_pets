#include "widget.h"
#include <QPaintEvent>
#include <QPainter>//绘图
#include <QPixmap>//图片

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, [this](){
        static int index = 0;//记录显示动作的当前图片索引
        auto paths = this->action_map.value(this->cur_role_act);
        this->cur_role_pix = paths[index++ % paths.size()];
        this->update();
    });

    loadRoleActRes();
    showActAnimation(RoleAct::Swing_ing);
}

Widget::~Widget() {}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pix;
    pix.load(this->cur_role_pix.toLocalFile());

    painter.drawPixmap(0, 0, pix);
}

void Widget::showActAnimation(RoleAct k)
{
    timer->stop();

    this->cur_role_act = k;

    timer->start(60);
}

void Widget::loadRoleActRes()
{
    auto addRes = [this](RoleAct k, QString path, int count) {
        QList<QUrl> paths;
        char buf[260];
        for (int i = 0; i <= count; ++i) {
            memset(buf, 0, sizeof(buf));
            sprintf_s(buf, path.toStdString().c_str(), i);
            paths.append(QUrl::fromLocalFile(buf));
        }
        action_map.insert(k, paths);
    };

    addRes(RoleAct::Greet, ":/Greet/resources/action/hs_%d.png", 28);
    addRes(RoleAct::Swing_ing, ":/Swing_ing/resources/action/qing_%d.png", 32);
    addRes(RoleAct::Sleep, ":/Sleep/resources/action/sleep_%d.png", 24);
}
