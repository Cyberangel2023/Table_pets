#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QTextBrowser>

class MainScene : public QWidget
{
public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();

private:
    void createAndWriteFile();
    void addDeskTopFileToTXT1();
    void clearFile();

private:
    QFile* Txt1;
    QFile* Txt2;
};

#endif // MAINSCENE_H
