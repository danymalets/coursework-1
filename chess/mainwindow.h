#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QMessageBox>
#include <vector>
#include "figure.h"
#include "situation.h"
#include <QTimer>
#include <QThread>
#include <stack>
using namespace std;

namespace Ui {
class MainWindow;
}

//bool MoveBot;
//Move move;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showBoard(bool s, Move m = {-1,-1,-1,-1});
private:
    Ui::MainWindow *ui;

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
    bool whiteTurnEnd, whiteTurnEndP;
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
};

#endif // MAINWINDOW_H
