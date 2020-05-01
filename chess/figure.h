#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>
#include "situation.h"

class Figure : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Figure(QGraphicsScene *scene, Situation *situation, vector<Move> *moves, int x, int y, bool s, QObject *parent = 0);
    Figure *leftRook, *rightRook;
    ~Figure();
    vector<Move> *moves;
    bool IsMove = false;
    void showMove(int nx,int ny);
    void move(int nx, int ny);
signals:

    void sl();

protected:

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QGraphicsScene *scene;
    Situation *situation;
    Figures *figures;
    int x, y;
    bool wasMove = false;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private slots:
    //void sl();
};

#endif // Figure_H
