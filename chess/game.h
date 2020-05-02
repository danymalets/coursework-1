#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QTimer>
#include <vector>
#include <stack>
#include "figure.h"
#include "situation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Game; }
QT_END_NAMESPACE

class Game : public QMainWindow
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = 0);
    ~Game();

    Ui::Game *ui;

    int level;

    QGraphicsScene  *scene;
    stack<Situation*> situations;
    Situation *situation;
    Figure *figures[8][8];
    vector<Move> moves;

    void start();

    void writeMoves();
    void showBoard(bool allowMovement, Move lastMove = {-1,-1,-1,-1});
private slots:
    void slotFromPoint();
    void blackTurn();

    void on_pushButton_clicked();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
};

#endif // GAME_H
