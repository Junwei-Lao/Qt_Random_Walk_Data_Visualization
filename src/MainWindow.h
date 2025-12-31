#pragma once
#include <QMainWindow>

class FileListWidget;
class AnimationWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    FileListWidget *fileList;
    AnimationWidget *animation;
};
