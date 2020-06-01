#include "figure.h"

Figure::Figure(Board *board, int x, int y, bool allowMovement, Colors userColor, QObject *parent):
    QObject(parent), QGraphicsItem()
{
    this->board = board;
    this->userColor = userColor;
    this->x = x;
    this->y = y;
    figure = board->boardFigures[x][y];
    color = board->boardColors[x][y];
    if (userColor == white) setPos(-350 + x * 100, -350 + y * 100);
    else setPos(350 - x * 100, -350 + y * 100);
    if (allowMovement && board->boardColors[x][y] == white) setFlag(ItemIsMovable);
}

QRectF Figure::boundingRect() const
{
    return QRectF(-50, -50, 100, 100);
}

void Figure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush;
    if ((color == black) == userColor){
        painter->setPen(QPen(Qt::black, 4));
    }
    else{
        painter->setPen(QPen(Qt::white, 4));
    }
    switch (figure){
    case king:
        painter->drawLine(-10, -40,  10, -40);
        painter->drawLine( 10, -40,  10, -10);
        painter->drawLine( 10, -10,  40, -10);
        painter->drawLine( 40, -10,  40,  10);
        painter->drawLine( 40,  10,  10,  10);
        painter->drawLine( 10,  10,  10,  40);
        painter->drawLine( 10,  40, -10,  40);
        painter->drawLine(-10,  40, -10,  10);
        painter->drawLine(-10,  10, -40,  10);
        painter->drawLine(-40,  10, -40, -10);
        painter->drawLine(-40, -10, -10, -10);
        painter->drawLine(-10, -10, -10, -40);
        break;
    case queen:
        painter->drawEllipse(-40, -40, 80, 80);
        painter->drawEllipse(-10, -10, 20, 20);
        break;
    case rook:
        painter->drawLine(-25, -40,  25, -40);
        painter->drawLine( 25, -40,  25, -20);
        painter->drawLine( 25, -20,  18, -20);
        painter->drawLine( 18, -20,  18,  20);
        painter->drawLine( 18,  20,  25,  20);
        painter->drawLine( 25,  20,  25,  40);
        painter->drawLine( 25,  40, -25,  40);
        painter->drawLine(-25,  40, -25,  20);
        painter->drawLine(-25,  20, -18,  20);
        painter->drawLine(-18,  20, -18, -20);
        painter->drawLine(-18, -20, -25, -20);
        painter->drawLine(-25, -20, -25, -40);
        break;
    case knight:
        painter->drawLine(-25, -40,  25, -40);
        painter->drawLine( 25, -40,  25,  40);
        painter->drawLine( 25,  40, -25,  40);
        painter->drawLine(-25,  40, -25,  20);
        painter->drawLine(-25,  20,  10, -20);
        painter->drawLine( 10, -20, -25, -20);
        painter->drawLine(-25, -20, -25, -40);
        break;
    case bishop:
        painter->drawLine(  0, -40,  30,  40);
        painter->drawLine( 30,  40, -30,  40);
        painter->drawLine(-30,  40,   0, -40);
        break;
    case pawn:
        painter->drawRect(-20, -20, 40, 40);
        break;
    default:
        break;
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Figure::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    QPointF point = scenePos();
    int nx, ny;
    nx = (point.x() + 400) / 100;
    if (userColor == black) nx = 7 - nx;
    ny = (point.y() + 400) / 100;
    if (board->canMove({x, y, nx, ny})){
        board->move({x, y, nx, ny});
        wasMove = true;
        if (userColor == white) move = {x, y, nx, ny};
        else move = {x, 7 - y, nx, 7 - ny};
        emit moveSignal();
    }
    else{
        if (userColor == white) setPos(-350 + x * 100, -350 + y * 100);
        else setPos(350 - x * 100, -350 + y * 100);
    }
}
