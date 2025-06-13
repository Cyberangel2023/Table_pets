#include "widget.h"
#include <QPaintEvent>
#include <QPainter>//绘图
#include <QPixmap>//图片
#include <QCursor>
#include <QMetaEnum>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer(this)),
    menu(new QMenu(this)),
    isLoop(true),
    isLeft(true),
    index(0)
{
    this->setWindowFlag(Qt::FramelessWindowHint);//去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground);//去除窗口

    this->installEventFilter(new DragFilter);

    connect(timer, &QTimer::timeout, [this](){
        auto paths = isLeft ? this->action_map_left.value(this->cur_role_act) :
                         this->action_map_right.value(this->cur_role_act);
        int last_index = index;
        this->cur_role_pix = paths[index++ % paths.size()];
        this->update();
        if (!isLoop && last_index + 1 == paths.size()) {
            showActAnimation(this->next_role_act);
        }
    });

    initMenu();
    loadRoleActResLeft();
    loadRoleActResRight();
    showActAnimation(RoleAct::Stand);
}

Widget::~Widget() {}

void Widget::CheckRoleAct(RoleAct roleAct) {
    if (roleAct == RoleAct::Stand) {
        this->isLoop = true;
    } else if (roleAct == RoleAct::Swing_ing) {
        this->isLoop = true;
    }
}

void Widget::showActAnimation(RoleAct k)
{
    timer->stop();

    this->index = 0;
    this->cur_role_act = k;
    CheckRoleAct(this->cur_role_act);

    timer->start(60);
}

void Widget::onMenuTriggered(QAction *action)
{
    const QString text = action->text();
    RoleAct roleAct;

    if (text == "打招呼") {
        roleAct = RoleAct::Greet;
        this->isLoop = false;
        this->next_role_act = RoleAct::Stand;
    } else if (text == "荡秋千") {
        roleAct = RoleAct::Swing_start;
        this->isLoop = false;
        this->next_role_act = RoleAct::Swing_ing;
    } else if (text == "睡觉") {
        roleAct = RoleAct::Sleep;
        this->isLoop = true;
        this->next_role_act = RoleAct::Stand;
    } else {
        roleAct = RoleAct::Stand; // 默认动作
        this->isLoop = true;
    }

    showActAnimation(roleAct);
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    QPixmap pix;
    pix.load(this->cur_role_pix.toLocalFile());

    painter.drawPixmap(0, 0, pix);
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    this->menu->popup(QCursor::pos());
}

void Widget::loadRoleActResLeft()
{
    auto addRes = [this](RoleAct k, QString path, int count) {
        QList<QUrl> paths;
        char buf[260];
        //向左图片加载
        for (int i = 0; i <= count; ++i) {
            memset(buf, 0, sizeof(buf));
            sprintf_s(buf, path.toStdString().c_str(), i);
            paths.append(QUrl::fromLocalFile(buf));
        }
        action_map_left.insert(k, paths);
    };

    addRes(RoleAct::Applaud, ":/Applaud/resources/action/yb_%d.png", 26);
    addRes(RoleAct::Click, ":/Click/resources/action/mm_%d.png", 20);
    addRes(RoleAct::Drag, ":/Drag/resources/action/t_%d.png", 24);
    addRes(RoleAct::Fly, ":/Fly/resources/action/fing_%d.png", 24);
    addRes(RoleAct::Greet, ":/Greet/resources/action/hs_%d.png", 28);
    addRes(RoleAct::Land, ":/Land/resources/action/mg_%d.png", 12);
    addRes(RoleAct::Play_1, ":/Play_1/resources/action/nl_%d.png", 72);
    addRes(RoleAct::Play_2, ":/Play_2/resources/action/zr_%d.png", 56);
    addRes(RoleAct::Play_sit, ":/Play_sit/resources/action/play_%d.png", 40);
    addRes(RoleAct::Sit, ":/Sit/resources/action/b_%d.png", 24);
    addRes(RoleAct::Sleep, ":/Sleep/resources/action/sleep_%d.png", 24);
    addRes(RoleAct::Sluggish, ":/Sluggish/resources/action/bhc_%d.png", 24);
    addRes(RoleAct::Stand, ":/Stand/resources/action/hx_%d.png", 24);
    addRes(RoleAct::Swing_end, ":/Swing_end/resources/action/qqend_%d.png", 36);
    addRes(RoleAct::Swing_ing, ":/Swing_ing/resources/action/qing_%d.png", 32);
    addRes(RoleAct::Swing_start, ":/Swing_start/resources/action/qq_%d.png", 36);
    addRes(RoleAct::Walk, ":/Walk/resources/action/walk_%d.png", 24);
}

void Widget::loadRoleActResRight()
{
    auto addRes = [this](RoleAct k, QString path, int count) {
        QList<QUrl> paths;
        char buf[260];
        //向右图片加载
        for (int i = 0; i <= count; ++i) {
            memset(buf, 0, sizeof(buf));
            sprintf_s(buf, path.toStdString().c_str(), i);
            paths.append(QUrl::fromLocalFile(buf));
        }
        action_map_right.insert(k, paths);
    };

    addRes(RoleAct::Applaud, ":/Applaud_r/resources/action_r/yb_%d.png", 26);
    addRes(RoleAct::Click, ":/Click_r/resources/action_r/mm_%d.png", 20);
    addRes(RoleAct::Drag, ":/Drag_r/resources/action_r/t_%d.png", 24);
    addRes(RoleAct::Fly, ":/Fly_r/resources/action_r/fing_%d.png", 24);
    addRes(RoleAct::Greet, ":/Greet_r/resources/action_r/hs_%d.png", 28);
    addRes(RoleAct::Land, ":/Land_r/resources/action_r/mg_%d.png", 12);
    addRes(RoleAct::Play_1, ":/Play_1_r/resources/action_r/nl_%d.png", 72);
    addRes(RoleAct::Play_2, ":/Play_2_r/resources/action_r/zr_%d.png", 56);
    addRes(RoleAct::Play_sit, ":/Play_sit_r/resources/action_r/play_%d.png", 40);
    addRes(RoleAct::Sit, ":/Sit_r/resources/action_r/b_%d.png", 24);
    addRes(RoleAct::Sleep, ":/Sleep_r/resources/action_r/sleep_%d.png", 24);
    addRes(RoleAct::Sluggish, ":/Sluggish_r/resources/action_r/bhc_%d.png", 24);
    addRes(RoleAct::Stand, ":/Stand_r/resources/action_r/hx_%d.png", 24);
    addRes(RoleAct::Swing_end, ":/Swing_end_r/resources/action_r/qqend_%d.png", 36);
    addRes(RoleAct::Swing_ing, ":/Swing_ing_r/resources/action_r/qing_%d.png", 32);
    addRes(RoleAct::Swing_start, ":/Swing_start_r/resources/action_r/qq_%d.png", 36);
    addRes(RoleAct::Walk, ":/Walk_r/resources/action_r/walk_%d.png", 24);
}

void Widget::initMenu()
{
    menu->addAction("打招呼");
    menu->addAction("荡秋千");
    menu->addAction("睡觉");

    QAction* act = new QAction("Hide");
    connect(act, &QAction::triggered, [this](){
        this->setVisible(false);
    });

    menu->addAction(act);

    connect(this->menu, &QMenu::triggered, this, &Widget::onMenuTriggered);
}
