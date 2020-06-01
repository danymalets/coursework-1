#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QGraphicsItem>
using namespace std;

const int INF = (int)1e9;

struct Move{
    int x1, y1, x2, y2;
};

enum Figures{
    king,
    queen,
    rook,
    knight,
    bishop,
    pawn,
    noFigure
};

enum Colors{
    black,
    white,
    noColor
};

const Figures FIGURES[8][8] = {
    {  rook, pawn, noFigure, noFigure, noFigure, noFigure, pawn,   rook},
    {knight, pawn, noFigure, noFigure, noFigure, noFigure, pawn, knight},
    {bishop, pawn, noFigure, noFigure, noFigure, noFigure, pawn, bishop},
    { queen, pawn, noFigure, noFigure, noFigure, noFigure, pawn,  queen},
    {  king, pawn, noFigure, noFigure, noFigure, noFigure, pawn,   king},
    {bishop, pawn, noFigure, noFigure, noFigure, noFigure, pawn, bishop},
    {knight, pawn, noFigure, noFigure, noFigure, noFigure, pawn, knight},
    {  rook, pawn, noFigure, noFigure, noFigure, noFigure, pawn,   rook}
};

const int KING[8][8] = {
    {   4,   4,  -2,  -4,  -6,  -6,  -6,  -6},
    {   6,   4,  -4,  -6,  -8,  -8,  -8,  -8},
    {   2,   0,  -4,  -6,  -8,  -8,  -8,  -8},
    {   0,   0,  -4,  -8, -10, -10, -10, -10},
    {   0,   0,  -4,  -8, -10, -10, -10, -10},
    {   2,   0,  -4,  -6,  -8,  -8,  -8,  -8},
    {   6,   4,  -4,  -6,  -8,  -8,  -8,  -8},
    {   4,   4,  -2,  -4,  -6,  -6,  -6,  -6}
};

const int QUEEN[8][8] = {
    {  -4,  -2,  -2,  -1,  -1,  -2,  -2,  -4},
    {  -2,   0,   0,   0,   0,   0,  -0,  -2},
    {  -2,   0,   1,   1,   1,   1,   0,  -2},
    {  -1,   0,   1,   1,   1,   1,   0,  -1},
    {  -1,   0,   1,   1,   1,   1,   0,  -1},
    {  -2,   0,   1,   1,   1,   1,   1,  -2},
    {  -2,   0,   0,   0,   0,   1,   0,  -2},
    {  -4,  -2,  -2,  -1,   0,  -2,  -2,  -4}
};

const int ROOK[8][8] = {
    {   0,  -1,  -1,  -1,  -1,  -1,   1,   0},
    {   0,   0,   0,   0,   0,   0,   2,   0},
    {   0,   0,   0,   0,   0,   0,   2,   0},
    {   1,   0,   0,   0,   0,   0,   2,   0},
    {   1,   0,   0,   0,   0,   0,   2,   0},
    {   0,   0,   0,   0,   0,   0,   2,   0},
    {   0,   0,   0,   0,   0,   0,   2,   0},
    {   0,  -1,  -1,  -1,  -1,  -1,   1,   0}
};

const int BISHOP[8][8] = {
    {  -4,  -2,  -2,  -2,  -2,  -2,  -2,  -4},
    {  -2,   1,   2,   0,   1,   0,   0,  -2},
    {  -2,   0,   2,   2,   1,   1,   0,  -2},
    {  -2,   0,   2,   2,   2,   2,   0,  -2},
    {  -2,   0,   2,   2,   2,   2,   0,  -2},
    {  -2,   0,   2,   2,   1,   1,   0,  -2},
    {  -2,   1,   2,   0,   1,   0,   0,  -2},
    {  -4,  -2,  -2,  -2,  -2,  -2,  -2,  -4}
};

const int KNIGHT[8][8] = {
    { -10,  -8,  -6,  -6,  -6,  -6,  -8, -10},
    {  -8,  -4,   1,   0,   1,   0,  -4,  -8},
    {  -6,   0,   2,   3,   3,   2,   0,  -6},
    {  -6,   1,   3,   4,   4,   3,   0,  -6},
    {  -6,   1,   3,   4,   4,   3,   0,  -6},
    {  -6,   0,   2,   3,   3,   2,   0,  -6},
    {  -8,  -4,   1,   0,   1,   0,  -4,  -8},
    { -10,  -8,  -6,  -6,  -6,  -6,  -8, -10}
};

const int PAWN[8][8] = {
    {   0,   1,   1,   0,   1,   2,  10,   0},
    {   0,   2,  -1,   0,   1,   2,  10,   0},
    {   0,   2,  -2,   0,   2,   4,  10,   0},
    {   0,  -4,   0,   4,   5,   6,  10,   0},
    {   0,  -4,   0,   4,   5,   6,  10,   0},
    {   0,   2,  -2,   0,   2,   4,  10,   0},
    {   0,   2,  -1,   0,   1,   2,  10,   0},
    {   0,   1,   1,   0,   1,   2,  10,   0}
};

class Board
{
public:
    Board();

    bool canMove(Move m);
    void move(Move m);

    Figures boardFigures[8][8];
    Colors boardColors[8][8];

    void copy(Board *a);

    bool isStalemate(Colors color);
    bool isCheck(Colors color);
    bool isMate(Colors color);

    Move solve(int level);

    Figures pawnEnd = queen;

private:
    int movePawn = -1;
    bool blackKingWasMoving = false,
         whiteKingWasMoving = false,
         leftBlackRookWasMoving = false,
         rightBlackRookWasMoving = false,
         leftWhiteRookWasMoving = false,
         rightWhiteRookWasMoving = false;

    bool isBarrier(int x, int y, int nx, int ny);
    bool isValidMove(Move m);

    bool isBlock(Colors color);
    bool isCheckPoint(Colors ncolor, int x0, int y0);

    pair<Move,int> solveRec(int hight, int level, int breakPoint, Move maybeGoodMove);

    int getValue();
};

#endif // BOARD_H
