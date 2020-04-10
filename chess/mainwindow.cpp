#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1,1);
    ui->graphicsView->setScene(scene);
    level = 6;
    start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showBoard(bool s, Move m)
{
    scene->clear();
    scene->addRect(-430,-430,860,860,QPen(Qt::NoPen),QBrush(QColor("#634936")));
    QGraphicsTextItem *text;

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

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if ((i + j) % 2 == 0){
                scene->addRect(-400 + i * 100, -400 + j * 100, 100, 100,QPen(Qt::NoPen), QBrush(Qt::gray));
            }
            else{
                scene->addRect(-400 + i * 100, -400 + j * 100, 100, 100,QPen(Qt::NoPen), QBrush(Qt::darkGray));
            }

    if (m.x1 != -1){
        if ((m.x1 + m.y1) % 2 == 0)
            scene->addRect(-400 + m.x1 * 100, -400 + m.y1 * 100, 100, 100,QPen(Qt::red), QBrush(Qt::gray));
        else
            scene->addRect(-400 + m.x1 * 100, -400 + m.y1 * 100, 100, 100,QPen(Qt::red), QBrush(Qt::darkGray));

        if ((m.x2 + m.y2) % 2 == 0)
            scene->addRect(-400 + m.x2 * 100, -400 + m.y2 * 100, 100, 100,QPen(Qt::red), QBrush(Qt::gray));
        else
            scene->addRect(-400 + m.x2 * 100, -400 + m.y2 * 100, 100, 100,QPen(Qt::red), QBrush(Qt::darkGray));
    }
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            figures[i][j] = new Figure(scene, situation, &moves, i, j, s);
            scene->addItem(figures[i][j]);
            connect(figures[i][j], SIGNAL(sl()), this, SLOT(slotFromPoint()));
            timer = new QTimer();
            timer->setInterval(100);
            connect(timer, SIGNAL(timeout()), this, SLOT(timerOut()));
            timer->start();
        }
    }
}

void MainWindow::start()
{
    moves.clear();
    writeMoves();
    while (!situations.empty()) situations.pop();
    ui->label_2->setText(QString::number(level));
    situation = new Situation();
    Situation *nsituation = new Situation();
    nsituation->copy(situation);
    situations.push(nsituation);
    showBoard(1);
}

void MainWindow::slotFromPoint(){
    whiteTurnEndP = true;
    showBoard(0);
    writeMoves();
}

void MainWindow::timerOut(){
    if (whiteTurnEndP){
        whiteTurnEndP = false;
        whiteTurnEnd = true;
    }
    else if (whiteTurnEnd) {
        whiteTurnEnd = false;
        delete timer;
        blackTurn();
    }
}


void MainWindow::blackTurn()
{

    if (situation->isMate(black)){
        QMessageBox::information(this,"","Вы победили!\nШах и мат");
        showBoard(0);
        Situation *nsituation = new Situation();
        nsituation->copy(situation);
        situations.push(nsituation);
        return;
    }
    if (situation->isStaleMate(black)){
        QMessageBox::information(this,"","Ничья.\nВы поставили пат");
        showBoard(0);
        Situation *nsituation = new Situation();
        nsituation->copy(situation);
        situations.push(nsituation);
        return;
    }
    Move m = situation->solveRec(0, level, 2 * INF, {0,0,0,0}).first;
    situation->move(m);
    moves.push_back(m);
    writeMoves();

    Situation *nsituation = new Situation();
    nsituation->copy(situation);
    situations.push(nsituation);

    showBoard(1, m);
    if (situation->isMate(white)){
        showBoard(0, m);
        QMessageBox::information(this,"","Вы проиграли.\nШах и мат");
        return;
    }
    if (situation->isStaleMate(white)){
        showBoard(0, m);
        QMessageBox::information(this,"","Ничья.\nВам поставлен пат");
        return;
    }
    if (situation->isCheck(white)) QMessageBox::information(this,"","Шах!");

}

void MainWindow::writeMoves()
{
    ui->textEdit->setText("");
    for (int i = 0; i < (int)moves.size(); i+=2){
        if (i + 1 == (int)moves.size())
            ui->textEdit->append(QString::number(i/2 + 1) + ".  "+ (moves[i].x1 + 'a') +
                                 QString::number(8 - moves[i].y1) + "―" + (moves[i].x2 + 'a') +
                                 QString::number(8 - moves[i].y2));
        else
            ui->textEdit->append(QString::number(i/2 + 1) + ".  "+ (moves[i].x1 + 'a') +
                                QString::number(8 - moves[i].y1) + "―" + (moves[i].x2 + 'a') +
                                QString::number(8 - moves[i].y2) + "   " +
                                   (moves[i+1].x1 + 'a') +
                                   QString::number(8 - moves[i+1].y1) + "―" + (moves[i+1].x2 + 'a') +
                                   QString::number(8 - moves[i+1].y2));
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (situations.size() > 1) {
        situations.pop();
        situation->copy(situations.top());
        moves.pop_back();
        moves.pop_back();
        writeMoves();
        showBoard(1);
    }
}

void MainWindow::on_radioButton_clicked()
{
    situation->pawnEnd = queen;
}

void MainWindow::on_radioButton_2_clicked()
{
    situation->pawnEnd = rook;
}

void MainWindow::on_radioButton_3_clicked()
{
    situation->pawnEnd = bishop;
}

void MainWindow::on_radioButton_4_clicked()
{
    situation->pawnEnd = knight;
}

void MainWindow::on_pushButton_2_clicked()
{
    level = 1;
    start();
}

void MainWindow::on_pushButton_3_clicked()
{
    level = 2;
    start();
}

void MainWindow::on_pushButton_4_clicked()
{
    level = 3;
    start();
}

void MainWindow::on_pushButton_5_clicked()
{
    level = 4;
    start();
}

void MainWindow::on_pushButton_6_clicked()
{
    level = 5;
    start();
}

void MainWindow::on_pushButton_7_clicked()
{
    level = 6;
    start();
}

void MainWindow::on_pushButton_8_clicked()
{
    level = 7;
    start();
}
