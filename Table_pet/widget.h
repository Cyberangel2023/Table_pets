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
    QMap<RoleAct, QList<QUrl>> action_map_left;
    QMap<RoleAct, QList<QUrl>> action_map_right;
    QTimer* timer;
    RoleAct cur_role_act;
    RoleAct next_role_act;
    QUrl cur_role_pix;
    QMenu* menu;
    bool isLeft;//根据你的需求设置初始方向
    bool isLoop;//初始状态可能是循环站立
    int index;//记录显示动作的当前图片索引
};

class DragFilter : public QObject {
public:
    bool eventFilter(QObject* obj, QEvent* event) {
        auto w = dynamic_cast<QWidget*>(obj);
        if (!w) {
            return false;
        }
        if (event->type() == QEvent::MouseButtonPress) {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if (e) {
                pos = e->pos();
            }
        } else if (event->type() == QEvent::MouseMove) {
            auto e = dynamic_cast<QMouseEvent*>(event);
            if (e) {
                if (e->buttons() & Qt::MouseButton::LeftButton) {
                    w->move(e->globalPosition().toPoint() - pos);
                }
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QPoint pos;
};

#endif // WIDGET_H
