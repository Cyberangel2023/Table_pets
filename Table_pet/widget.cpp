#include "widget.h"
#include <QPaintEvent>
#include <QPainter>//绘图
#include <QPixmap>//图片
#include <QCursor>
#include <QMetaEnum>
#include <QPixmapCache>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer(this)),
    menu(new QMenu(this)),
    isLoop(true),
    isLeft(true),
    isWill(false),
    index(0)
{
    this->setWindowFlag(Qt::FramelessWindowHint);//去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground);//去除窗口

    this->installEventFilter(new DragFilter(this));

    connect(timer, &QTimer::timeout, this, [this](){
        auto paths = isLeft ? this->action_map_left.value(this->cur_role_act) :
                         this->action_map_right.value(this->cur_role_act);
        this->cur_role_pix = paths[index++ % paths.size()];
        this->update();
        if (this->isWill && index >= paths.size()) {
            showActAnimation(this->willRoleAct);
        } else if (!isLoop && index >= paths.size()) {
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
    } else if (roleAct == RoleAct::Sleep) {
        this->isLoop = true;
    } else if (roleAct == RoleAct::Swing_ing) {
        this->isLoop = true;
    } else if (roleAct == RoleAct::Fly) {
        this->isLoop = true;
    } else if (roleAct == RoleAct::Greet) {
        this->isLoop = false;
        this->next_role_act = RoleAct::Stand;
    } else if (roleAct == RoleAct::Greet) {
        this->isLoop = false;
        this->next_role_act = RoleAct::Stand;
    }
    this->isWill = false;
}

void Widget::showActAnimation(RoleAct k)
{
    timer->stop();

    index = 0;
    this->cur_role_act = k;
    CheckRoleAct(this->cur_role_act);

    timer->start(60);
}

void Widget::onMenuTriggered(QAction *action)
{
    const QString text = action->text();

    if (text == "打招呼") {
        if (this->cur_role_act == RoleAct::Swing_ing) {
            this->willRoleAct = RoleAct::Swing_end;
            this->isLoop = false;
            this->isWill = true;
            this->next_role_act = RoleAct::Greet;
            return;
        } else if (this->cur_role_act == RoleAct::Fly) {

        } else {
            this->willRoleAct = RoleAct::Greet;
            this->isLoop = false;
            this->next_role_act = RoleAct::Stand;
        }
    } else if (text == "荡秋千") {
        if (this->cur_role_act == RoleAct::Swing_ing) {
            return;
        } else if (this->cur_role_act == RoleAct::Fly) {
            this->willRoleAct = RoleAct::Swing_end;
            this->isLoop = false;
            this->isWill = true;
            this->next_role_act = RoleAct::Fly_start;
            return;
        } else {
            this->willRoleAct = RoleAct::Swing_start;
            this->isLoop = false;
            this->next_role_act = RoleAct::Swing_ing;
        }
    } else if (text == "睡觉") {
        if (this->cur_role_act == RoleAct::Swing_ing) {
            this->willRoleAct = RoleAct::Swing_end;
            this->isLoop = false;
            this->isWill = true;
            this->next_role_act = RoleAct::Sleep;
            return;
        } else if (this->cur_role_act == RoleAct::Fly) {

        } else {
            this->willRoleAct = RoleAct::Sleep;
            this->isLoop = true;
        }
    } else if (text == "站立") {
        if (this->cur_role_act == RoleAct::Swing_ing) {
            this->willRoleAct = RoleAct::Swing_end;
            this->isLoop = false;
            this->isWill = true;
            this->next_role_act = RoleAct::Stand;
            return;
        } else if (this->cur_role_act == RoleAct::Fly) {

        } else {
            this->willRoleAct = RoleAct::Stand;
            this->isLoop = true;
        }
    } else {
        this->willRoleAct = RoleAct::Stand; // 默认动作
        this->isLoop = true;
    }
    showActAnimation(this->willRoleAct);
}

void Widget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);

    // 双缓冲：先绘制到临时图像
    QPixmap buffer(size());
    buffer.fill(Qt::transparent);
    QPainter bufferPainter(&buffer);
    bufferPainter.drawPixmap(0,  0, cur_role_pix);

    // 一次性绘制到屏幕
    painter.drawPixmap(0,  0, buffer);
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    this->menu->popup(QCursor::pos());
}

void Widget::loadRoleActResLeft()
{
    auto addRes = [this](RoleAct k, QString path, int count) {
        QList<QPixmap> paths;
        char buf[260];
        //向左图片加载
        for (int i = 0; i <= count; ++i) {
            memset(buf, 0, sizeof(buf));
            sprintf_s(buf, path.toStdString().c_str(), i);
            QPixmap pix;
            pix.load(QUrl::fromLocalFile(buf).toLocalFile());
            paths.append(pix);
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
        QList<QPixmap> paths;
        char buf[260];
        //向左图片加载
        for (int i = 0; i <= count; ++i) {
            memset(buf, 0, sizeof(buf));
            sprintf_s(buf, path.toStdString().c_str(), i);
            QPixmap pix;
            pix.load(QUrl::fromLocalFile(buf).toLocalFile());
            paths.append(pix);
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
    menu->addAction("站立");

    QAction* act = new QAction("退出");
    connect(act, &QAction::triggered, this, [this](){
        this->setVisible(false);
    });

    menu->addAction(act);

    connect(this->menu, &QMenu::triggered, this, &Widget::onMenuTriggered);
}
