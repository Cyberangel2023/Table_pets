#ifndef HIDEBUTTON_H
#define HIDEBUTTON_H

#include <QWidget>
#include <QPushButton>

#include "widget.h"

class HideButton : public QWidget
{
    Q_OBJECT
public:
    HideButton(Widget* mainScene, QWidget *parent = nullptr);
    ~HideButton();

private:
    void switchShow();
    void openThisPC();

private:
    QScreen* screen; // 屏幕
    QRect screenRect; // 屏幕大小
    QRect availableRect;  // 可用区域大小

private:
    Widget* mainScene;
};

#endif // HIDEBUTTON_H
