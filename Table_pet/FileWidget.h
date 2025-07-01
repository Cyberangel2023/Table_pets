#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <QStandardPaths>
#include <QDir>
#include <QList>
#include <QTimer>
#include <QMenu>
#include <QContextMenuEvent>
#include <QProcess>
#include <ShlObj.h> // 链接shell32.lib
#include <comdef.h>
#include <QMessageBox>

#include "MainFile.h"
#include "AnchorPane.h"
#include "ScrollPane.h"

class FileWidget : public QWidget
{
public:
    FileWidget(QWidget *parent = nullptr);
    ~FileWidget();

private:
    void focusOutEvent(QFocusEvent *event) override;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void listDesktopFiles();
    void addSystemSpecialItems();
    void addSpecialItem(const QString &name, const QString &itemPath, QString iconName);
    void addSpecialItem(const QString &name, const QString &itemPath);
    QIcon getIconForFile(const QFileInfo &fileInfo);
    void openThisPC();

private slots:
    void onOpenActionTriggered();
    void onCopyActionTriggered();
    void onDeleteActionTriggered();
    void initSpecialItems();

public:
    // 其他成员函数
    QList<MainFile*> getSelected() {
        return selectedFiles;
    }

    void addSelected(MainFile file) {
        this->selectedFiles.push_back(&file);
    }

    int getSelectedSize() {
        return selectedFiles.size();
    }

    void resetFiles();

public:
    void reset(QPoint position) {
        int posX = position.x() > 400 ? position.x() - 380 : position.x() + 220;
        this->move(posX, position.y() + 70);
    }

private:
    QMap<QString, MainFile*> deskFiles; // 储存桌面文件
    QList<MainFile*> selectedFiles; // 选择的文件

    // 背景板
    AnchorPane* anchorPane;
    ScrollPane* scrollPane;
    QWidget *contentWidget;
    QGridLayout* gridLayout;

    QMenu *contextMenu; // 右键菜单
    MainFile *contextMenuFile; // 用于存储右键点击的 MainFile 指针
};

#endif // FILEWIDGET_H
