#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MainScene.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QVector>


class MainWindow : public QMainWindow
{
Q_OBJECT

private :
    MainScene* mainScene;
    QGraphicsView* mainView;

public:
    static int const EXIT_CODE_REBOOT;
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() {
    };

public slots:
    void slotReboot();
    void slotQuit();
};

#endif // MAINWINDOW_H
