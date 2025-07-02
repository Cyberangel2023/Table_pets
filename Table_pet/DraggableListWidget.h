#ifndef DRAGGABLELISTWIDGET_H
#define DRAGGABLELISTWIDGET_H

#include <QListWidget>
#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QByteArray>

#include "DraggableTreeView.h"

class DraggableListWidget : public QListWidget {
public:
    DraggableListWidget(QWidget *parent = nullptr) : QListWidget(parent) {
        setAcceptDrops(true);          // 允许接受拖放
        setDragDropMode(QAbstractItemView::InternalMove); // 内部拖放
        setSelectionMode(QAbstractItemView::SingleSelection);
    }

    // 处理拖拽进入事件
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
            event->acceptProposedAction();
        }
    }

    // 处理拖拽放下事件
    void dropEvent(QDropEvent *event) override {
        if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
            QByteArray data = event->mimeData()->data("application/x-dnditemdata");
            QString text = QString::fromUtf8(data);

            // 添加到队列
            addItem(text);

            // 删除原位置项（如果来自目录树）
            if (sender() && sender()->objectName() == "treeView") {
                DraggableTreeView *tree = qobject_cast<DraggableTreeView*>(sender());
                tree->model()->removeRow(tree->currentIndex().row());
            }

            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->ignore();
        }
    }
};

#endif // DRAGGABLELISTWIDGET_H
