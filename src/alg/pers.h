#ifndef PERS_H
#define PERS_H

#include <QVector>

class Pers
{
public:
    Pers(int position, int height, int size);
    int act(QVector <int> &level, int &position, int attempt = 0);
    int position();
    void restart(int position);
    void setBrain(QVector <int> brain);
    QVector <int> brain();

private:
    int mPosition;
    QVector <int> mBrain;
    int mSize;
    int mark = 0;
    int hp;
    int orientation = 0;
    int arrOr[8];
    int arrLe[8];
    int arrRi[8];
};

#endif // PERS_H
