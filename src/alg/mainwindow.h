#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pers.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int timer;
    int height = 50, width = 50, size;
    int countAct = 0;
    void timerEvent(QTimerEvent*);
    void paintEvent(QPaintEvent*);
    QVector <int> level;
    QVector <QColor> colors;
    QList <Pers*> pers;
    void tackt(bool view, int &count);
    void newRound();

    int startPer = 64;
    int finishPer = 8;

    void gen();
};

#endif // MAINWINDOW_H
