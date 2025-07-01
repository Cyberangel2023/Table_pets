#ifndef MAINFILE_H
#define MAINFILE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QFileInfo>
#include <QIcon>
#include <QFileIconProvider>
#include <QCursor>
#include <QPoint>
#include <QRect>
#include <QUrl>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QEvent>
#include <QFontDatabase>
#include <QPaintEvent>
#include <QPainter>

class MainFile : public QWidget {
    Q_OBJECT
public:
    MainFile(const QString &name, const QString& filePath, QWidget *parent = nullptr)
        : QWidget(parent), fileName(name), filePath(filePath), selected(false) {
        this->Layout = new QVBoxLayout(this);
        this->Layout->setContentsMargins(0, 0, 0, 0);
        this->Layout->setSpacing(0);

        this->setMouseTracking(true);
        this->setAttribute(Qt::WA_Hover);
        setFocusPolicy(Qt::NoFocus); // 禁止子组件传递焦点

        // 设置图标
        initIcon();
        // 设置名称
        setFileName();

        setLayout(this->Layout);
    }

    ~MainFile() {
        delete iconLabel;
        delete nameLabel;
    }

signals:
    void clicked();

protected:
    void enterEvent(QEnterEvent *event) override {
        if (!selected) {
            //setStyleSheet("background-color: rgba(255, 255, 255, 32);");
        }
        QWidget::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override {
        if (!selected) {
            //setStyleSheet("background: transparent;");
        }
        QWidget::leaveEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
            updateStyle();
        }
        QWidget::mousePressEvent(event);
    }

private slots:
    void updateStyle() {
        if (selected) {
            //setStyleSheet("background-color: rgba(255, 255, 255, 64);");
        } else {
            //setStyleSheet("background: transparent;");
        }
    }

private:
    void setFileName() {
        // 设置名称
        this->nameLabel = new QLabel(this->fileName, this);

        // 基础样式表（不含对齐方式）
        QString style = R"(
        color: white;
        background: transparent;
        font-family: Microsoft YaHei, Segoe UI, Arial;
        font-size: 10px;
        padding: 0 4px;
        line-height: 10px;
        min-height: 10px;
        max-height: 10px;
        border: none;
        border-radius: 2px;
        letter-spacing: 0.5px;
        font-weight: 400;
        white-space: nowrap;
    )";

        // 高性能字体配置
        QFont font("Microsoft YaHei", 10, QFont::Normal);
        font.setPixelSize(10);
        font.setHintingPreference(QFont::PreferNoHinting);
        nameLabel->setFont(font);

        // 精确尺寸控制
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        nameLabel->setSizePolicy(sizePolicy);
        nameLabel->setMinimumSize(QSize(80, 10));
        nameLabel->setMaximumSize(QSize(80, 10));

        // 智能对齐计算
        QFontMetrics fm(font);
        int textWidth = fm.horizontalAdvance(this->fileName);
        int labelWidth = nameLabel->width();

        // 动态样式生成
        style += (textWidth <= labelWidth)
                     ? "text-align: center;"
                     : "text-align: left;";

        nameLabel->setStyleSheet(style);

        // 布局优化
        Layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        nameLabel->setAlignment(Qt::AlignCenter);

        // 智能布局策略
        if (textWidth > labelWidth - 20) {
            nameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            Layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        }

        Layout->addWidget(nameLabel);
    }

    void initIcon() {
        this->iconLabel = new QLabel();
        // 设置图片
        QPixmap pixmap;
        QFileInfo fileInfo(filePath);
        QFileIconProvider iconProvider;
        icon = iconProvider.icon(fileInfo);

        // 强制图标大小
        pixmap = icon.pixmap(QSize(60, 60));
        iconLabel->setMaximumSize(80, 60);
        iconLabel->setMinimumSize(80, 60);
        iconLabel->setPixmap(pixmap);

        // 设置背景透明
        iconLabel->setStyleSheet("background: transparent;");
        // 图片居中显示
        iconLabel->setAlignment(Qt::AlignCenter);

        Layout->addWidget(iconLabel);
    }

    void setIcon() {
        // 强制图标大小
        QFileIconProvider iconProvider;
        if (icoName == "computer")
        {
            icon = iconProvider.icon(QFileIconProvider::Computer);
        } else if (icoName == "user-trash") {
            icon = iconProvider.icon(QFileIconProvider::Trashcan);
        } else {
            icon = QIcon::fromTheme(icoName);
        }

        QPixmap pixmap = icon.pixmap(QSize(60, 60));
        iconLabel->setPixmap(pixmap);
    }

public:
    void setSelected(bool state)
    {
        selected = state;
        updateStyle();
    }

    bool isSelected() const
    {
        return selected;
    }

    void reset() {
        //setStyleSheet("background: transparent;");
        this->selected = false;
    }

    QString getFilePath() {
        return this->filePath;
    }

    void setFileName(QString name) {
        this->fileName = name;
        this->setFileName();
    }

    void setIcon(QString iconName) {
        this->icoName = iconName;
        this->setIcon();
    }

    QString getFilePath() const {
        return this->filePath;
    }

    void setFilePath(const QString &filePath) {
        this->filePath = filePath;
    }

    QString getFileName() {
        return this->fileName;
    }

private:
    QString fileName;
    QString filePath;
    QString icoName;
    bool selected;
    QIcon icon;
    QLabel* iconLabel;
    QLabel* nameLabel;
    QVBoxLayout* Layout;
};

#endif // MAINFILE_H
