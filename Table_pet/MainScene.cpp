#include "MainScene.h"

MainScene::MainScene(QWidget *parent)
{
    createAndWriteFile();
    // clearFile();

}

MainScene::~MainScene()
{
    // 确保文件关闭和删除
    if (this->Txt1) {
        this->Txt1->close();
        delete this->Txt1;
    }
}

// 创建并写入文件
void MainScene::createAndWriteFile() {
    // 获取当前工作目录（相对路径基准）
    QString currentPath = QDir::currentPath();
    QString filePath = currentPath + "/txt1.txt";

    // 检测文件存在性
    if (!QFile::exists(filePath)) {
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
    QString filePath = QDir::currentPath() + "/txt1.txt";
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write("");
        file.close();
    } else {
        qCritical() << "清空文件失败：" << file.errorString();
    }
}
