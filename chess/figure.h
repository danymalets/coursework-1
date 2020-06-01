#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include "board.h"

class Figure : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    Figure(Board *board, int x, int y, bool allowMovement, Colors userColor,
                    QObject *parent = 0);

    bool wasMove = false;
    Move move;

private:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    Colors userColor;
    Board *board;
    int x, y;
    Figures figure;
    Colors color;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void moveSignal();
};

#endif // Figure_H
