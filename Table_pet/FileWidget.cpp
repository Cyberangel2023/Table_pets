#include "FileWidget.h"

FileWidget::FileWidget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 160);
    this->move(0, 0);
    this->setWindowFlag(Qt::FramelessWindowHint); // 去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground); // 去除窗口
    this->setWindowFlags(this->windowFlags() | Qt::ToolTip | Qt::WindowStaysOnTopHint); // 任务栏不显示

    // 设置焦点策略，允许通过 Tab 键或点击获得焦点
    setFocusPolicy(Qt::StrongFocus);

    // 初始化 ArchorPane
    this->anchorPane = new AnchorPane(this);

    // 初始化 scrollPane
    this->scrollPane = new ScrollPane();

    // 创建一个容器 widget 用于放置滚动内容
    QWidget *contentWidget = new QWidget();
    QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 0, 20, 0);
    contentLayout->setSpacing(21);

    // 加载文件按钮
    listDesktopFiles();

    foreach (MainFile* file, this->deskFiles) {
        contentLayout->addWidget(file);
    }

    this->scrollPane->setWidget(contentWidget);

    // 设置 anchorPane 的边距，将 scrollPane 添加到 anchorPane 的布局中
    QVBoxLayout *anchorPaneLayout = qobject_cast<QVBoxLayout*>(this->anchorPane->layout());
    if (anchorPaneLayout) {
        anchorPaneLayout->setContentsMargins(0, 0, 0, 0);
        anchorPaneLayout->addWidget(this->scrollPane);
    }

    // 将 AnchorPane 添加到 MainScene 的布局中
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(this->anchorPane);

    setLayout(mainLayout);
}

FileWidget::~FileWidget()
{

}

void FileWidget::focusOutEvent(QFocusEvent *event)
{
    // 当fileWidget失去焦点时执行的代码
    //qDebug() << "FileWidget lost focus";
    this->hide();
    foreach (MainFile* file, this->selectedFiles) {
        file->reset();
    }

    // 调用基类的focusOutEvent以确保其他处理正常进行
    QWidget::focusOutEvent(event);
}

void FileWidget::listDesktopFiles() {
    // 获取桌面路径
    QString desktopPath = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();

    // 创建桌面目录对象
    QDir desktopDir(desktopPath);

    // 检查桌面目录是否存在
    if (!desktopDir.exists()) {
        qDebug() << "Desktop directory does not exist.";
        return;
    }

    // 遍历桌面目录中的文件
    foreach (QFileInfo fileInfo, desktopDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::System)) {
        // 获取文件路径
        QString filePath = fileInfo.absoluteFilePath();
        // 获取文件名字
        QString fileName = fileInfo.fileName();

        //qDebug() << fileName;

        MainFile* file = new MainFile(fileName, filePath, this);
        this->deskFiles.insert(fileName, file);
    }
}
