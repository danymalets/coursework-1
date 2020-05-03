#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Game)
{
    ui->setupUi(this);
    setWindowTitle("Шахматы");
    scene = new QGraphicsScene();
    scene->setSceneRect(-400, -400 , 800, 800);
    ui->textEdit->setReadOnly(true);
    ui->graphicsView->setScene(scene);
    level = 4;
    userColor = white;
    start();
}

void Game::start()
{
    moves.clear();
    writeMoves();
    while (!boards.empty()) boards.pop();
    ui->label_2->setText(QString::number(level));
    board = new Board();
    Board *nBoard = new Board();
    nBoard->copy(board);
    boards.push(nBoard);
    if (userColor == black){
        Move m = board->solve(0, level, 2 * INF, {0,0,0,0}).first;
        board->move(m);
        if (userColor == black) m = {m.x1, 7 - m.y1, m.x2, 7 - m.y2};
        moves.push_back(m);
        writeMoves();
        showBoard(true, m);
    }
    else showBoard(true);
}

void Game::slotFromPoint(){
    showBoard(false);
    writeMoves();
    QTimer::singleShot(100, this, SLOT(blackTurn()));
}

void Game::blackTurn()
{
    if (board->isMate(black)){
        showBoard(0);
        QMessageBox::information(this," ","Вы победили!\nШах и мат");
        Board *nBoard = new Board();
        nBoard->copy(board);
        boards.push(nBoard);
        return;
    }
    if (board->isStaleMate(black)){
        showBoard(0);
        QMessageBox::information(this," ","Ничья.\nВы поставили пат");
        Board *nBoard = new Board();
        nBoard->copy(board);
        boards.push(nBoard);
        return;
    }
    Move m = board->solve(0, level, 2 * INF, {0,0,0,0}).first;
    board->move(m);
    if (userColor == black) m = {m.x1, 7 - m.y1, m.x2, 7 - m.y2};
    moves.push_back(m);
    writeMoves();

    Board *nBoard = new Board();
    nBoard->copy(board);
    boards.push(nBoard);

    showBoard(true, m);
    if (board->isMate(white)){
        showBoard(0, m);
        QMessageBox::information(this," ","Вы проиграли.\nШах и мат");
        return;
    }
    if (board->isStaleMate(white)){
        showBoard(0, m);
        QMessageBox::information(this," ","Ничья.\nВам поставлен пат");
        return;
    }
    if (board->isCheck(white)) QMessageBox::information(this,"","Шах!");
}

void Game::showBoard(bool allowMovement, Move lastMove)
{
    scene->clear();
    scene->addRect(-430, -430, 860, 860, QPen(Qt::NoPen), QBrush(QColor("#634936")));
    QGraphicsTextItem *text;
    if (userColor == white){
        for (int i = 0; i < 8; i++){
            text = scene->addText(QString(char(i + 'a')), QFont("calibri", 18));
            text->setDefaultTextColor(QColor(214, 198, 174));
            text->setPos(-360 + i * 100, 400);
        }
        for (int i = 0; i < 8; i++){
            text = scene->addText(QString::number(i + 1), QFont("calibri", 18));
            text->setDefaultTextColor(QColor(214, 198, 174));
            text->setPos(-421, 335 - i * 100);
        }
    }
    else{
        for (int i = 0; i < 8; i++){
            text = scene->addText(QString(char((7 - i) + 'a')), QFont("calibri", 18));
            text->setDefaultTextColor(QColor(214, 198, 174));
            text->setPos(-360 + i * 100, 400);
        }
        for (int i = 0; i < 8; i++){
            text = scene->addText(QString::number(8 - i), QFont("calibri", 18));
            text->setDefaultTextColor(QColor(214, 198, 174));
            text->setPos(-421, 335 - i * 100);
        }
    }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if ((i + j) % 2 == 0){
                scene->addRect(-400 + i * 100, -400 + j * 100, 100, 100, QPen(Qt::NoPen), QBrush(Qt::gray));
            }
            else{
                scene->addRect(-400 + i * 100, -400 + j * 100, 100, 100, QPen(Qt::NoPen), QBrush(Qt::darkGray));
            }

    Move m = lastMove;
    if (userColor == black) m = {7 - m.x1, 7 - m.y1, 7 - m.x2, 7 - m.y2};
    if (m.x1 != -1){
        if ((m.x1 + m.y1) % 2 == 0)
            scene->addRect(-400 + m.x1 * 100, -400 + m.y1 * 100, 100, 100, QPen(Qt::red), QBrush(Qt::gray));
        else
            scene->addRect(-400 + m.x1 * 100, -400 + m.y1 * 100, 100, 100, QPen(Qt::red), QBrush(Qt::darkGray));

        if ((m.x2 + m.y2) % 2 == 0)
            scene->addRect(-400 + m.x2 * 100, -400 + m.y2 * 100, 100, 100, QPen(Qt::red), QBrush(Qt::gray));
        else
            scene->addRect(-400 + m.x2 * 100, -400 + m.y2 * 100, 100, 100, QPen(Qt::red), QBrush(Qt::darkGray));
    }
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            figures[i][j] = new Figure(board, &moves, i, j, allowMovement, userColor);
            scene->addItem(figures[i][j]);
            connect(figures[i][j], SIGNAL(sl()), this, SLOT(slotFromPoint()));
        }
    }
}

void Game::writeMoves()
{
    ui->textEdit->setText("");
    for (int i = 0; i < (int)moves.size(); i += 2){
        if (i + 1 == (int)moves.size())
            ui->textEdit->append(QString::number(i / 2 + 1) + ".  "+ (moves[i].x1 + 'a') +
                                 QString::number(8 - moves[i].y1) + "―" + (moves[i].x2 + 'a') +
                                 QString::number(8 - moves[i].y2));
        else
            ui->textEdit->append(QString::number(i / 2 + 1) + ".  "+ (moves[i].x1 + 'a') +
                                QString::number(8 - moves[i].y1) + "―" + (moves[i].x2 + 'a') +
                                QString::number(8 - moves[i].y2) + "   " +
                                   (moves[i + 1].x1 + 'a') +
                                   QString::number(8 - moves[i + 1].y1) + "―" + (moves[i + 1].x2 + 'a') +
                                   QString::number(8 - moves[i + 1].y2));
    }
}

void Game::on_pushButton_clicked()
{
    if (boards.size() > 1) {
        boards.pop();
        board->copy(boards.top());
        moves.pop_back();
        moves.pop_back();
        writeMoves();
        showBoard(1);
    }
}

void Game::on_radioButton_clicked()
{
    board->pawnEnd = queen;
}

void Game::on_radioButton_2_clicked()
{
    board->pawnEnd = rook;
}

void Game::on_radioButton_3_clicked()
{
    board->pawnEnd = bishop;
}

void Game::on_radioButton_4_clicked()
{
    board->pawnEnd = knight;
}

void Game::on_pushButton_2_clicked()
{
    level = 1;
    start();
}

void Game::on_pushButton_3_clicked()
{
    level = 2;
    start();
}

void Game::on_pushButton_4_clicked()
{
    level = 3;
    start();
}

void Game::on_pushButton_5_clicked()
{
    level = 4;
    start();
}

void Game::on_pushButton_6_clicked()
{
    level = 5;
    start();
}

void Game::on_pushButton_7_clicked()
{
    level = 6;
    start();
}

Game::~Game()
{
    delete ui;
}

void Game::on_pushButton_8_clicked()
{
    userColor = white;
    start();
}

void Game::on_pushButton_9_clicked()
{
    userColor = black;
    start();
}
