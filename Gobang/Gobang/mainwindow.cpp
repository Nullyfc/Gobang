#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <QTimer>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // setMouseTracking(true);

    //设置窗口大小
    setFixedSize(MARGIN*2+BLOCK_SIZE *BOARD_GRAD_SIZE,
                 MARGIN*2+BLOCK_SIZE *BOARD_GRAD_SIZE);

    initGame();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/image/chessboard_background.png");//背景图片

    QPixmap white("://image/white.png");//白棋
    QPixmap black("://image/black.png");//黑旗

    painter.drawPixmap(this->rect(),pixmap);
    painter.setRenderHint(QPainter::Antialiasing,true);//抗锯齿

    for(int i=0;i<BOARD_GRAD_SIZE+1 ;i++)
    {
        //从左到右画横线
        painter.drawLine(MARGIN+BLOCK_SIZE*i,MARGIN,
                         MARGIN+BLOCK_SIZE*i,size().width()-MARGIN);

        //从上到下画竖线
        painter.drawLine(MARGIN,MARGIN+BLOCK_SIZE*i,
                         size().width()-MARGIN,MARGIN+BLOCK_SIZE*i);

    }

    //绘制选中点

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    if(clickPosRow>0 && clickPosRow <BOARD_GRAD_SIZE &&
            clickPosCol >0 && clickPosCol<BOARD_GRAD_SIZE &&
            game->gameMapVec[clickPosRow][clickPosCol]==0)
    {
        if(game->playerFlag)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);


        painter.drawRect(MARGIN+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,MARGIN+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,MARK_SIZE,MARK_SIZE);



    }
    //绘制棋子
    for(int i=0;i<BOARD_GRAD_SIZE;i++)
    {
        for(int j=0;j<BOARD_GRAD_SIZE;j++)
        {
            if(game->gameMapVec[i][j]==1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,
                                    MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,
                                    CHESS_RADIUS*2,CHESS_RADIUS*2);
            }
            else if(game->gameMapVec[i][j]==-1)
            {
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                painter.drawEllipse(MARGIN+BLOCK_SIZE*j-CHESS_RADIUS,
                                    MARGIN+BLOCK_SIZE*i-CHESS_RADIUS,
                                    CHESS_RADIUS*2,CHESS_RADIUS*2);
            }

        }

    }
    //判断输赢
    if(clickPosCol>0 && clickPosCol<BOARD_GRAD_SIZE &&
       clickPosRow>0 && clickPosRow<BOARD_GRAD_SIZE &&
       (game->gameMapVec[clickPosRow][clickPosCol]==1||
        game->gameMapVec[clickPosRow][clickPosCol]==-1))
    {
        if(game->isWin(clickPosRow,clickPosCol) && game->gameStatus == PLAYING)
        {
            game->gameStatus = WIN;
            QString str;
            str = game->gameMapVec[clickPosRow][clickPosCol]==1?"黑棋":"白棋";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this,"五子棋",str+"胜利");
            if(btnValue == QMessageBox::Ok)
            {
                game->startGame(game_type);
                game->gameStatus = PLAYING;
            }

        }

    }
}
void MainWindow::initGame()
{
    game=new GameModel;
    initAIGame();

}
void MainWindow::initAIGame()
{
    game_type =AI;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->x();
    int y=event->y();
    if(x>=MARGIN+BLOCK_SIZE/2 &&
            x<size().width()-MARGIN-BLOCK_SIZE/2 &&
            y>=MARGIN +BLOCK_SIZE/2 &&
            y<size().height()-MARGIN-BLOCK_SIZE/2)
    {
        int col=(x-MARGIN)/BLOCK_SIZE;
        int row=(y-MARGIN)/BLOCK_SIZE;

        int leftTopPosX=MARGIN +BLOCK_SIZE *col;
        int leftTopPosY=MARGIN +BLOCK_SIZE *row;

        clickPosCol=-1;
        clickPosRow=-1;


        int len=0;

        selectPos =false;

        len=sqrt((x-leftTopPosX)*(x-leftTopPosX)+(y-leftTopPosY)*(y-leftTopPosY));
        if(len<POS_OFFSET)
        {
            clickPosCol=col;
            clickPosRow=row;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0)
            {
                selectPos=true;

            }

        }
        len=sqrt((x-leftTopPosX-BLOCK_SIZE)*(x-leftTopPosX-BLOCK_SIZE)+(y-leftTopPosY-BLOCK_SIZE)*(y-leftTopPosY-BLOCK_SIZE));
        if(len<POS_OFFSET)
        {
            clickPosCol=col+1;
            clickPosRow=row+1;
            if(game->gameMapVec[clickPosRow][clickPosCol]==0)
            {
                selectPos=true;

            }

        }


    }
    update();


}


void MainWindow::mouseReleaseEvent(QMouseEvent *event)

{

    if(selectPos == false){
        return;
    }
    else{
        selectPos = false;
    }
    //由人來下棋

    chessOneByPerson();
    if(game_type == AI)
    {
        //AI 下棋
        QTimer::singleShot(AI_THINK_TIME,this,SLOT(chessOneByAI()));

    }
}

void MainWindow::chessOneByPerson()
{
    if(clickPosRow!=-1 && clickPosCol!=-1 &&
            game->gameMapVec[clickPosRow][clickPosCol]==0)
    {

        game->actionByperson(clickPosRow,clickPosCol);
        //音效
        //TODO:


        update();

    }
}

void MainWindow::chessOneByAI()
{
    game->actionByAI(clickPosRow,clickPosCol);
    update();

}




