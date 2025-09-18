#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QTextBrowser>
#include <QSettings>
#include <windows.h>

#include "WallpaperStyle.h"
#include "Widget.h"
#include "FileWidget.h"

class MainScene : public QWidget
{
public:
    MainScene(Widget *widget, FileWidget *fileWidget, QWidget *parent = nullptr);
    ~MainScene();

private:
    QString GetRealPathFromResource(const QString &resourcePath); // 获取绝对路径
    bool SetWallpaperStyle(); // 设置壁纸样式
    bool SetWallpaper(); // 设置壁纸
    bool HideDesktopIcons(bool hide); // 设置桌面图标是否隐藏
    void createAndWriteFile();
    void addDeskTopFileToTXT1();
    void clearFile();
    void listShowFile();
    void addSystemSpecialItems();
    void addSpecialItem(const QString &name, const QString &itemPath, QString iconName, int index);
    void addToFileWidget();

private:
    QFile* Txt1;
    QFile* Txt2;

    WallpaperStyle style; // 壁纸样式
    QString wallpaperPath; // 壁纸路径
    Widget* widget;
    FileWidget* fileWidget;

    QMap<int, MainFile*> showFiles; // 储存显示的文件
};

#endif // MAINSCENE_H
