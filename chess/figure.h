#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include "board.h"

class Figure : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Figure(Board *board, vector<Move> *moves,
        int x, int y, bool allowMovement, Colors userColor, QObject *parent = 0);
signals:
    void sl();
private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    vector<Move> *moves;
    QGraphicsScene *scene;
    Board *board;
    Figures *figures;
    Colors userColor;
    int x, y;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // Figure_H
