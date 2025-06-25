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

    // 加载文件按钮
    listDesktopFiles();

    // 初始化 ArchorPane
    this->anchorPane = new AnchorPane(this);

    // 初始化 scrollPane
    this->scrollPane = new ScrollPane();

    // 创建一个容器 widget 用于放置滚动内容
    QWidget *contentWidget = new QWidget();
    QGridLayout* gridLayout = new QGridLayout(contentWidget);
    gridLayout->setContentsMargins(10, 10, 10, 10);
    gridLayout->setSpacing(21);

    // 使用网格布局添加文件项
    int row = 0, col = 0;
    foreach (MainFile* file, this->deskFiles) {
        gridLayout->addWidget(file, row, col);
        col = (col + 1) % 5;
        if (col == 0) row++;
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

    // 创建右键菜单
    contextMenu = new QMenu(this);
    QAction *openAction = contextMenu->addAction("打开");
    QAction *copyAction = contextMenu->addAction("复制");
    QAction *deleteAction = contextMenu->addAction("删除");

    // 连接菜单项的信号到槽函数
    connect(openAction, &QAction::triggered, this, &FileWidget::onOpenActionTriggered);
    connect(copyAction, &QAction::triggered, this, &FileWidget::onCopyActionTriggered);
    connect(deleteAction, &QAction::triggered, this, &FileWidget::onDeleteActionTriggered);
}

FileWidget::~FileWidget()
{

}

void FileWidget::focusOutEvent(QFocusEvent *event)
{
    // 当fileWidget失去焦点时执行的代码
    //qDebug() << "FileWidget lost focus";
    if (!this->contextMenu->isHidden()) {
        return;
    }
    this->hide();
    this->contextMenu->hide();
    this->scrollPane->resetValue();
    foreach (MainFile* file, this->selectedFiles) {
        file->reset();
    }

    // 调用基类的focusOutEvent以确保其他处理正常进行
    QWidget::focusOutEvent(event);
}

void FileWidget::listDesktopFiles() {
    // 添加系统特殊项
    addSystemSpecialItems();

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

void FileWidget::addSystemSpecialItems() {
    // 添加"我的电脑"特殊项
    QString string = "我的电脑";
    QString path = "::20D04FE0-3AEA-1069-A2D8-08002B30309D";
    addSpecialItem(
        string,
        path, // ComputerFolder
        "computer"
        );

    // 添加回收站特殊项
    string = "回收站";
    path = "::{645FF040-5081-101B-9F08-00AA002F954E}";
    addSpecialItem(
        string,
        path, // RecycleBin
        "user-trash"
        );

    // 添加微信
    string = "微信";
    path = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\微信\\微信.lnk";
    addSpecialItem(
        string,
        path // weChat
        );
}

void FileWidget::addSpecialItem(const QString &name, const QString &itemPath, QString iconName) {
    // 创建虚拟文件项
    MainFile* specialItem = new MainFile(name, itemPath, this);

    specialItem->setToolTip(name);
    specialItem->setIcon(iconName);
    deskFiles.insert(name, specialItem);
}

void FileWidget::addSpecialItem(const QString &name, const QString &itemPath) {
    // 创建虚拟文件项
    MainFile* specialItem = new MainFile(name, itemPath, this);

    specialItem->setToolTip(name);
    deskFiles.insert(name, specialItem);
}

void FileWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    QPoint clickPos = event->pos();
    if (event->button() == Qt::LeftButton) {
        // 在这里处理鼠标左键双击事件
        // 检查点击是否在某个 MainFile 控件上
        QWidget *child = childAt(clickPos);
        if (child) {
            if (child->toolTip() == "我的电脑") {
                openThisPC();
                return;
            }
            MainFile *clickedFile = qobject_cast<MainFile*>(child);
            if (clickedFile) {
                QUrl url = QUrl::fromLocalFile(clickedFile->getFilePath());
                if (!QDesktopServices::openUrl(url)) {
                    QMessageBox::warning(this, "错误", "无法使用系统程序打开文件！");
                    return;
                }
                this->hide();
            }
        }
    }
}

void FileWidget::mousePressEvent(QMouseEvent *event) {
    QPoint clickPos = event->pos();
    if (event->button() == Qt::LeftButton) {
        // 检查点击是否在某个 MainFile 控件上
        QWidget *child = childAt(clickPos);
        if (child) {
            MainFile *clickedFile = qobject_cast<MainFile*>(child);
            if (clickedFile) {
                // 处理 MainFile 点击
                bool ctrlPressed = qApp->keyboardModifiers() & Qt::ControlModifier;
                bool shiftPressed = qApp->keyboardModifiers() & Qt::ShiftModifier;

                if (ctrlPressed) {
                    // Ctrl 多选：切换选中状态
                    if (this->selectedFiles.contains(clickedFile)) {
                        this->selectedFiles.removeOne(clickedFile);
                        clickedFile->reset();
                    } else {
                        this->selectedFiles.push_back(clickedFile);
                        clickedFile->setSelected(true);
                    }
                } else if (shiftPressed && !this->selectedFiles.isEmpty()) {
                    // Shift 多选：从最后一个选中的 MainFile 到当前点击的 MainFile 之间的所有 MainFile 都被选中
                    // 这里需要实现 Shift 多选逻辑
                } else {
                    // 单选：清除之前的选中状态，选中当前点击的 MainFile
                    resetFiles();
                    this->selectedFiles.push_back(clickedFile);
                    clickedFile->setSelected(true);
                }
            } else {
                // 点击了非 MainFile 控件（例如背景控件）
                resetFiles();
            }
        }
    } else if (event->button() == Qt::RightButton) {
        // 右键点击事件
        QWidget *child = childAt(clickPos);
        if (child) {
            MainFile *clickedFile = qobject_cast<MainFile*>(child);
            if (clickedFile) {
                // 点击了 MainFile，显示 MainFile 的右键菜单
                contextMenuFile = clickedFile; // 存储当前点击的 MainFile 指针
                resetFiles();
                this->selectedFiles.push_back(clickedFile);
                clickedFile->setSelected(true);
                contextMenu->exec(clickPos + this->pos()); // 在鼠标点击位置显示菜单
            } else {
                // 点击了非 MainFile 控件，可以隐藏菜单或执行其他操作
                resetFiles();
                contextMenu->hide();
            }
        } else {
            // 点击了 FileWidget 的背景区域，隐藏菜单
            resetFiles();
            contextMenu->hide();
        }
    }
}

void FileWidget::mouseReleaseEvent(QMouseEvent *event) {
    // 可以在这里处理鼠标释放事件
}

void FileWidget::resetFiles() {
    foreach (MainFile* file, this->selectedFiles) {
        file->reset();
    }
    this->selectedFiles.clear();
}

// 菜单槽函数实现
void FileWidget::onOpenActionTriggered() {
    if (contextMenuFile) {
        QString filePath = contextMenuFile->getFilePath();
        QUrl url = QUrl::fromLocalFile(filePath);
        if (!QDesktopServices::openUrl(url)) {
            QMessageBox::warning(this, "错误", "无法使用系统程序打开文件！");
            return;
        }
        this->hide();
    }
}

void FileWidget::onCopyActionTriggered() {
    if (contextMenuFile) {
        QString filePath = contextMenuFile->getFilePath();
        // 在这里添加复制文件的逻辑
    }
}

void FileWidget::onDeleteActionTriggered() {
    if (contextMenuFile) {
        QString filePath = contextMenuFile->getFilePath();
        // 在这里添加删除文件的逻辑
        QFile file(filePath);
        if (file.remove()) {
            qDebug() << "文件删除成功";
        } else {
            qDebug() << "文件删除失败";
        }
    }
}

// 打开“此电脑”
void FileWidget::openThisPC() {
    // 使用Windows Shell API的特殊文件夹标识符
    const QString computerGuid = QStringLiteral("::{20D04FE0-3AEA-1069-A2D8-08002B30309D}");

    // 构造explorer.exe命令参数
    QStringList arguments;
    arguments << computerGuid;

    // 启动资源管理器
    QProcess::startDetached(QStringLiteral("explorer.exe"), arguments);
}
