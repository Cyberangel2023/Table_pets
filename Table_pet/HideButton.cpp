#include "HideButton.h"
#include <QGuiApplication>
#include <QScreen>

HideButton::HideButton(Widget* mainScene, QWidget *parent)
    : QWidget(parent), mainScene(mainScene)
{
    // 获取屏幕尺寸
    this->screen = QGuiApplication::primaryScreen();
    this->screenRect = this->screen->geometry();
    this->availableRect = screen->availableGeometry();

    QRect getRect = screenRect;
    getRect.setRect(screenRect.x(), screenRect.y(), screenRect.width() + 20, screenRect.height() + 20);
    this->resize(getRect.size());
    this->move(0, 0);
    this->setWindowFlag(Qt::FramelessWindowHint); // 去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground); // 去除窗口
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool); // 任务栏不显示，置顶窗口

    // 创建按钮
    QPushButton *button = new QPushButton("", this);
    // 设置右侧边
    button->setGeometry(this->screenRect.width() - 1, 200, 5, this->screenRect.height() - 448); // 设置按钮位置和大小
    //button->setGeometry(this->screenRect.width() - 1, this->screenRect.height() - 96, 5, 48); // 设置按钮位置和大小
    // 设置按钮的样式
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 255, 255, 2); /* 白色背景，透明度2 */"
        "   border-radius: 0px; /* 圆角边框 */"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(2, 220, 77, 64); /* 鼠标悬停时背景颜色加深 */"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(2, 220, 77, 128); /* 按下时背景颜色最深 */"
        "}"
        );

    // 连接按钮点击信号到槽函数
    connect(button, &QPushButton::clicked, this, &HideButton::switchShow);
}

HideButton::~HideButton()
{

}

void HideButton::switchShow() {
    if (this->mainScene->isVisible()) {
        this->mainScene->hide();
    } else {
        this->mainScene->show();
    }
    this->show();
}
