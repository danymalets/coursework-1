#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include "situation.h"

class Figure : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Figure(Situation *situation, vector<Move> *moves,
                    int x, int y, bool allowMovement, QObject *parent = 0);
signals:
    void sl();
private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    vector<Move> *moves;
    void move(int nx, int ny);
    QGraphicsScene *scene;
    Situation *situation;
    Figures *figures;
    int x, y;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // Figure_H
