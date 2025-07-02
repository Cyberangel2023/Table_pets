#include "MainScene.h"

MainScene::MainScene(Widget *widget, FileWidget *fileWidget, QWidget *parent)
    : widget(widget), fileWidget(fileWidget)
{
    createAndWriteFile();
    // clearFile();
    listShowFile();

    style = WallpaperStyle::Fill;
    SetWallpaperStyle();
    wallpaperPath = "C:\\Users\\25444\\Desktop\\imgs\\Kiana3.jpg";
    SetWallpaper();

    HideDesktopIcons(false);
}

MainScene::~MainScene()
{
    // 确保文件关闭和删除
    if (this->Txt1) {
        this->Txt1->close();
        delete this->Txt1;
    }
}

bool MainScene::SetWallpaperStyle()
{
    HKEY hKey;
    const QString subKey = QString::fromWCharArray(L"Control Panel\\Desktop");

    // 打开注册表键
    if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey.toStdWString().c_str(), 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    QString wallpaperStyle = "0";
    QString tileWallpaper = "0";

    switch (style) {
    case WallpaperStyle::Tile:
        wallpaperStyle = "0";
        tileWallpaper = "1";
        break;
    case WallpaperStyle::Center:
        wallpaperStyle = "0";
        tileWallpaper = "0";
        break;
    case WallpaperStyle::Stretch:
        wallpaperStyle = "2";
        tileWallpaper = "0";
        break;
    case WallpaperStyle::Fit:
        wallpaperStyle = "6";
        tileWallpaper = "0";
        break;
    case WallpaperStyle::Fill:
        wallpaperStyle = "10";
        tileWallpaper = "0";
        break;
    }

    // 写入注册表
    if (RegSetValueEx(hKey, L"WallpaperStyle", 0, REG_SZ, (const BYTE*)wallpaperStyle.toStdWString().c_str(), (wallpaperStyle.toStdWString().size() + 1) * sizeof(wchar_t)) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return false;
    }

    if (RegSetValueEx(hKey, L"TileWallpaper", 0, REG_SZ, (const BYTE*)tileWallpaper.toStdWString().c_str(), (tileWallpaper.toStdWString().size() + 1) * sizeof(wchar_t)) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return false;
    }

    // 关闭注册表键
    RegCloseKey(hKey);
    return true;
}

bool MainScene::SetWallpaper()
{
    // 使用 SystemParametersInfo 函数设置壁纸
    if (!SystemParametersInfoW(
            SPI_SETDESKWALLPAPER, // 设置桌面壁纸
            0,
            (PVOID)wallpaperPath.utf16(), // 壁纸路径
            SPIF_UPDATEINIFILE | SPIF_SENDCHANGE // 立即生效
            )) {
        return false;
    }
    return true;
}

bool MainScene::HideDesktopIcons(bool hide) {
    // 获取桌面窗口句柄
    HWND hWndDesktop = FindWindow(L"Progman", L"Program Manager");
    if (hWndDesktop == NULL) {
        return -1;
    }

    // 获取桌面图标的句柄
    HWND hWndFolderView = FindWindowEx(hWndDesktop, NULL, L"SHELLDLL_DefView", NULL);
    if (hWndFolderView == NULL) {
        return -1;
    }

    // 获取桌面图标的子窗口句柄
    HWND hWndIconView = FindWindowEx(hWndFolderView, NULL, L"SysListView32", L"FolderView");
    if (hWndIconView == NULL) {
        return -1;
    }

    if (hide) {
        // 隐藏桌面图标
        ShowWindow(hWndIconView, SW_HIDE);
    } else {
        ShowWindow(hWndIconView, SW_SHOW);
    }
    return false;
}

// 创建并写入文件
void MainScene::createAndWriteFile() {
    // 获取当前工作目录（相对路径基准）
    QString currentPath = QDir::currentPath();
    QString filePath = currentPath + "/txt1.txt";

    // 创建并打开文件
    this->Txt1 = new QFile(filePath);
    if (!this->Txt1->open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "文件打开失败：" << this->Txt1->errorString();
        delete Txt1;
        this->Txt1 = nullptr;
        return;
    }
    addDeskTopFileToTXT1();
    this->Txt1->close();
}

// 添加桌面文件
void MainScene::addDeskTopFileToTXT1()
{
    // 获取桌面路径
    QString desktopPath = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();

    // 创建桌面目录对象
    QDir desktopDir(desktopPath);

    // 检查桌面目录是否存在
    if (!desktopDir.exists()) {
        qDebug() << "Desktop directory does not exist.";
        return;
    }

    QTextStream stream(this->Txt1);
    stream.setAutoDetectUnicode(true);
    int i = 1;

    // 遍历桌面目录中的文件
    foreach (QFileInfo fileInfo, desktopDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::System)) {
        // 获取文件路径
        QString filePath = fileInfo.absoluteFilePath();
        // 获取文件名字
        QString fileName = fileInfo.baseName();

        stream << QString("%1?" + fileName + "?" + filePath + "\n").arg(i);
        i++;
    }
}

void MainScene::clearFile()
{
    // 清空文件
    QString currentPath = QDir::currentPath();
    QString filePath = currentPath + "/txt1.txt";
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write("");
        file.close();
    } else {
        qCritical() << "清空文件失败：" << file.errorString();
    }
    file.close();
}

void MainScene::listShowFile()
{
    QString currentPath = QDir::currentPath();
    QString filePath = currentPath + "/txt1.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "文件打开失败：" << file.errorString();
        return;
    }

    // 读取文件内容
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) continue;

        QStringList parts = line.split('?');
        if (parts.size() >= 3) {
            QString fileIndex;
            QString filePath;
            QString fileName;
            filePath = parts[2]; // 第三部分是文件路径
            fileName = parts[1]; // 第二部分是文件名称
            fileIndex = parts[0]; // 第一部分是文件索引
            //qDebug() << fileName;
            MainFile* file = new MainFile(fileName, filePath, this);
            file->toolTip() = fileName;;
            this->showFiles.insert(fileIndex.toInt() + 4, file);
        }
    }
    file.close();
    addSystemSpecialItems();
    this->fileWidget->addContent(&this->showFiles);
}

void MainScene::addSystemSpecialItems() {
    // 添加文件资源管理器特殊项
    QString string = "文件资源管理器";
    QString path = "C:/Windows/explorer.exe";
    int index = 1;
    addSpecialItem(
        string,
        path, // ComputerFolder
        "home",
        index
        );

    // 添加"我的电脑"特殊项
    string = "我的电脑";
    path = "::20D04FE0-3AEA-1069-A2D8-08002B30309D";
    index = 2;
    addSpecialItem(
        string,
        path, // ComputerFolder
        "computer",
        index
        );

    // 添加回收站特殊项
    string = "回收站";
    path = "::{645FF040-5081-101B-9F08-00AA002F954E}";
    index = 3;
    addSpecialItem(
        string,
        path, // RecycleBin
        "user-trash",
        index
        );

    // 添加控制面板特殊项
    string = "控制面板";
    path = "::{21EC2020-3AEA-1069-A2DD-08002B30309D}";
    index = 4;
    addSpecialItem(
        string,
        path, // RecycleBin
        "control-panel",
        index
        );
}

void MainScene::addSpecialItem(const QString &name, const QString &itemPath, QString iconName, int index) {
    // 创建虚拟文件项
    MainFile* specialItem = new MainFile(name, itemPath, this);

    specialItem->setToolTip(name);
    specialItem->setIcon(iconName);
    showFiles.insert(index, specialItem);
}
