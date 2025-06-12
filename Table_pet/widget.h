#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QUrl>
#include <QTimer>

class QPaintEvent;

namespace Act {
    Q_NAMESPACE

enum RoleAct {
        Greet,
        Swing_ing,
        Sleep
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

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    void showActAnimation(RoleAct);

private:
    //加载图片
    void loadRoleActRes();

private:
    QMap<RoleAct, QList<QUrl>> action_map;
    QTimer* timer;
    RoleAct cur_role_act;
    QUrl cur_role_pix;
};
#endif // WIDGET_H
