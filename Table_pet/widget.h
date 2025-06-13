#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QUrl>
#include <QTimer>
#include <QEvent>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QTime>

class QPaintEvent;

namespace Act {
    Q_NAMESPACE

enum RoleAct {
        Greet,
        Swing_start,
        Swing_ing,
        Swing_end,
        Sleep,
        Click,
        Drag,
        Applaud,
        Fly,
        Fly_start,
        Land,
        Play_1,
        Play_2,
        Play_sit,
        Sit,
        Sluggish,
        Stand,
        Walk
    };
    Q_ENUM_NS(RoleAct);
}

    using namespace Act;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    void showActAnimation(RoleAct);

    void setLoop(bool flag) {
        this->isLoop = flag;
    }

    void setLeft(bool flag) {
        this->isLeft = flag;
    }

    RoleAct getCurRoleAct() {
        return this->cur_role_act;
    }

    void setNextRoleAct(RoleAct roleAct) {
        this->next_role_act = roleAct;
    }

public slots:
    void onMenuTriggered(QAction* action);

protected:
    void paintEvent(QPaintEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private:
    //加载图片
    void loadRoleActResLeft();
    void loadRoleActResRight();
    //初始化菜单
    void initMenu();
    void CheckRoleAct(RoleAct roleAct);

private:
    QMap<RoleAct, QList<QPixmap>> action_map_left;
    QMap<RoleAct, QList<QPixmap>> action_map_right;
    QTimer* timer;
    RoleAct cur_role_act;
    RoleAct next_role_act;
    RoleAct willRoleAct;
    QPixmap cur_role_pix;
    QMenu* menu;
    bool isLeft;//根据你的需求设置初始方向
    bool isLoop;//初始状态可能是循环站立
    bool isWill;//准备播放下一个动画
    int index;//记录显示动作的当前图片索引
};

class DragFilter : public QObject {
    Q_OBJECT
public:
    explicit DragFilter(Widget* widget, QObject* parent = nullptr)
        : QObject(parent), widget(widget), mLongPressTimer(this) {
        // 连接定时器的超时信号到 onLongPress 槽函数
        connect(&mLongPressTimer, &QTimer::timeout, this, &DragFilter::onLongPress);
    }

    bool eventFilter(QObject* obj, QEvent* event) override {
        auto w = dynamic_cast<QWidget*>(obj);
        if (!w) {
            return false;
        }
        if (event->type() == QEvent::MouseButtonPress) {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if (e) {
                pos = e->pos();
                if (widget->getCurRoleAct() == RoleAct::Stand && e->buttons() & Qt::MouseButton::LeftButton) {
                    // 启动定时器，300毫秒后触发长按事件
                    mLongPressTimer.start(100);
                    mpressed = true;
                    mclickStartPosition = e->pos();
                    mclickStartTime = QTime::currentTime();
                }
            }
        } else if (event->type() == QEvent::MouseMove) {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if (e) {
                if (e->buttons() & Qt::MouseButton::LeftButton) {
                    w->move(e->globalPosition().toPoint() - pos);
                }
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if (e && e->button() == Qt::LeftButton) {
                mpressed = false;
                QTime currentTime = QTime::currentTime();
                qint64 clickDuration = mclickStartTime.msecsTo(currentTime);
                QPoint clickEndPosition = e->pos();
                int clickDistance = QLineF(mclickStartPosition, clickEndPosition).length();

                mLongPressTimer.stop();
                // 检测是否为点击事件
                if (widget->getCurRoleAct() == RoleAct::Stand && clickDuration < 100 && clickDistance < 5) { // 300ms内，移动距离小于5像素
                    if (widget) {
                        widget->showActAnimation(RoleAct::Click);
                        widget->setLoop(false);
                        widget->setNextRoleAct(RoleAct::Stand);
                    }
                }
                if (widget->getCurRoleAct() == RoleAct::Drag) {
                    if (widget) {
                        widget->showActAnimation(RoleAct::Stand);
                        widget->setLoop(true);
                    }
                }
            }
        }
        return QObject::eventFilter(obj, event);
    }

private slots:
    void onLongPress() {
        if (widget) {
            widget->showActAnimation(RoleAct::Drag);
            widget->setLoop(true);
        }
    }

private:
    QPoint pos;
    Widget* widget; //存储 Widget 的指针
    bool mpressed;
    QPoint mclickStartPosition;
    QTime mclickStartTime;
    QTimer mLongPressTimer;
};

#endif // WIDGET_H
