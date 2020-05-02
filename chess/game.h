#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMessageBox>
#include <vector>
#include "figure.h"
#include "situation.h"
#include <QTimer>
#include <QThread>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class Game; }
QT_END_NAMESPACE

class Game : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = 0);
    ~Game();
    void showBoard(bool s, Move m = {-1,-1,-1,-1});
private:
    Ui::Game *ui;

    QGraphicsScene  *scene;
    stack<Situation*> situations;
    Situation *situation;
    Figure *figures[8][8];
    void isBlock(Situation *s, Colors color);
    void start();
    int level;
    void blackTurn();
    vector<Move> moves;
    void writeMoves();
    bool whiteTurnEnd = false, whiteTurnEndP = false;
    QTimer *timer;
private slots:
    void slotFromPoint();
    void timerOut();
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
    void on_pushButton_8_clicked();
};
#endif // GAME_H
