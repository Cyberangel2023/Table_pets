#ifndef ANCHORPANE_H
#define ANCHORPANE_H

#include <QWidget>
#include <QVBoxLayout>

class AnchorPane : public QWidget {
    Q_OBJECT
public:
    explicit AnchorPane(QWidget *parent = nullptr) : QWidget(parent) {
        // 设置灰色半透明背景
        setStyleSheet("background-color: rgba(200, 200, 200, 64); border-radius: 8px;");
        setAttribute(Qt::WA_TranslucentBackground); // 启用背景透明

        // 设置布局（可选）
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        setFocusPolicy(Qt::NoFocus); // 禁止子组件传递焦点
    }
};


#endif // ANCHORPANE_H
