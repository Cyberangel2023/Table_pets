#ifndef MAINFILE_H
#define MAINFILE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QMessageBox>
#include <QFileInfo>
#include <QIcon>
#include <QFileIconProvider>
#include <QCursor>
#include <QPoint>
#include <QRect>
#include <QUrl>
#include <QDesktopServices>

class MainFile : public QLabel {
    Q_OBJECT
public:
    MainFile(const QString &name, const QString& filePath, QWidget *parent = nullptr)
        : QLabel(parent), filePath(filePath), selected(false) {
        // 设置图片
        QPixmap pixmap;
        QFileInfo fileInfo(filePath);
        QFileIconProvider iconProvider;
        QIcon icon = iconProvider.icon(fileInfo);

        // 强制图标大小
        pixmap = icon.pixmap(QSize(60, 60));
        setMinimumSize(60, 60);
        setMaximumSize(60, 60);
        setPixmap(pixmap);

        // 设置背景透明
        setStyleSheet("background: transparent;");
        // 图片居中显示
        setAlignment(Qt::AlignCenter);

        // 设置点击事件
        setMouseTracking(true);
        setAttribute(Qt::WA_Hover);
        connect(this, &MainFile::hovered, this, &MainFile::onHovered);

        setFocusPolicy(Qt::NoFocus); // 禁止子组件传递焦点
    }

    QString getFilePath() const {
        return this->filePath;
    }

    void setFilePath(const QString &filePath) {
        this->filePath = filePath;
    }

protected:
    bool event(QEvent *event) override {
        if (event->type() == QEvent::HoverEnter) {
            emit hovered();
        } else {
            if (!this->selected) {
                if (event->type() == QEvent::HoverLeave) {
                    setStyleSheet("background: transparent;");
                } else if (event->type() == QEvent::Wheel) {
                    setStyleSheet("background: transparent;");
                }
            } else {
                if (event->type() == QEvent::HoverLeave) {
                    setStyleSheet("background-color: rgba(255, 255, 255, 64);");
                } else if (event->type() == QEvent::Wheel) {
                    setStyleSheet("background-color: rgba(255, 255, 255, 64);");
                }
            }
        }
        return QLabel::event(event);
    }

signals:
    void hovered();

private slots:
    void onHovered() {
        // 在这里处理鼠标悬停事件
        setStyleSheet("background-color: rgba(255, 255, 255, 32);");
    }

public:
    void setSelected(bool selected)
    {
        this->selected = selected;
        // 更新 UI 以显示选中状态，例如改变背景色
        if (this->selected) {
            setStyleSheet("background-color: rgba(255, 255, 255, 64);");
        } else {
            setStyleSheet("background: transparent;");
        }
    }

    bool isSelected() const
    {
        return selected;
    }

    void reset() {
        setStyleSheet("background: transparent;");
        this->selected = false;
    }

    QString getFilePath() {
        return this->filePath;
    }

private:
    QString filePath;
    bool selected;
};

#endif // MAINFILE_H
