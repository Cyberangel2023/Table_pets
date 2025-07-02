#ifndef DRAGGABLETREEVIEW_H
#define DRAGGABLETREEVIEW_H

#include <QTreeView>
#include <QDrag>
#include <QMimeData>

class DraggableTreeView : public QTreeView {
public:
    DraggableTreeView(QWidget *parent = nullptr) : QTreeView(parent) {
        setDragEnabled(true);           // 启用拖拽
        setAcceptDrops(true);          // 允许接受拖放
        setDropIndicatorShown(true);    // 显示拖放指示器
        setSelectionMode(QAbstractItemView::SingleSelection); // 单选模式
    }

    // 处理拖拽开始事件
    void startDrag(Qt::DropActions supportedActions) override {
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        // 获取选中项的路径
        QModelIndex index = currentIndex();
        if (index.isValid()) {
            QString path = index.data().toString();
            mimeData->setText(path);
            mimeData->setData("application/x-dnditemdata", path.toUtf8());
        }

        drag->setMimeData(mimeData);
        drag->exec(Qt::MoveAction);
    }
};

#endif // DRAGGABLETREEVIEW_H
