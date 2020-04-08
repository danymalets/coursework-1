#include "situation.h"

struct Vertex{
    Situation situation;
    int hight, parent, value, child;
    Move m;
};

Situation::Situation()
{

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            boardFigures[i][j] = FIGURES[i][j];
        }
        for (int j = 0; j < 4; j++)
            if (boardFigures[i][j] == noFigure) boardColors[i][j] = noColor; else boardColors[i][j] = black;

        for (int j = 4; j < 8; j++)
            if (boardFigures[i][j] == noFigure) boardColors[i][j] = noColor; else boardColors[i][j] = white;

    }
}

bool Situation::isValidMove(int x, int y,int nx, int ny)
{
    if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8 || (nx == x && ny == y)
            || boardColors[x][y] == boardColors[nx][ny] || boardFigures[x][y] == noFigure) return false;
    switch (boardFigures[x][y]){
    case king:
        return max(abs(nx - x), abs(ny - y)) == 1 ||

                (boardColors[x][y] == black && !isKingMove1 && !isRookMove1 && nx == 2 && ny == 0 &&
                boardFigures[1][0] == noFigure && boardFigures[2][0] == noFigure && boardFigures[3][0] == noFigure &&
                !isCheckPoint(white, 3, 0) && !isCheckPoint(white, 4, 0)) ||

                (boardColors[x][y] == black && !isKingMove1 && !isRookMove2 && nx == 6 && ny == 0 &&
                boardFigures[5][0] == noFigure && boardFigures[6][0] == noFigure &&
                !isCheckPoint(white, 4, 0) && !isCheckPoint(white, 5, 0)) ||

                (boardColors[x][y] == white && !isKingMove2 && !isRookMove3 && nx == 2 && ny == 7 &&
                boardFigures[1][7] == noFigure && boardFigures[2][7] == noFigure && boardFigures[3][7] == noFigure &&
                !isCheckPoint(black, 3, 7) && !isCheckPoint(black, 4, 7)) ||

                (boardColors[x][y] == white && !isKingMove2 && !isRookMove4 && nx == 6 && ny == 7 &&
                boardFigures[5][7] == noFigure && boardFigures[6][7] == noFigure &&
                !isCheckPoint(black, 4 , 7) && !isCheckPoint(black, 5, 7));


    case queen:
        return (x == nx || y == ny || abs(nx - x) == abs(ny - y)) && !isBarrier(x, y, nx, ny);

    case rook:
        return (x == nx || y == ny) && !isBarrier(x, y, nx, ny);

    case knight:
        return abs(nx - x) + abs(ny - y) == 3 &&
                nx != x && ny != y;

    case bishop:
        return abs(nx - x) == abs(ny - y) && !isBarrier(x, y, nx, ny);

    case pawn:
        if (boardColors[x][y] == white) return (nx == x &&
                (y - ny == 1 || (y - ny == 2 && boardFigures[nx][ny + 1] == noFigure && y == 6))
                && boardFigures[nx][ny] == noFigure) ||
                (abs(nx - x) == 1 && y - ny == 1 && boardColors[nx][ny] == black) ||
                (movePawn == nx && abs(x - nx) == 1 && y == 3 && ny == 2);
        else return (nx == x &&
                (ny - y == 1 || (ny - y == 2 && boardFigures[nx][ny - 1] == noFigure && y == 1))
                && boardFigures[nx][ny] == noFigure) ||
                (abs(nx - x) == 1 && ny - y == 1 && boardColors[nx][ny] == white)||
                (movePawn == nx && abs(x - nx) == 1 && y == 4 && ny == 5);


    default:
        return false;
    }
}

bool Situation::canMove(int x, int y, int nx, int ny)
{
    if (!isValidMove(x,y,nx,ny)) return false;
    Situation n_situation;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            n_situation.boardFigures[i][j] = boardFigures[i][j];
            n_situation.boardColors[i][j] = boardColors[i][j];
        }
    }
    n_situation.move(x, y, nx, ny);
    return !n_situation.isCheck(boardColors[x][y]);
}

bool Situation::isCheckPoint(Colors ncolor, int x0, int y0){
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (boardColors[i][j] == ncolor && isValidMove(i,j,x0,y0)){
                return true;
            }
        }
    }
    return false;
}

bool Situation::isCheck(Colors color)
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

bool Situation::isBlock(Colors color){
    for (int x1 = 0; x1 < 8; x1++){
        for (int y1 = 0; y1 < 8; y1++){
            if (boardColors[x1][y1] == color){
                for (int x2 = 0; x2 < 8; x2++){
                    for (int y2 = 0; y2 < 8; y2++){
                        if (canMove(x1, y1, x2, y2)){
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void Situation::copy(Situation *a)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++){
            boardFigures[i][j] = a->boardFigures[i][j];
            boardColors[i][j] = a->boardColors[i][j];
        }
    isKingMove1 = a->isKingMove1;
    isKingMove2 = a->isKingMove2;
    isRookMove1 = a->isRookMove1;
    isRookMove2 = a->isRookMove2;
    isRookMove3 = a->isRookMove3;
    isRookMove4 = a->isRookMove4;
    movePawn = a->movePawn;
}




bool Situation::isMate(Colors color)
{
    return isCheck(color) && isBlock(color);
}

bool Situation::isStaleMate(Colors color)
{
    return !isCheck(color) && isBlock(color);
}


void Situation::move(int x1, int y1, int x2, int y2)
{
    if (x1 == 4 && y1 == 0) isKingMove1 = true;
    else if (x1 == 4 && y1 == 7) isKingMove2 = true;
    else if ((x1 == 0 && y1 == 0) || (x2 == 0 && y2 == 0)) isRookMove1 = true;
    else if ((x1 == 7 && y1 == 0) || (x2 == 7 && y2 == 0)) isRookMove2 = true;
    else if ((x1 == 0 && y1 == 7) || (x2 == 0 && y2 == 7)) isRookMove3 = true;
    else if ((x1 == 7 && y1 == 7) || (x2 == 7 && y2 == 7)) isRookMove4 = true;

    movePawn = -1;
    if (boardFigures[x1][y1] == pawn && boardFigures[x2][y2] == noFigure && abs(y1 - y2) == 2) movePawn = x1;

    if (x1 == 4 && y1 == 0 && x2 ==2 && y2 == 0) move(0,0,3,0);
    else if (x1 == 4 && y1 == 0 && x2 == 6 && y2 == 0) move(7,0,5,0);
    else if (x1 == 4 && y1 == 7 && x2 == 2 && y2 == 7) move(0,7,3,7);
    else if (x1 == 4 && y1 == 7 && x2 == 6 && y2 == 7) move(7,7,5,7);


    if (boardFigures[x1][y1] == pawn && abs(x1 - x2) == 1 && boardFigures[x2][y2] == noFigure) boardFigures[x2][y1] = noFigure;
    boardFigures[x2][y2] = boardFigures[x1][y1];
    boardFigures[x1][y1] = noFigure;
    boardColors[x2][y2] = boardColors[x1][y1];
    boardColors[x1][y1] = noColor;
    if ((y2 == 7 || y2 == 0) && boardFigures[x2][y2] == pawn){
        if (boardColors[x2][y2] == white) boardFigures[x2][y2] = pawnEnd;
        else boardFigures[x2][y2] = queen;
    }
}


bool Situation::isBarrier(int x, int y, int nx, int ny)
{
    int stepi = (x == nx) ? 0 : abs(nx - x) / (nx - x);
    int stepj = (y == ny) ? 0 : abs(ny - y) / (ny - y);
    for (int i = x + stepi, j = y + stepj; i != nx || j != ny; i += stepi, j += stepj){
        if (boardFigures[i][j] != noFigure) return true;
    }
    return false;
}

Move Situation::solve()
{

    vector<Vertex> v;
    v.push_back({*this, 0, -1, -INF, 0, {0,0,0,0}});
    int i = 0;
    while (v.back().hight < 5 || v.size() < 1000000){
        if (i == (int)v.size()) break;
        bool b = false;
        for (int x1 = 0; x1 < 8; x1++){
            for (int y1 = 0; y1 < 8; y1++){
                if ((v[i].hight % 2 == 0 && v[i].situation.boardColors[x1][y1] == black) ||
                        (v[i].hight % 2 == 1 && v[i].situation.boardColors[x1][y1] == white)){
                    for (int x2 = 0; x2 < 8; x2++){
                        for (int y2 = 0; y2 < 8; y2++){
                            if (v[i].situation.canMove(x1, y1, x2, y2)){
                                b = true;
                                v.push_back({v[i].situation, v[i].hight + 1, i, (v[i].hight % 2 == 0) ? INF : -INF, 0,
                                             {x1, y1, x2, y2}});
                                v.back().situation.move(x1,y1,x2,y2);
                            }
                        }
                    }
                }
            }
        }
        if (b == false){ // пат или мат
            if (v[i].hight % 2 == 0){ // ход черных
                if (v[i].situation.isCheck(black)) // мат
                    v[i].value = -INF / 2; // мат черным
                else v[i].value = 0; // пат черным
            }
            else{ // ход белых
                if (v[i].situation.isCheck(white)) // мат
                    v[i].value = INF / 2 - v[i].hight; // мат белым
                else v[i].value = 0; // пат белым
            }

        }
        i++;
    }

    i = v.size() - 2;
    while (v[i].hight == v[i + 1].hight){
        i--;
    }
    while (v[i].hight % 2 == 1){
        i--;
    }

    while (v[i].hight % 2 == 0 && i > 0){
        v[i].value = v[i].situation.getValue();
        if (v[i].value < v[v[i].parent].value){
            v[v[i].parent].value = v[i].value;
            v[v[i].parent].child = i;
        }
        i--;
    }

    while (i > 0){
        if (v[i].hight % 2 == 0){
            if (v[i].value < v[v[i].parent].value){
                v[v[i].parent].value = v[i].value;
                v[v[i].parent].child = i;
            }
            else if (v[i].value == v[v[i].parent].value && rand() % 2 == 0){
                v[v[i].parent].value = v[i].value;
                v[v[i].parent].child = i;
            }
        }
        else{
            if (v[i].value > v[v[i].parent].value){
                v[v[i].parent].value = v[i].value;
                v[v[i].parent].child = i;
            }
            else if (v[i].value == v[v[i].parent].value && rand() % 2 == 0){
                v[v[i].parent].value = v[i].value;
                v[v[i].parent].child = i;
            }
        }
        i--;
    }
    this->copy(&v[v[0].child].situation);

    return v[v[0].child].m;
}

pair<Move,int> Situation::solveRec(int hight, bool isBlack, int level)
{
    if (hight == level){
        if (isBlack) return {{0,0,0,0}, getValue()};
        else return {{0,0,0,0}, -getValue()};
    }
    bool b = false;
    int value;
    int r = 1;
    Move bestMove = {0,0,0,0};
    if (hight % 2 == 0) value = -INF; else value = INF;
    for (int x1 = 0; x1 < 8; x1++){
        for (int y1 = 0; y1 < 8; y1++){
            if ( (isBlack && ( (hight % 2 == 0 && boardColors[x1][y1] == black) ||
                    (hight % 2 == 1 && boardColors[x1][y1] == white) ) )||
                (!isBlack && ( (hight % 2 == 0 && boardColors[x1][y1] == white) ||
                                    (hight % 2 == 1 && boardColors[x1][y1] == black))))
            {
                for (int x2 = 0; x2 < 8; x2++){
                    for (int y2 = 0; y2 < 8; y2++){
                        if (canMove(x1, y1, x2, y2)){
                            b = true;
                            Situation *nsituation = new Situation();
                            nsituation->copy(this);
                            nsituation->move(x1,y1,x2,y2);
                            pair<Move,int> t = nsituation->solveRec(hight + 1, isBlack, level);
                            delete nsituation;
                            if ((hight % 2 == 0) == (t.second > value)) {
                                r = 1;
                                value = t.second;
                                bestMove = {x1,y1,x2,y2};
                            }
                            else if (t.second == value){
                                r++;
                                if (rand() % r == 0){
                                    value = t.second;
                                    bestMove = {x1,y1,x2,y2};
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (b == false){ // пат или мат
            if (hight % 2 == 0){ // ход черных
                if ((isBlack && isCheck(black)) || (!isBlack && isCheck(white))) // мат
                    value = -INF / 2 + hight; // мат черным
                else value = 0; // пат черным
            }
            else{ // ход белых
                if ((isBlack && isCheck(white)) || (!isBlack && isCheck(black))) // мат
                    value = INF / 2 - hight; // мат белым
                else value = 0; // пат белым
            }
    }

    move(bestMove.x1, bestMove.y1, bestMove.x2 , bestMove.y2);
    return {bestMove, value};
}

double Situation::getValue()
{
    double res = 0;
    int cntBlack = 0, cntWhite = 0, xWhite, yWhite, xBlack, yBlack;
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (boardColors[i][j] == white){
                //xWhite = i;
                //yWhite = j;
                cntWhite++;
            }
            else if (boardColors[i][j] == black){
                cntBlack++;
            }
        }
    }
    if (cntWhite == 0){
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (boardColors[i][j] == white){
                    switch (boardFigures[i][j]){
                    case king:     res -= 1000 + (i + j) * 200; break;
                    case queen:    res -= 1800 + (i + j) * 200; break;
                    case rook:     res -= 1000 + (i + j) * 200; break;
                    case knight:   res -= 1000 + (i + j) * 200; break;
                    case bishop:   res -= 1000 + (i + j) * 200; break;
                    case pawn:     res -= 1000 + (i + j) * 200; break;
                    case noFigure: res -= 0;
                    }
                }
                else if (boardColors[i][j] == black){
                    switch (boardFigures[i][j]){
                    case king:     res += 1000 + (i + j); break;
                    case queen:    res += 1000 + (i + j); break;
                    case rook:     res += 1000 + (i + j); break;
                    case knight:   res += 1000 + (i + j); break;
                    case bishop:   res += 1000 + (i + j); break;
                    case pawn:     res += 1000 + (i + j); break;
                    case noFigure: res += 0;
                    }
                }

            }
        }
    }
    else{
        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 8; j++){
                if (boardColors[i][j] == white){
                    switch (boardFigures[i][j]){
                    case king:     res -= 1000000 + KING[i][7 -j]; break;
                    case queen:    res -= 180 + QUEEN[i][7 - j]; break;
                    case rook:     res -= 100 + ROOK[i][7-j]; break;
                    case knight:   res -= 60 + KNIGHT[i][7-j]; break;
                    case bishop:   res -= 60 + BISHOP[i][7-j]; break;
                    case pawn:     res -= 20 + PAWN[i][7-j]; break;
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
    }
    return res;
}
