#include "Widget.h"
#include "HideButton.h"
#include "MainScene.h"
#include "FileWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 桌宠文件界面
    FileWidget* f = new FileWidget();

    // 桌宠界面
    Widget* w = new Widget(f);
    w->show();

    // 隐藏按钮
    HideButton* hideButton = new HideButton(w);
    hideButton->show();

    // 主界面
    MainScene* m = new MainScene(w, f);
    //m->show();

    return a.exec();
}
