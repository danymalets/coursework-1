#include "figure.h"

Figure::Figure(QGraphicsScene *scene, Situation *situation, vector<Move> *moves,
               int x, int y, bool s, QObject *parent): QObject(parent), QGraphicsItem()
{
    this->scene = scene;
    this->situation = situation;
    this->x = x;
    this->y = y;
    this->moves = moves;
    setPos(-350 + x * 100, -350 + y * 100);

    if (s && situation->boardColors[x][y] == white)
        setFlag(ItemIsMovable);
}

Figure::~Figure()
{

}

QRectF Figure::boundingRect() const
{

    return QRectF(-50, -50, 100, 100);
}

void Figure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush brush;
    if (situation->boardColors[x][y] == black){
        painter->setPen(QPen(Qt::black, 4));
    }
    else{
        painter->setPen(QPen(Qt::white, 4));
    }
    switch (situation->boardFigures[x][y]){
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
        painter->drawLine(-25, -40, 25, -40);
        painter->drawLine(25, -40, 25, -20);
        painter->drawLine(25, -20, 18, -20);
        painter->drawLine(18, -20, 18, 20);
        painter->drawLine(18, 20, 25, 20);
        painter->drawLine(25, 20, 25, 40);
        painter->drawLine(25, 40, -25, 40);
        painter->drawLine(-25, 40, -25, 20);
        painter->drawLine(-25, 20, -18, 20);
        painter->drawLine(-18, 20, -18, -20);
        painter->drawLine(-18, -20, -25, -20);
        painter->drawLine(-25, -20, -25, -40);
        break;
    case knight:
        painter->drawLine(-25, -40, 25, -40);
        painter->drawLine(25, -40, 25, 40);
        painter->drawLine(25, 40, -25, 40);
        painter->drawLine(-25, 40, -25, 20);
        painter->drawLine(-25, 20, 10, -20);
        painter->drawLine(10, -20, -25, -20);
        painter->drawLine(-25, -20, -25, -40);
        break;
    case bishop:
        painter->drawLine(0, -40, 30, 40);
        painter->drawLine(30, 40, -30, 40);
        painter->drawLine(-30, 40, 0, -40);
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
    ny = (point.y() + 400) / 100;
    if (situation->canMove({x, y, nx, ny})){
        situation->move({x,y,nx,ny});
        moves->push_back({x,y,nx,ny});
        emit sl();
    }
    else{
        setPos(-350 + x * 100, -350 + y * 100);
    }
}
