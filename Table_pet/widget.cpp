#include "widget.h"
#include <QPaintEvent>
#include <QPainter>//绘图
#include <QPixmap>//图片
#include <QCursor>
#include <QMetaEnum>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer(this)),
    menu(new QMenu(this))
{
    this->setWindowFlag(Qt::FramelessWindowHint);//去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->installEventFilter(new DragFilter);

    connect(timer, &QTimer::timeout, [this](){
        static int index = 0;//记录显示动作的当前图片索引
        auto paths = this->action_map.value(this->cur_role_act);
        this->cur_role_pix = paths[index++ % paths.size()];
        this->update();
    });

    initMenu();
    loadRoleActRes();
    showActAnimation(RoleAct::Swing_ing);
}

Widget::~Widget() {}

void Widget::showActAnimation(RoleAct k)
{
    timer->stop();

    this->cur_role_act = k;

    timer->start(60);
}

void Widget::onMenuTriggered(QAction *action)
{
    //QMetaEnum me = QMetaEnum::fromType<RoleAct>();
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
