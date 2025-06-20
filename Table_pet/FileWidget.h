#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QWidget>
#include <QStandardPaths>
#include <QDir>
#include <QList>
#include <QTimer>

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
    void listDesktopFiles();

public:
    void reset(QPoint position) {
        this->move(position.x(), position.y() - 200);
    }

private:
    QMap<QString, MainFile*> deskFiles; // 储存桌面文件
    QList<MainFile*> selectedFiles; // 选择的文件

    // 背景板
    AnchorPane* anchorPane;
    ScrollPane* scrollPane;
};

#endif // FILEWIDGET_H
