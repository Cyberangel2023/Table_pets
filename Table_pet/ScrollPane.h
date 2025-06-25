#ifndef SCROLLPANE_H
#define SCROLLPANE_H

#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QWheelEvent>
#include <QScrollBar>

class ScrollPane : public QScrollArea {
    Q_OBJECT
public:
    explicit ScrollPane(QWidget* parent = nullptr) : QScrollArea(parent), leftHide(0) {
        // 滚动策略设置
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 禁用水平滚动
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);     // 启用垂直滚动
        setWidgetResizable(true);

        // 滚动条样式（可选隐藏）
        setStyleSheet(R"(
            QScrollBar:vertical {
                width: 0px;  /* 完全隐藏垂直滚动条 */
            }
            QScrollBar::handle:vertical {
                background: transparent;
            }
        )");

        // 创建内容容器
        QWidget* scrollContent = new QWidget(this);
        QGridLayout* gridLayout = new QGridLayout(scrollContent);
        gridLayout->setContentsMargins(0, 0, 0, 0);  // 移除布局内边距
        gridLayout->setSpacing(0);                   // 移除布局间距

        // 设置列宽自适应
        for (int i = 0; i < 5; i++) {
            gridLayout->setColumnStretch(i, 1);
        }

        setAlignment(Qt::AlignTop | Qt::AlignLeft);  // 强制内容左上角对齐

        setWidget(scrollContent);
        setFocusPolicy(Qt::NoFocus);  // 禁止焦点传递
    }

protected:
    void wheelEvent(QWheelEvent* event) override {
        // 修改滚轮事件为垂直滚动
        if (event->angleDelta().y() != 0) {
            int scrollAmount = event->angleDelta().y() > 0 ? -40 : 40;
            verticalScrollBar()->setValue(verticalScrollBar()->value() + scrollAmount);
            event->accept();
        } else {
            QScrollArea::wheelEvent(event);
        }
    }

public:
    int getLeftHide() {
        return this->leftHide;
    }

    void resetValue() {
        verticalScrollBar()->setValue(0);
    }

private:
    int leftHide;
};

#endif // SCROLLPANE_H
