#include "GameModel.h"
#include <time.h>
#include <stdlib.h>
using namespace std;
void GameModel::startGame(GameType type)
{
    gameType = type;

    if(gameType==AI)
    {
        gameMapVec.clear();
        for(int i=0;i<BOARD_GRAD_SIZE;i++)
        {
            vector<int> lineBorad;
            for(int j=0;j<BOARD_GRAD_SIZE;j++)
                lineBorad.push_back(0);
            gameMapVec.push_back(lineBorad);
        }
    }

    playerFlag =true;




}
void GameModel::actionByperson(int row, int col)
{
    updateGameMap(row,col);

}
void GameModel::updateGameMap(int row, int col)
{
    if(playerFlag){
        gameMapVec[row][col] = 1;
    }else{
        gameMapVec[row][col] = -1;
    }
    //换手
    playerFlag = !playerFlag;
}

bool GameModel::isWin(int row, int col)
{
    for(int i=0;i<5;i++){
        //水平方向是否有5子
        if(row>0 && row<BOARD_GRAD_SIZE &&
                col-i>0 && col-i+4<BOARD_GRAD_SIZE &&
                gameMapVec[row][col-i] == gameMapVec[row][col-i+1] &&
                gameMapVec[row][col-i] == gameMapVec[row][col-i+2] &&
                gameMapVec[row][col-i] == gameMapVec[row][col-i+3] &&
                gameMapVec[row][col-i] == gameMapVec[row][col-i+4])
        {
            return true;
        }
        //垂直方向是否有5子
        if(row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
                col>0 && col<BOARD_GRAD_SIZE &&
                gameMapVec[row-i][col] == gameMapVec[row-i+1][col] &&
                gameMapVec[row-i][col] == gameMapVec[row-i+2][col] &&
                gameMapVec[row-i][col] == gameMapVec[row-i+3][col] &&
                gameMapVec[row-i][col] == gameMapVec[row-i+4][col])
        {
            return true;
        }
        //左下到右上是否有棋子相连
        if(
                row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
                col+i-4>0 && col+i<BOARD_GRAD_SIZE &&
                gameMapVec[row-i][col+i] == gameMapVec[row-i+1][col+i-1] &&
                gameMapVec[row-i][col+i] == gameMapVec[row-i+2][col+i-2] &&
                gameMapVec[row-i][col+i] == gameMapVec[row-i+3][col+i-3] &&
                gameMapVec[row-i][col+i] == gameMapVec[row-i+4][col+i-4])
        {
            return true;
        }
        //右下到左上是否有棋子相连
        if(row-i>0 && row-i+4<BOARD_GRAD_SIZE &&
                col-i>0 && col-i+4<BOARD_GRAD_SIZE &&
                gameMapVec[row-i][col-i] == gameMapVec[row-i+1][col-i+1] &&
                gameMapVec[row-i][col-i] == gameMapVec[row-i+2][col-i+2] &&
                gameMapVec[row-i][col-i] == gameMapVec[row-i+3][col-i+3] &&
                gameMapVec[row-i][col-i] == gameMapVec[row-i+4][col-i+4])
        {
            return true;
        }

    }
    return false;
}


void GameModel::actionByAI(int &clickRow, int &clickCol)
{

    {
        //计算评分
        calculateScore();

        //從評分中找出最大分數的位置
        int maxScore = 0;
        std::vector<std::pair<int,int>> maxPoints;
        for(int row = 1;row<BOARD_GRAD_SIZE;row++){
            for(int col = 1;col<BOARD_GRAD_SIZE;col++){
                //前提是這個坐標是空的
                if(gameMapVec[row][col] == 0){
                    if(scoreMapVec[row][col]>maxScore){     //找最大數和坐標
                        maxPoints.clear();
                        maxScore = scoreMapVec[row][col];
                        maxPoints.push_back(std::make_pair(row,col));
                    }else if(scoreMapVec[row][col] == maxScore){   //如果有多個最大值就將他們存儲起來，在後面的代碼隨機抽1個
                        maxPoints.push_back(std::make_pair(row,col));
                    }
                }
            }
        }
        //隨機落子，如果有多個點的話
        srand((unsigned)time(0));
        int index = rand()%maxPoints.size();
        std::pair<int,int> pointPair = maxPoints.at(index);
        clickRow = pointPair.first;
        clickCol = pointPair.second;
        updateGameMap(clickRow,clickCol);

    }
}


void GameModel::calculateScore()
{

    //计算AI和玩家连成的子
    int personNum = 0; //玩家连子个数
    int botNum = 0;   //AI连子个数
    int emptyNum = 0;   //各方向空白的个数

    //清空点位评分
    scoreMapVec.clear();
    for(int i=0;i<BOARD_GRAD_SIZE;i++)
    {
        std::vector<int> lineScores;
        for(int j=0;j<BOARD_GRAD_SIZE;j++)
        {
            lineScores.push_back(0);
        }
        scoreMapVec.push_back(lineScores);
    }
    //评分
    for(int row=0;row<BOARD_GRAD_SIZE;row++)
    {
        for(int col=0;col<BOARD_GRAD_SIZE;col++)
        {
            //空白點才算
            if(row>0 && col>0 && gameMapVec[row][col]==0)
            {
                //遍历周围8个方向
                for(int y=-1;y<=1;y++)// -1 0 1
                {
                    for(int x=-1;x<=1;x++)
                    {
                        //重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;
                        //原坐標不算
                        if(!(y==0 && x==0))
                        {
                            //每個方向延伸4個子

                            //對玩家黑子評分(正反兩個方向)
                            for(int i=1;i<=4;i++)
                            {
                                if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                        col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                        gameMapVec[row+i*y][col+i*x]==1)
                                { //真人玩家的子
                                    personNum++;
                                }
                                else if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                        col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                        gameMapVec[row+i*y][col+i*x]==0)
                                { //空白位
                                    emptyNum++;
                                    break;
                                }
                                else
                                { //出邊界，或有白子
                                    break;
                                }
                            }
                            for(int i=1;i<=4;i++)
                            {
                                if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                        col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                        gameMapVec[row-i*y][col-i*x]==1)
                                { //真人玩家的子
                                    personNum++;
                                }
                                else if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                        col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                        gameMapVec[row-i*y][col-i*x]==0)
                                { //空白位
                                    emptyNum++;
                                    break;
                                }else
                                { //出邊界，或有白子
                                    break;
                                }
                            }
                            if(personNum == 1)
                            {                 //殺2
                                scoreMapVec[row][col]+=10;
                            }
                            else if(personNum == 2)
                            {           //殺3
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=30;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=40;
                            }
                            else if(personNum == 3)
                            {           //殺4
                                //量變空位不一樣，優先級不一樣
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=60;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=110;
                            }
                            else if(personNum == 4)
                            {           //殺5
                                scoreMapVec[row][col]+=10100;
                            }

                            //進行一次清空
                            emptyNum = 0;

                            //對AI白子評分
                            for(int i=1;i<=4;i++)
                            {
                                if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                        col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                        gameMapVec[row+i*y][col+i*x]==-1)
                                { //AI的子
                                    botNum++;
                                }
                                else if(row+i*y>0 && row+i*y<BOARD_GRAD_SIZE &&
                                        col+i*x>0 && col+i*x<BOARD_GRAD_SIZE &&
                                        gameMapVec[row+i*y][col+i*x]==0)
                                { //空白位
                                    emptyNum++;
                                    break;
                                }
                                else
                                { //出邊界
                                    break;
                                }
                            }
                            for(int i=1;i<=4;i++)
                            {
                                if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                        col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                        gameMapVec[row-i*y][col-i*x]==-1)
                                { //AI的子
                                    botNum++;
                                }
                                else if(row-i*y>0 && row-i*y<BOARD_GRAD_SIZE &&
                                        col-i*x>0 && col-i*x<BOARD_GRAD_SIZE &&
                                        gameMapVec[row-i*y][col-i*x]==0)
                                { //空白位
                                    emptyNum++;
                                    break;
                                }
                                else
                                { //出邊界
                                    break;
                                }
                            }
                            if(botNum == 0)
                            {
                                scoreMapVec[row][col]+=5;  //活1
                            }else if(botNum == 1)
                            {
                                scoreMapVec[row][col]+=10; //活2
                            }else if(botNum == 2)
                            {         //活3
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=25;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=50;
                            }
                            else if(botNum == 3)
                            {         //活4
                                if(emptyNum == 1)
                                    scoreMapVec[row][col]+=55;
                                else if(emptyNum == 2)
                                    scoreMapVec[row][col]+=100;
                            }
                            else if(botNum >= 4)
                            {         //活5
                                scoreMapVec[row][col]+=20000;
                            }


                        }
                    }
                }
            }
        }
    }

}
