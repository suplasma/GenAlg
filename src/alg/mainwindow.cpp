#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());

    size = width * height;

    for(int i = 0; i < size; ++i)
        level.append(0);

    colors.append(Qt::white);
    colors.append(Qt::blue);
    colors.append(Qt::gray);
    colors.append(Qt::green);
    colors.append(Qt::red);

    gen();

    for(int i = 0; i < startPer; ++i) {
        int pos;
        do
        {
            pos = qrand() % size;
        }while(level.value(pos));
        pers.append(new Pers(pos, height, size));
        level.replace(pos, 1);
    }
    int t = 0;

    while(t < 100) {
        tackt(false, t);
        if(countAct > 10000) {
            gen();
            newRound();
            qDebug() << "END";

            break;
        }
    }

    timer = startTimer(100);
}

MainWindow::~MainWindow()
{
    delete ui;
    killTimer(timer);
}

void MainWindow::timerEvent(QTimerEvent *)
{
    int t;
    tackt(true, t);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    for(int i = 0; i < width; ++i)
        for(int j = 0; j < height; ++j) {
            painter.setBrush(QBrush(colors[level.value(j + i * height)], Qt::SolidPattern));
            painter.drawRect(i * 8, j * 8, 8, 8);
        }
}

void MainWindow::tackt(bool view, int &count)
{
    int pos;
    ++countAct;
    int count3;
    for(int i = 0; i < pers.size(); ++i)
        switch (pers.value(i)->act(level, pos)) {
        case 1:
            delete pers.takeAt(i);
            --i;
            if(pers.size() <= finishPer) {

                if(view)
                    repaint();

                qDebug() << countAct;

                countAct = 0;
                ++count;

                gen();

                newRound();

                return;
            }

            break;

        case 4:
            pers.append(new Pers(pos, height, size));
            pers.back()->setBrain(pers.value(i)->brain());
            level.replace(pos, 1);
        case 2:
            count3 = 0;
            do
            {
                pos = qrand() % size;
                ++count3;
                if(count3 > 1000)
                    break;
            }while(level.value(pos));
            if(count3 <= 1000)
                level.replace(pos, 3);

            break;

        case 3:
            count3 = 0;
            do
            {
                pos = qrand() % size;
                ++count3;
                if(count3 > 1000)
                    break;
            }while(level.value(pos) != 3);
            if(count3 <= 1000)
                level.replace(pos, 4);

            break;
        }

    if(view)
        repaint();
}

void MainWindow::newRound()
{
    int i = 0;
    int persSize = pers.size();
    for(; i < persSize; ++i) {
        int pos;
        do
        {
            pos = qrand() % size;
        }while(level.value(pos));
        pers.value(i)->restart(pos);
        level.replace(pos, 1);
    }

    for(; i < startPer; ++i) {
        int pos;
        do
        {
            pos = qrand() % size;
        }while(level.value(pos));
        Pers *p = new Pers(pos, height, size);
        p->setBrain(pers.value(qrand() % persSize)->brain());
        pers.append(p);
        level.replace(pos, 1);
    }
}

void MainWindow::gen()
{
    for(int i = 0; i < size; ++i)
        level.replace(i, 0);

    for(int i = 0; i < 200; ++i) {
        int pos;
        do
        {
            pos = qrand() % size;
        }while(level.value(pos));
        level.replace(pos, 2);
    }

    for(int i = 0; i < 450; ++i) {
        int pos;
        do
        {
            pos = qrand() % size;
        }while(level.value(pos));
        level.replace(pos, 3);
    }

    for(int i = 0; i < 75; ++i) {
        int pos;
        do
        {
            pos = qrand() % size;
        }while(level.value(pos));
        level.replace(pos, 4);
    }
}
