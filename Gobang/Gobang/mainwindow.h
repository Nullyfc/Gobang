#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameModel.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameModel *game;
    GameType game_type;

    int clickPosRow,clickPosCol;
    bool selectPos =false;

    void paintEvent(QPaintEvent *event);

    void initGame();
    void initAIGame();
    void chessOneByPerson();

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void chessOneByAI();
};
#endif // MAINWINDOW_H
