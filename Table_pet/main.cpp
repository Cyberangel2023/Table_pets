#include "widget.h"
#include "HideButton.h"
#include "MainScene.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainScene m;
    //m.show();

    Widget w;
    w.show();

    // 创建一个新窗口
    HideButton* hideButton = new HideButton(&w);
    hideButton->show();
    return a.exec();
}
