#include "pers.h"
#include <QDebug>

Pers::Pers(int position, int height, int size)
{
    mPosition = position;
    hp = 50;
    mSize = size;
    for(int i = 0; i < 64; ++i)
        mBrain.append(qrand() & 63);
    arrOr[0] = 1;
    arrOr[1] = 1 - height;
    arrOr[2] = - height;
    arrOr[3] = - height - 1;
    arrOr[4] = -1;
    arrOr[5] = height - 1;
    arrOr[6] = height;
    arrOr[7] = height + 1;

    arrLe[0] = 2;
    arrLe[1] = 2 - height;
    arrLe[2] = 2 - 2 * height;
    arrLe[3] = 1 - 2 * height;
    arrLe[4] = -2 * height;
    arrLe[5] = -1 -2 * height;
    arrLe[6] = -2 -2 * height;
    arrLe[7] = -2 - height;

    arrRi[0] = -2;
    arrRi[1] = height - 2;
    arrRi[2] = 2 * height - 2;
    arrRi[3] = 2 * height - 1;
    arrRi[4] = 2 * height;
    arrRi[5] = 2 * height + 1;
    arrRi[6] = 2 * height + 2;
    arrRi[7] = height + 2;
}

int Pers::act(QVector<int> &level, int &position, int attempt)
{
    if(++attempt >= 10) {
        mark = (mark + 1) & 63;
        --hp;
        if(hp <= 0) {
            level.replace(mPosition, 0);
            return 1;
        }
        return 0;
    }

    switch(mBrain[mark]) {
    case 0: { //шаг
        switch(level.value((mPosition + arrOr[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize)) {
        case 0: {
            level.replace(mPosition, 0);
            mPosition = (mPosition + arrOr[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize;
            level.replace(mPosition, 1);

            mark = (mark + mBrain[(mark + 2) & 63]) & 63;

            break;
        }
        case 1: {
            mark = (mark + mBrain[(mark + 3) & 63]) & 63;

            break;
        }

        case 2: {
            mark = (mark + mBrain[(mark + 4) & 63]) & 63;

            break;
        }

        case 3: {
            level.replace(mPosition, 0);
            mPosition = (mPosition + arrOr[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize;
            level.replace(mPosition, 1);

            mark = (mark + mBrain[(mark + 5) & 63]) & 63;

            hp += 20;

            if(hp > 128) {
                hp = 0;
                level.replace(mPosition, 0);

                return 1;
            }

            return 2;
        }

        case 4: {
            hp = 0;
            level.replace(mPosition, 0);

            return 1;
        }
        }
        break;
    }
    case 1: { //осмотр
        mark = (mark + level.value((mPosition + arrOr[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize)) & 63;
        return act(level, position, attempt);
    }
    case 2: { //поворот
        orientation += mBrain[(mark + 1) & 63];
        mark = (mark + 2) & 63;
        return act(level, position, attempt);
    }
    case 3: { //взять
        switch(level.value((mPosition + arrOr[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize)) {
        case 0: {
            mark = (mark + mBrain[(mark + 2) & 63]) & 63;

            break;
        }
        case 1: {
            mark = (mark + mBrain[(mark + 3) & 63]) & 63;

            break;
        }

        case 2: {
            mark = (mark + mBrain[(mark + 4) & 63]) & 63;

            break;
        }

        case 3: {
            level.replace((mPosition + arrOr[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize, 0);

            mark = (mark + mBrain[(mark + 5) & 63]) & 63;

            hp += 20;

            if(hp > 128) {
                hp = 0;
                level.replace(mPosition, 0);

                return 1;
            }

            return 2;
        }

        case 4: {
            level.replace((mPosition + arrOr[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize, 3);

            mark = (mark + mBrain[(mark + 6) & 63]) & 63;

            return 3;
        }
        }
        break;
    }
    case 4: { //дальний осмотр слева
        mark = (mark + level.value((mPosition + arrLe[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize)) & 63;
        return act(level, position, attempt);
    }
    case 5: { //дальний осмотр справа
        mark = (mark + level.value((mPosition + arrRi[(orientation + mBrain[(mark + 1) & 63]) & 7] + mSize) % mSize)) & 63;
        return act(level, position, attempt);
    }
    case 6: { //сколько hp
        if(hp > mBrain[(mark + 1) & 63] * 2)
            mark = (mark + mBrain[(mark + 2) & 63]) & 63;
        else
            mark = (mark + mBrain[(mark + 3) & 63]) & 63;
        return act(level, position, attempt);
    }
    default: {
        mark = (mark + mBrain[mark]) & 63;
        return act(level, position, attempt);
    }
    }
    --hp;

    if(hp <= 0) {
        level.replace(mPosition, 0);
        return 1;
    }
    return 0;
}

int Pers::position()
{
    return mPosition;
}

void Pers::restart(int position)
{
    mPosition = position;
    hp = 50;
    mark = 0;
    orientation = 0;
}

void Pers::setBrain(QVector<int> brain)
{
    for(int i = 0; i < brain.size(); ++i)
        if(qrand() & 7)
            mBrain[i] = brain[i];
        else
            mBrain[i] = qrand() & 63;
}

QVector<int> Pers::brain()
{
    return mBrain;
}
