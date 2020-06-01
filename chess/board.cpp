#include "board.h"

Board::Board()
{
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            boardFigures[i][j] = FIGURES[i][j];
        }
        for (int j = 0; j < 4; j++)
            if (boardFigures[i][j] == noFigure) boardColors[i][j] = noColor;
            else boardColors[i][j] = black;

        for (int j = 4; j < 8; j++)
            if (boardFigures[i][j] == noFigure) boardColors[i][j] = noColor;
            else boardColors[i][j] = white;
    }
}

bool Board::isValidMove(Move m)
{
    if (m.x2 < 0 || m.x2 >= 8 || m.y2 < 0 || m.y2 >= 8 || (m.x2 == m.x1 && m.y2 == m.y1)
            || boardColors[m.x1][m.y1] == boardColors[m.x2][m.y2] ||
            boardFigures[m.x1][m.y1] == noFigure) return false;
    switch (boardFigures[m.x1][m.y1]){
    case king:
        return max(abs(m.x2 - m.x1), abs(m.y2 - m.y1)) == 1 ||
                (boardColors[m.x1][m.y1] == black && !blackKingWasMoving &&
                !leftBlackRookWasMoving && m.x2 == 2 && m.y2 == 0 &&
                boardFigures[1][0] == noFigure && boardFigures[2][0] == noFigure &&
                boardFigures[3][0] == noFigure && !isCheckPoint(white, 3, 0) &&
                !isCheckPoint(white, 4, 0)) ||
                (boardColors[m.x1][m.y1] == black && !blackKingWasMoving &&
                !rightBlackRookWasMoving && m.x2 == 6 && m.y2 == 0 &&
                boardFigures[5][0] == noFigure && boardFigures[6][0] == noFigure &&
                !isCheckPoint(white, 4, 0) && !isCheckPoint(white, 5, 0)) ||
                (boardColors[m.x1][m.y1] == white && !whiteKingWasMoving &&
                !rightWhiteRookWasMoving && m.x2 == 2 && m.y2 == 7 &&
                boardFigures[1][7] == noFigure && boardFigures[2][7] == noFigure &&
                boardFigures[3][7] == noFigure && !isCheckPoint(black, 3, 7) &&
                !isCheckPoint(black, 4, 7)) ||
                (boardColors[m.x1][m.y1] == white && !whiteKingWasMoving &&
                !leftWhiteRookWasMoving && m.x2 == 6 && m.y2 == 7 &&
                boardFigures[5][7] == noFigure && boardFigures[6][7] == noFigure &&
                !isCheckPoint(black, 4 , 7) && !isCheckPoint(black, 5, 7));
    case queen:
        return (m.x1 == m.x2 || m.y1 == m.y2 || abs(m.x2 - m.x1) == abs(m.y2 - m.y1)) &&
                !isBarrier(m.x1, m.y1, m.x2, m.y2);
    case rook:
        return (m.x1 == m.x2 || m.y1 == m.y2) && !isBarrier(m.x1, m.y1, m.x2, m.y2);
    case knight:
        return abs(m.x2 - m.x1) + abs(m.y2 - m.y1) == 3 &&
                m.x2 != m.x1 && m.y2 != m.y1;
    case bishop:
        return abs(m.x2 - m.x1) == abs(m.y2 - m.y1) && !isBarrier(m.x1, m.y1, m.x2, m.y2);
    case pawn:
        if (boardColors[m.x1][m.y1] == white)
            return (m.x2 == m.x1 && (m.y1 - m.y2 == 1 || (m.y1 - m.y2 == 2 &&
                boardFigures[m.x2][m.y2 + 1] == noFigure && m.y1 == 6))
                && boardFigures[m.x2][m.y2] == noFigure) ||
                (abs(m.x2 - m.x1) == 1 && m.y1 - m.y2 == 1 && boardColors[m.x2][m.y2] == black) ||
                (movePawn == m.x2 && abs(m.x1 - m.x2) == 1 && m.y1 == 3 && m.y2 == 2);
        else
            return (m.x2 == m.x1 && (m.y2 - m.y1 == 1 || (m.y2 - m.y1 == 2 &&
                boardFigures[m.x2][m.y2 - 1] == noFigure && m.y1 == 1))
                && boardFigures[m.x2][m.y2] == noFigure) ||
                (abs(m.x2 - m.x1) == 1 && m.y2 - m.y1 == 1 && boardColors[m.x2][m.y2] == white)||
                (movePawn == m.x2 && abs(m.x1 - m.x2) == 1 && m.y1 == 4 && m.y2 == 5);
    default:
        return false;
    }
}

bool Board::isBarrier(int x, int y, int nx, int ny)
{
    int stepi = (x == nx) ? 0 : abs(nx - x) / (nx - x);
    int stepj = (y == ny) ? 0 : abs(ny - y) / (ny - y);
    for (int i = x + stepi, j = y + stepj; i != nx || j != ny; i += stepi, j += stepj){
        if (boardFigures[i][j] != noFigure) return true;
    }
    return false;
}

bool Board::canMove(Move m)
{
    if (!isValidMove(m)) return false;
    Board nBoard;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            nBoard.boardFigures[i][j] = boardFigures[i][j];
            nBoard.boardColors[i][j] = boardColors[i][j];
        }
    }
    nBoard.move(m);
    return !nBoard.isCheck(boardColors[m.x1][m.y1]);
}

bool Board::isCheckPoint(Colors color, int x0, int y0){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (boardColors[i][j] == color && isValidMove({i,j,x0,y0})){
                return true;
            }
        }
    }
    return false;
}

bool Board::isCheck(Colors color)
{
    Colors ncolor;
    if (color == white) ncolor = black; else ncolor = white;
    int x0, y0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (boardFigures[i][j] == king && boardColors[i][j] == color){
                x0 = i;
                y0 = j;
            }
        }
    }
    return isCheckPoint(ncolor, x0, y0);
}

bool Board::isBlock(Colors color){
    for (int x1 = 0; x1 < 8; x1++){
        for (int y1 = 0; y1 < 8; y1++){
            if (boardColors[x1][y1] == color){
                for (int x2 = 0; x2 < 8; x2++){
                    for (int y2 = 0; y2 < 8; y2++){
                        if (canMove({x1, y1, x2, y2})){
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void Board::copy(Board *a)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++){
            boardFigures[i][j] = a->boardFigures[i][j];
            boardColors[i][j] = a->boardColors[i][j];
        }
    blackKingWasMoving = a->blackKingWasMoving;
    whiteKingWasMoving = a->whiteKingWasMoving;
    leftBlackRookWasMoving = a->leftBlackRookWasMoving;
    rightBlackRookWasMoving = a->rightBlackRookWasMoving;
    rightWhiteRookWasMoving = a->rightWhiteRookWasMoving;
    leftWhiteRookWasMoving = a->leftWhiteRookWasMoving;
    movePawn = a->movePawn;
}

bool Board::isMate(Colors color)
{
    return isCheck(color) && isBlock(color);
}

bool Board::isStalemate(Colors color)
{
    return !isCheck(color) && isBlock(color);
}

void Board::move(Move m)
{
    if (m.x1 == 4 && m.y1 == 0) blackKingWasMoving = true;
    else if (m.x1 == 4 && m.y1 == 7) whiteKingWasMoving = true;
    else if ((m.x1 == 0 && m.y1 == 0) || (m.x2 == 0 && m.y2 == 0)) leftBlackRookWasMoving = true;
    else if ((m.x1 == 7 && m.y1 == 0) || (m.x2 == 7 && m.y2 == 0)) rightBlackRookWasMoving = true;
    else if ((m.x1 == 0 && m.y1 == 7) || (m.x2 == 0 && m.y2 == 7)) rightWhiteRookWasMoving = true;
    else if ((m.x1 == 7 && m.y1 == 7) || (m.x2 == 7 && m.y2 == 7)) leftWhiteRookWasMoving = true;

    movePawn = -1;
    if (boardFigures[m.x1][m.y1] == pawn && boardFigures[m.x2][m.y2] == noFigure &&
            abs(m.y1 - m.y2) == 2) movePawn = m.x1;

    if (m.x1 == 4 && m.y1 == 0 && m.x2 ==2 && m.y2 == 0) move({0,0,3,0});
    else if (m.x1 == 4 && m.y1 == 0 && m.x2 == 6 && m.y2 == 0) move({7,0,5,0});
    else if (m.x1 == 4 && m.y1 == 7 && m.x2 == 2 && m.y2 == 7) move({0,7,3,7});
    else if (m.x1 == 4 && m.y1 == 7 && m.x2 == 6 && m.y2 == 7) move({7,7,5,7});


    if (boardFigures[m.x1][m.y1] == pawn && abs(m.x1 - m.x2) == 1 &&
            boardFigures[m.x2][m.y2] == noFigure) boardFigures[m.x2][m.y1] = noFigure;
    boardFigures[m.x2][m.y2] = boardFigures[m.x1][m.y1];
    boardFigures[m.x1][m.y1] = noFigure;
    boardColors[m.x2][m.y2] = boardColors[m.x1][m.y1];
    boardColors[m.x1][m.y1] = noColor;
    if ((m.y2 == 7 || m.y2 == 0) && boardFigures[m.x2][m.y2] == pawn){
        if (boardColors[m.x2][m.y2] == white) boardFigures[m.x2][m.y2] = pawnEnd;
        else boardFigures[m.x2][m.y2] = queen;
    }
}

Move Board::solve(int level)
{
    return solveRec(0, level, 2 * INF, {0, 0, 0, 0}).first;
}

pair<Move, int> Board::solveRec(int hight, int level, int breakPoint, Move maybeGoodMove)
{
    if (hight == level) return {{0, 0, 0, 0}, getValue()};
    bool b = false;
    int value;
    Move bestMove = {0, 0, 0, 0};
    Move secondBestMove = {0, 0, 0, 0};
    if (hight % 2 == 0) value = -INF; else value = INF;
    if ((hight % 2 == 0) != (boardColors[maybeGoodMove.x1][maybeGoodMove.y1] == white))
    {
        if (canMove(maybeGoodMove)){
            b = true;
            Board *nBoard = new Board();
            nBoard->copy(this);
            nBoard->move(maybeGoodMove);
            pair<Move,int> t = nBoard->solveRec(hight + 1, level, value, secondBestMove);
            delete nBoard;
            if ((hight % 2 == 0) == (t.second > value)) {
                secondBestMove = t.first;
                value = t.second;
                bestMove = maybeGoodMove;
            }
            if ((hight % 2 == 0 && value >= breakPoint) ||
                (hight % 2 == 1 && value <= breakPoint)){
                return {bestMove, value};
            }
        }
    }
    for (int x1 = 0; x1 < 8; x1++){
        for (int y1 = 0; y1 < 8; y1++){
            if ( (hight % 2 == 0) != (boardColors[x1][y1] == white))
            {
                for (int x2 = 0; x2 < 8; x2++){
                    for (int y2 = 0; y2 < 8; y2++){
                        if (canMove({x1, y1, x2, y2})){
                            b = true;
                            Board *nBoard = new Board();
                            nBoard->copy(this);
                            nBoard->move({x1,y1,x2,y2});
                            pair<Move,int> t = nBoard->solveRec(hight + 1, level, value,
                                                                secondBestMove);
                            delete nBoard;
                            if ((hight % 2 == 0) == (t.second > value)) {
                                secondBestMove = t.first;
                                value = t.second;
                                bestMove = {x1,y1,x2,y2};
                            }
                            if ((hight % 2 == 0 && value >= breakPoint) ||
                                (hight % 2 == 1 && value <= breakPoint)){
                                return {bestMove, value};
                            }
                        }
                    }
                }
            }
        }
    }
    if (b == false){ // пат или мат
            if (hight % 2 == 0){ // ход черных
                if (isCheck(black))
                    value = -INF / 2 + hight; // мат черным
                else
                    value = 0; // пат черным
            }
            else{ // ход белых
                if (isCheck(white))
                    value = INF / 2 - hight; // мат белым
                else
                    value = 0; // пат белым
            }
    }
    return {bestMove, value};
}

int Board::getValue()
{
    double res = 0;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (boardColors[i][j] == white){
                switch (boardFigures[i][j]){
                case king:     res -= 1000000 + KING[i][7 -j]; break;
                case queen:    res -= 180 + QUEEN[i][7 - j]; break;
                case rook:     res -= 100 + ROOK[i][7 - j]; break;
                case knight:   res -= 60 + KNIGHT[i][7 - j]; break;
                case bishop:   res -= 60 + BISHOP[i][7 - j]; break;
                case pawn:     res -= 20 + PAWN[i][7 - j]; break;
                case noFigure: res -= 0;
                }
            }
            else if (boardColors[i][j] == black){
                switch (boardFigures[i][j]){
                case king:     res += 1000000 + KING[i][j]; break;
                case queen:    res += 180 + QUEEN[i][j]; break;
                case rook:     res += 100 + ROOK[i][j]; break;
                case knight:   res += 60 + KNIGHT[i][j]; break;
                case bishop:   res += 60 + BISHOP[i][j]; break;
                case pawn:     res += 20 + PAWN[i][j]; break;
                case noFigure: res += 0;
                }
            }
        }
    }
    return res;
}
